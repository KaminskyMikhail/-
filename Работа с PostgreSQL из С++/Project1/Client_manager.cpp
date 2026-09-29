#include "сlient_manager.h"
#include <stdexcept>

ClientManager::ClientManager(const std::string& conn_str)
    : conn_(conn_str)
{
    if (!conn_.is_open()) {
        throw std::runtime_error("Не удалось открыть соединение с БД");
    }
}

void ClientManager::create_tables() {
    pqxx::work tx(conn_);

    tx.exec(R"(
        CREATE TABLE IF NOT EXISTS clients (
            id         SERIAL PRIMARY KEY,
            first_name VARCHAR(100) NOT NULL,
            last_name  VARCHAR(100) NOT NULL,
            email      VARCHAR(255) NOT NULL UNIQUE
        );
    )");

    tx.exec(R"(
        CREATE TABLE IF NOT EXISTS phones (
            id        SERIAL PRIMARY KEY,
            client_id INTEGER NOT NULL REFERENCES clients(id) ON DELETE CASCADE,
            phone     VARCHAR(30) NOT NULL,
            UNIQUE (client_id, phone)
        );
    )");

    tx.commit();
}

int ClientManager::add_client(const std::string& first_name,
    const std::string& last_name,
    const std::string& email)
{
    pqxx::work tx(conn_);
    pqxx::result r = tx.exec(
        "INSERT INTO clients (first_name, last_name, email) "
        "VALUES ($1, $2, $3) RETURNING id",
        pqxx::params{ first_name, last_name, email }
    );
    tx.commit();
    return r[0][0].as<int>();
}

void ClientManager::add_phone(int client_id, const std::string& phone) {
    pqxx::work tx(conn_);
    tx.exec(
        "INSERT INTO phones (client_id, phone) VALUES ($1, $2)",
        pqxx::params{ client_id, phone }
    );
    tx.commit();
}

void ClientManager::update_client(int client_id,
    const std::optional<std::string>& first_name,
    const std::optional<std::string>& last_name,
    const std::optional<std::string>& email)
{
    std::vector<std::string> parts;
    int param_index = 1;

    if (first_name) parts.push_back("first_name = $" + std::to_string(param_index++));
    if (last_name)  parts.push_back("last_name  = $" + std::to_string(param_index++));
    if (email)      parts.push_back("email      = $" + std::to_string(param_index++));

    if (parts.empty()) return;

    std::string sql = "UPDATE clients SET ";
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i) sql += ", ";
        sql += parts[i];
    }
    sql += " WHERE id = $" + std::to_string(param_index);

    pqxx::params params;
    if (first_name) params.append(*first_name);
    if (last_name)  params.append(*last_name);
    if (email)      params.append(*email);
    params.append(client_id);

    pqxx::work tx(conn_);
    tx.exec(sql, params);
    tx.commit();
}

void ClientManager::delete_phone(int client_id, const std::string& phone) {
    pqxx::work tx(conn_);
    tx.exec(
        "DELETE FROM phones WHERE client_id = $1 AND phone = $2",
        pqxx::params{ client_id, phone }
    );
    tx.commit();
}

void ClientManager::delete_client(int client_id) {
    pqxx::work tx(conn_);
    tx.exec(
        "DELETE FROM clients WHERE id = $1",
        pqxx::params{ client_id }
    );
    tx.commit();
}

std::vector<Client> ClientManager::find_clients(const std::string& query) {
    pqxx::work tx(conn_);
    std::string pattern = "%" + query + "%";

    pqxx::result r = tx.exec(R"(
        SELECT DISTINCT c.id, c.first_name, c.last_name, c.email
        FROM clients c
        LEFT JOIN phones p ON p.client_id = c.id
        WHERE c.first_name ILIKE $1
           OR c.last_name  ILIKE $1
           OR c.email      ILIKE $1
           OR p.phone      ILIKE $1
        ORDER BY c.id
    )", pqxx::params{ pattern });

    std::vector<Client> result;
    for (const auto& row : r) {
        Client cl;
        cl.id = row["id"].as<int>();
        cl.first_name = row["first_name"].as<std::string>();
        cl.last_name = row["last_name"].as<std::string>();
        cl.email = row["email"].as<std::string>();
        result.push_back(std::move(cl));
    }

    // Подтягиваем телефоны для каждого найденного клиента
    for (auto& cl : result) {
        pqxx::result pr = tx.exec(
            "SELECT phone FROM phones WHERE client_id = $1 ORDER BY id",
            pqxx::params{ cl.id }
        );
        for (const auto& prow : pr) {
            cl.phones.push_back(prow["phone"].as<std::string>());
        }
    }

    tx.commit();
    return result;
}

std::optional<Client> ClientManager::get_client(int client_id) {
    pqxx::work tx(conn_);

    pqxx::result r = tx.exec(
        "SELECT id, first_name, last_name, email FROM clients WHERE id = $1",
        pqxx::params{ client_id }
    );

    if (r.empty()) {
        tx.commit();
        return std::nullopt;
    }

    Client cl;
    cl.id = r[0]["id"].as<int>();
    cl.first_name = r[0]["first_name"].as<std::string>();
    cl.last_name = r[0]["last_name"].as<std::string>();
    cl.email = r[0]["email"].as<std::string>();

    pqxx::result pr = tx.exec(
        "SELECT phone FROM phones WHERE client_id = $1 ORDER BY id",
        pqxx::params{ client_id }
    );
    for (const auto& prow : pr) {
        cl.phones.push_back(prow["phone"].as<std::string>());
    }

    tx.commit();
    return cl;
}
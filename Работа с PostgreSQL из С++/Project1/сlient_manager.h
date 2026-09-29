#pragma once

#include <string>
#include <vector>
#include <optional>
#include <pqxx/pqxx>

// Структура, описывающая клиента
struct Client {
    int id = 0;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::vector<std::string> phones;
};

class ClientManager {
public:
    explicit ClientManager(const std::string& conn_str);

    // Создать таблицы (идемпотентно — можно вызывать повторно)
    void create_tables();

    // Добавить нового клиента. Возвращает id созданной записи.
    int add_client(const std::string& first_name,
        const std::string& last_name,
        const std::string& email);

    // Добавить телефон существующему клиенту
    void add_phone(int client_id, const std::string& phone);

    // Изменить данные клиента. Пустые optional-поля не меняются.
    void update_client(int client_id,
        const std::optional<std::string>& first_name = std::nullopt,
        const std::optional<std::string>& last_name = std::nullopt,
        const std::optional<std::string>& email = std::nullopt);

    // Удалить конкретный телефон у клиента
    void delete_phone(int client_id, const std::string& phone);

    // Удалить клиента (телефоны уйдут каскадом)
    void delete_client(int client_id);

    // Поиск по имени, фамилии, email или телефону
    std::vector<Client> find_clients(const std::string& query);

    // Дополнительно: получить одного клиента по id
    std::optional<Client> get_client(int client_id);

private:
    pqxx::connection conn_;
};
#include "сlient_manager.h"
#include <iostream>

void print_client(const Client& c) {
    std::cout << "  ID: " << c.id
        << " | " << c.first_name << " " << c.last_name
        << " | " << c.email << "\n";
    if (c.phones.empty()) {
        std::cout << "    Телефоны: (нет)\n";
    }
    else {
        std::cout << "    Телефоны: ";
        for (size_t i = 0; i < c.phones.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << c.phones[i];
        }
        std::cout << "\n";
    }
}

void print_results(const std::string& title, const std::vector<Client>& clients) {
    std::cout << "=== " << title << " ===\n";
    if (clients.empty()) {
        std::cout << "  (ничего не найдено)\n";
    }
    else {
        for (const auto& c : clients) print_client(c);
    }
    std::cout << "\n";
}

int main() {
    try {
        ClientManager mgr(
            "host=localhost port=5432 dbname=clients "
            "user=postgres password=12345678"
        );

        // Создаём структуру БД
        mgr.create_tables();

        // 2. Добавляем клиентов
        int id_ivan = mgr.add_client("Иван", "Иванов", "ivanov@mail.ru");
        int id_petr = mgr.add_client("Пётр", "Петров", "petrov@mail.ru");
        int id_anna = mgr.add_client("Анна", "Сидорова", "sidorova@mail.ru");

        // Добавляем телефоны
        mgr.add_phone(id_ivan, "+7-900-111-22-33");
        mgr.add_phone(id_ivan, "+7-900-444-55-66");
        mgr.add_phone(id_petr, "+7-900-777-88-99");
        // У Анны телефонов нет — это допустимо

        //  Изменяем email Анны
        mgr.update_client(id_anna, std::nullopt, std::nullopt, "anna.new@mail.ru");

        // Удаляем один телефон у Ивана
        mgr.delete_phone(id_ivan, "+7-900-444-55-66");

        // Поиск по разным полям
        print_results("Поиск по имени 'Иван'", mgr.find_clients("Иван"));
        print_results("Поиск по фамилии 'Петров'", mgr.find_clients("Петров"));
        print_results("Поиск по email 'sidorova'", mgr.find_clients("sidorova"));
        print_results("Поиск по телефону '777'", mgr.find_clients("777"));
        print_results("Поиск 'mail.ru' (все)", mgr.find_clients("mail.ru"));

        // Удаляем клиента 
        mgr.delete_client(id_petr);
        print_results("После удаления Петра", mgr.find_clients("Петров"));

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
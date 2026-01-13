#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Product.h"
#include "OrderItem.h" 
#include "Order.h"
#include "User.h"
#include "Customer.h"
#include "Manager.h"
#include "Admin.h"
#include "DatabaseConnection.h"
#include "PaymentStrategy.h"
#include "Payment.h"

using namespace std;

void displayMainMenu();
void loginAsAdmin(shared_ptr<DatabaseConnection<string>> db);
void loginAsManager(shared_ptr<DatabaseConnection<string>> db);
void loginAsCustomer(shared_ptr<DatabaseConnection<string>> db);
void adminMenu(shared_ptr<Admin> admin);
void managerMenu(shared_ptr<Manager> manager);
void customerMenu(shared_ptr<Customer> customer);
void clearInputBuffer();

int main() {
    cout << "========================================" << endl;
    cout << "     СИСТЕМА ИНТЕРНЕТ-МАГАЗИНА          " << endl;
    cout << "========================================" << endl;

    try {
        cout << endl;
        cout << "Подключение к базе данных..." << endl;
        auto db = make_shared<DatabaseConnection<string>>(
            "host=localhost user=postgres password=19942007 dbname=online_store_db"
        );

        cout << "Подключение успешно установлено." << endl;

        // Главное меню
        int choice = 0;
        do {
            displayMainMenu();
            cin >> choice;
            clearInputBuffer();

            switch (choice) {
            case 1:
                loginAsAdmin(db);
                break;
            case 2:
                loginAsManager(db);
                break;
            case 3:
                loginAsCustomer(db);
                break;
            case 4:
                cout << endl;
                cout << "Выход из системы. До свидания!" << endl;
                break;
            default:
                cout << endl;
                cout << "Неверный выбор. Попробуйте снова." << endl;
            }
        } while (choice != 4);

    }
    catch (const exception& e) {
        cout << endl;
        cerr << "Критическая ошибка: " << e.what() << endl;
        cout << "Проверьте:" << endl;
        cout << "1. Запущен ли PostgreSQL" << endl;
        cout << "2. Правильность пароля в строке подключения" << endl;
        cout << "3. Существует ли база данных online_store_db" << endl;
        return 1;
    }

    return 0;
}

void displayMainMenu() {
    cout << endl;
    cout << "=== ГЛАВНОЕ МЕНЮ ===" << endl;
    cout << "1. Войти как Администратор" << endl;
    cout << "2. Войти как Менеджер" << endl;
    cout << "3. Войти как Покупатель" << endl;
    cout << "4. Выход" << endl;
    cout << "Выберите роль: ";
}

// Вход как администратор
void loginAsAdmin(shared_ptr<DatabaseConnection<string>> db) {
    cout << endl;
    cout << "=== ВХОД АДМИНИСТРАТОРА ===" << endl;

    
    try {
        auto result = db->executeQuery(
            "SELECT user_id, name, email FROM users WHERE role = 'admin' LIMIT 1;"
        );

        if (result.empty()) {
            cout << "Администратор не найден в базе данных." << endl;
            cout << "Запустите sample_data.sql для создания тестовых данных." << endl;
            return;
        }

        int userId = stoi(result[0][0]);
        string name = result[0][1];
        string email = result[0][2];

        auto admin = make_shared<Admin>(userId, name, email, db);
        cout << "Добро пожаловать, " << name << " (Администратор)!" << endl;

        adminMenu(admin);

    }
    catch (const exception& e) {
        cerr << "Ошибка входа: " << e.what() << endl;
    }
}

// Вход как менеджер
void loginAsManager(shared_ptr<DatabaseConnection<string>> db) {
    cout << endl;
    cout << "=== ВХОД МЕНЕДЖЕРА ===" << endl;

    try {
        auto result = db->executeQuery(
            "SELECT user_id, name, email FROM users WHERE role = 'manager' LIMIT 1;"
        );

        if (result.empty()) {
            cout << "Менеджер не найден в базе данных." << endl;
            return;
        }

        int userId = stoi(result[0][0]);
        string name = result[0][1];
        string email = result[0][2];

        auto manager = make_shared<Manager>(userId, name, email, db);
        cout << "Добро пожаловать, " << name << " (Менеджер)!" << endl;

        managerMenu(manager);

    }
    catch (const exception& e) {
        cerr << "Ошибка входа : " << e.what() << endl;
    }
}

// Вход как покупатель
void loginAsCustomer(shared_ptr<DatabaseConnection<string>> db) {
    cout << endl;
    cout << "=== ВХОД ПОКУПАТЕЛЯ ===" << endl;

    // Показываем список покупателей для выбора
    try {
        auto result = db->executeQuery(
            "SELECT user_id, name, email FROM users WHERE role = 'customer' ORDER BY name;"
        );

        if (result.empty()) {
            cout << "Покупатели не найдены в базе данных." << endl;
            return;
        }

        cout << "Выберите покупателя:" << endl;
        for (size_t i = 0; i < result.size(); ++i) {
            cout << i + 1 << ". " << result[i][1] << " (" << result[i][2] << ")" << endl;
        }

        int choice;
        cout << "Ваш выбор: ";
        cin >> choice;
        clearInputBuffer();

        if (choice < 1 || choice > static_cast<int>(result.size())) {
            cout << "Неверный выбор." << endl;
            return;
        }

        int userId = stoi(result[choice - 1][0]);
        string name = result[choice - 1][1];
        string email = result[choice - 1][2];

        auto customer = make_shared<Customer>(userId, name, email, db);
        cout << "Добро пожаловать, " << name << "!" << endl;

        customerMenu(customer);

    }
    catch (const exception& e) {
        cerr << "Ошибка входа: " << e.what() << endl;
    }
}

// Меню администратора
void adminMenu(shared_ptr<Admin> admin) {
    int choice = 0;
    do {
        admin->displayMenu();
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: { // Добавить продукт
            string name;
            double price;
            int quantity;

            cout << "Введите название товара: ";
            getline(cin, name);

            cout << "Введите цену: ";
            cin >> price;

            cout << "Введите количество: ";
            cin >> quantity;
            clearInputBuffer();

            admin->addProduct(name, price, quantity);
            break;
        }
        case 2: { // Обновить продукт
            int productId;
            double newPrice;
            int newQuantity;

            cout << "Введите ID товара: ";
            cin >> productId;

            cout << "Введите новую цену: ";
            cin >> newPrice;

            cout << "Введите новое количество: ";
            cin >> newQuantity;
            clearInputBuffer();

            admin->updateProduct(productId, newPrice, newQuantity);
            break;
        }
        case 3: { // Удалить продукт
            int productId;
            cout << "Введите ID товара для удаления: ";
            cin >> productId;
            clearInputBuffer();

            admin->deleteProduct(productId);
            break;
        }
        case 4: // Просмотр всех заказов
            admin->viewAllOrders();
            break;
        case 5: { // Просмотр деталей заказа
            int orderId;
            cout << "Введите ID заказа: ";
            cin >> orderId;
            clearInputBuffer();

            admin->viewOrderDetails(orderId);
            break;
        }
        case 6: { // Изменить статус заказа
            int orderId;
            string newStatus;

            cout << "Введите ID заказа: ";
            cin >> orderId;
            clearInputBuffer();
            cout << "pending - ожидание/completed - завершен/canceled - отменен/returned - возврат" << endl;
            cout << "Введите новый статус (pending/completed/canceled/returned): ";
            
            getline(cin, newStatus);

            admin->updateOrderStatus(orderId, newStatus);
            break;
        }
        case 7: { // История статусов заказа
            int orderId;
            cout << "Введите ID заказа: ";
            cin >> orderId;
            clearInputBuffer();

            admin->viewOrderStatusHistory(orderId);
            break;
        }
        case 8: // Журнал аудита
            admin->viewAuditLog();
            break;
        case 9: // CSV отчёт
            admin->generateCSVReport();
            break;
        case 10: // Выход
            cout << "Выход из меню администратора." << endl;
            break;
        default:
            cout << "Неверный выбор." << endl;
        }

        if (choice != 10) {
            cout << endl;
            cout << "Нажмите Enter для продолжения...";
            cin.get();
        }

    } while (choice != 10);
}

// Меню менеджера
void managerMenu(shared_ptr<Manager> manager) {
    int choice = 0;
    do {
        manager->displayMenu();
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: // Заказы в ожидании
            manager->viewPendingOrders();
            break;
        case 2: { // Утвердить заказ
            int orderId;
            cout << "Введите ID заказа для утверждения: ";
            cin >> orderId;
            clearInputBuffer();

            manager->approveOrder(orderId);
            break;
        }
        case 3: { // Обновить склад
            int productId, newQuantity;

            cout << "Введите ID товара: ";
            cin >> productId;

            cout << "Введите новое количество: ";
            cin >> newQuantity;
            clearInputBuffer();

            manager->updateStock(productId, newQuantity);
            break;
        }
        case 4: { // Детали заказа
            int orderId;
            cout << "Введите ID заказа: ";
            cin >> orderId;
            clearInputBuffer();

            manager->viewOrderDetails(orderId);
            break;
        }
        case 5: { // Изменить статус заказа
            int orderId;
            string newStatus;

            cout << "Введите ID заказа: ";
            cin >> orderId;
            clearInputBuffer();

            cout << "Введите новый статус: ";
            getline(cin, newStatus);

            // Менеджер может менять только на определённые статусы
            if (newStatus == "completed" || newStatus == "pending") {
                cout << "Статус изменён." << endl;
            }
            else {
                cout << "Менеджер не может установить этот статус." << endl;
            }
            break;
        }
        case 6: // История утверждённых заказов
            manager->viewMyApprovedOrders();
            break;
        case 7: { // История статусов
            int orderId;
            cout << "Введите ID заказа: ";
            cin >> orderId;
            clearInputBuffer();

            cout << "Функция в разработке..." << endl;
            break;
        }
        case 8: // Выход
            cout << "Выход из меню менеджера." << endl;
            break;
        default:
            cout << "Неверный выбор." << endl;
        }

        if (choice != 8) {
            cout << endl;
            cout << "Нажмите Enter для продолжения...";
            cin.get();
        }

    } while (choice != 8);
}

// Меню покупателя
void customerMenu(shared_ptr<Customer> customer) {
    int choice = 0;
    do {
        customer->displayMenu();
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: { // Создать заказ
            auto order = customer->createOrder();
            if (order) {
                cout << "Заказ создан. ID: " << order->getOrderId() << endl;
            }
            break;
        }
        case 2: { // Добавить товар в заказ
            int orderId, productId, quantity;

            cout << "Введите ID вашего заказа: ";
            cin >> orderId;

            auto products = customer->getAvailableProducts();
            if (products.empty()) {
                break;
            }

            cout << "Введите ID товара: ";
            cin >> productId;

            cout << "Введите количество: ";
            cin >> quantity;
            clearInputBuffer();

            customer->addToOrder(orderId, productId, quantity);
            break;
        }
        case 3: { // Удалить товар из заказа
            int orderId, productId;

            cout << "Введите ID вашего заказа: ";
            cin >> orderId;

            cout << "Введите ID товара для удаления: ";
            cin >> productId;
            clearInputBuffer();

            customer->removeFromOrder(orderId, productId);
            break;
        }
        case 4: // Мои заказы
            customer->viewMyOrders();
            break;
        case 5: { // Статус заказа
            int orderId;
            cout << "Введите ID заказа: ";
            cin >> orderId;
            clearInputBuffer();

            customer->viewOrderStatus(orderId);
            break;
        }
        case 6: { // Оплатить заказ
            int orderId;
            string paymentMethod, details;

            cout << "Введите ID заказа: ";
            cin >> orderId;
            clearInputBuffer();

            cout << "Выберите способ оплаты (карта/кошелёк/сбп): ";
            getline(cin, paymentMethod);

            if (paymentMethod == "карта") {
                cout << "Введите данные карты (номер,срок,CVV): ";
                getline(cin, details);
            }
            else if (paymentMethod == "кошелёк") {
                cout << "Введите ID кошелька: ";
                getline(cin, details);
            }
            else if (paymentMethod == "сбп") {
                cout << "Введите номер телефона: ";
                getline(cin, details);
            }
            else {
                cout << "Неверный способ оплаты." << endl;
                break;
            }

            customer->makePayment(orderId, paymentMethod, details);
            break;
        }
        case 7: { // Возврат заказа
            int orderId;
            cout << "Введите ID заказа для возврата: ";
            cin >> orderId;
            clearInputBuffer();

            customer->returnOrder(orderId);
            break;
        }
        case 8: { // История статусов
            int orderId;
            cout << "Введите ID заказа: ";
            cin >> orderId;
            clearInputBuffer();

            customer->viewOrderStatusHistory(orderId);
            break;
        }
        case 9: // Доступные товары
            customer->getAvailableProducts();
            break;
        case 10: // Выход
            cout << "Выход из меню покупателя." << endl;
            break;
        default:
            cout << "Неверный выбор." << endl;
        }

        if (choice != 10) {
            cout << endl;
            cout << "Нажмите Enter для продолжения...";
            cin.get();
        }

    } while (choice != 10);
}

// Очистка буфера ввода
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
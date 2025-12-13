#include "Database.h"
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

Database::Database() : conn(nullptr), connected(false),
categoryObj(nullptr), dishObj(nullptr), orderObj(nullptr) {
}

Database::~Database() {
    delete categoryObj;
    delete dishObj;
    delete orderObj;
    disconnect();
}

bool Database::connect() {
    try {
        string connectionString =
            "dbname=restaurant_db "
            "host=localhost "
            "port=5432 "
            "user=postgres "
            "password=19942007";
        conn = new pqxx::connection(connectionString);

        if (conn->is_open()) {
            connected = true;
            categoryObj = new Category(conn);
            dishObj = new MainCourse(conn);
            orderObj = new Order(conn);
            cout << "Успешное подключение к базе данных: " << conn->dbname() << endl;
            return true;
        }
    }
    catch (const exception& e) {
        cout << "Ошибка подключения к БД: " << e.what() << endl;
    }
    return false;
}

void Database::disconnect() {
    delete categoryObj;
    delete dishObj;
    delete orderObj;
    categoryObj = nullptr;
    dishObj = nullptr;
    orderObj = nullptr;

    if (conn && conn->is_open()) {
        conn->close();
    }
    delete conn;
    conn = nullptr;
    connected = false;
    cout << "Отключено от БД" << endl;
}

// Заказы (делегируем Order)
bool Database::addOrder(int dishId, int quantity, double totalPrice) {
    return orderObj ? orderObj->add(dishId, quantity, totalPrice) : false;
}

vector<tuple<int, int, string, int, double>> Database::getAllOrders() {
    return orderObj ? orderObj->getAll() : vector<tuple<int, int, string, int, double>>();
}

// Аналитические методы (оставляем как есть - они работают напрямую с БД)
vector<pair<string, double>> Database::getRevenueByCategory() {
    vector<pair<string, double>> revenue;

    if (!connected || !conn) return revenue;

    try {
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec(
            "SELECT c.category_name, SUM(o.total_price) as total "
            "FROM orders o "
            "JOIN dishes d ON o.dish_id = d.id "
            "JOIN categories c ON d.category_id = c.id "
            "GROUP BY c.category_name "
            "ORDER BY total DESC"
        );

        for (const auto& row : result) {
            revenue.emplace_back(row[0].as<string>(), row[1].as<double>());
        }
    }
    catch (const exception& e) {
        cout << "Ошибка получения выручки: " << e.what() << endl;
    }

    return revenue;
}

vector<pair<string, int>> Database::getTopSellingDishes(int limit) {
    vector<pair<string, int>> topDishes;

    if (!connected || !conn) return topDishes;

    try {
        pqxx::work txn(*conn);
        string query = "SELECT d.name, SUM(o.quantity) as total_quantity "
            "FROM orders o "
            "JOIN dishes d ON o.dish_id = d.id "
            "GROUP BY d.name "
            "ORDER BY total_quantity DESC "
            "LIMIT " + to_string(limit);
        pqxx::result result = txn.exec(query);

        for (const auto& row : result) {
            topDishes.emplace_back(row[0].as<string>(), row[1].as<int>());
        }
    }
    catch (const exception& e) {
        cout << "Ошибка получения топ-блюд: " << e.what() << endl;
    }

    return topDishes;
}

double Database::getAverageOrderValue() {
    if (!connected || !conn) return 0.0;

    try {
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec("SELECT AVG(total_price) FROM orders");

        if (!result.empty()) {
            return result[0][0].as<double>();
        }
    }
    catch (const exception& e) {
        cout << "Ошибка получения среднего чека: " << e.what() << endl;
    }

    return 0.0;
}

vector<pair<string, int>> Database::getDishOrderCount() {
    vector<pair<string, int>> dishCounts;

    if (!connected || !conn) return dishCounts;

    try {
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec(
            "SELECT d.name, SUM(o.quantity) as total_orders "
            "FROM orders o "
            "JOIN dishes d ON o.dish_id = d.id "
            "GROUP BY d.name "
            "ORDER BY total_orders DESC"
        );

        for (const auto& row : result) {
            dishCounts.emplace_back(row[0].as<string>(), row[1].as<int>());
        }
    }
    catch (const exception& e) {
        cout << "Ошибка получения статистики блюд: " << e.what() << endl;
    }

    return dishCounts;
}
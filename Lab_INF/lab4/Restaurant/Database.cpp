#include "Database.h"
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

Database::Database() : conn(nullptr), connected(false) {}

Database::~Database() {
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
    if (conn && conn->is_open()) {
        conn->close();
    }
    delete conn;
    conn = nullptr;
    connected = false;
    cout << "Отключено от БД" << endl;
}


bool Database::addCategory(const string& name) {
    if (!connected) return false;

    try {
        pqxx::work txn(*conn);
        txn.exec("INSERT INTO categories (category_name) VALUES (" + txn.quote(name) + ")");
        txn.commit();
        return true;
    }
    catch (const exception& e) {
        cout << "Ошибка добавления категории: " << e.what() << endl;
        return false;
    }
}

vector<pair<int, string>> Database::getAllCategories() {
    vector<pair<int, string>> categories;
    
    if (!connected) return categories;
    
    try {
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec("SELECT id, category_name FROM categories ORDER BY id");
        
        for (const auto& row : result) {
            categories.emplace_back(row[0].as<int>(), row[1].as<string>());
        }
    }
    catch (const exception& e) {
        cout << "Ошибка получения категорий: " << e.what() << endl;
    }
    
    return categories;
}


bool Database::addDish(const string& name, int categoryId, double price, const string& type) {
    if (!connected) return false;

    try {
        pqxx::work txn(*conn);
        string query = "INSERT INTO dishes (name, category_id, price) VALUES (" +
            txn.quote(name) + ", " +
            to_string(categoryId) + ", " +
            to_string(price) + ")";
        txn.exec(query);
        txn.commit();
        return true;
    }
    catch (const exception& e) {
        cout << "Ошибка добавления блюда: " << e.what() << endl;
        return false;
    }
}

vector<tuple<int, string, int, double>> Database::getAllDishes() {
    vector<tuple<int, string, int, double>> dishes;
    
    if (!connected) return dishes;
    
    try {
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec(
            "SELECT d.id, d.name, d.category_id, d.price, c.category_name "
            "FROM dishes d JOIN categories c ON d.category_id = c.id "
            "ORDER BY d.id"
        );
        
        for (const auto& row : result) {
            dishes.emplace_back(
                row[0].as<int>(),
                row[1].as<string>(),
                row[2].as<int>(),
                row[3].as<double>()
            );
        }
    }
    catch (const exception& e) {
        cout << "Ошибка получения блюд: " << e.what() << endl;
    }
    
    return dishes;
}

bool Database::addOrder(int dishId, int quantity, double totalPrice) {
    if (!connected) return false;

    try {
        pqxx::work txn(*conn);
        string query = "INSERT INTO orders (dish_id, quantity, total_price) VALUES (" +
            to_string(dishId) + ", " +
            to_string(quantity) + ", " +
            to_string(totalPrice) + ")";
        txn.exec(query);
        txn.commit();
        return true;
    }
    catch (const exception& e) {
        cout << "Ошибка добавления заказа: " << e.what() << endl;
        return false;
    }
}

vector<tuple<int, int, string, int, double>> Database::getAllOrders() {
    vector<tuple<int, int, string, int, double>> orders;
    
    if (!connected) return orders;
    
    try {
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec(
            "SELECT o.order_id, o.dish_id, o.order_date, o.quantity, o.total_price, d.name "
            "FROM orders o JOIN dishes d ON o.dish_id = d.id "
            "ORDER BY o.order_date DESC"
        );
        
        for (const auto& row : result) {
            orders.emplace_back(
                row[0].as<int>(),
                row[1].as<int>(),
                row[2].as<string>(),
                row[3].as<int>(),
                row[4].as<double>()
            );
        }
    }
    catch (const exception& e) {
        cout << "Ошибка получения заказов: " << e.what() << endl;
    }
    
    return orders;
}


vector<pair<string, double>> Database::getRevenueByCategory() {
    vector<pair<string, double>> revenue;
    
    if (!connected) return revenue;
    
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

    if (!connected) return topDishes;

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
    if (!connected) return 0.0;
    
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
    
    if (!connected) return dishCounts;
    
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

    
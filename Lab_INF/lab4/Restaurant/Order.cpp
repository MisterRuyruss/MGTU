#include "Order.h"
#include <iostream>

using namespace std;

bool Order::add(int dishId, int quantity, double totalPrice) {
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

vector<tuple<int, int, string, int, double>> Order::getAll() {
    vector<tuple<int, int, string, int, double>> orders;
    try {
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec(
            "SELECT order_id, dish_id, order_date, quantity, total_price FROM orders ORDER BY order_date DESC"
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
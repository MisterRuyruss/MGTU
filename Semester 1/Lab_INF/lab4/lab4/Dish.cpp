#include "Dish.h"
#include <iostream>

using namespace std;

bool Dish::add(const string& name, int categoryId, double price) {
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

vector<tuple<int, string, int, double>> Dish::getAll() {
    vector<tuple<int, string, int, double>> dishes;
    try {
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec(
            "SELECT d.id, d.name, d.category_id, d.price FROM dishes d ORDER BY d.id"
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
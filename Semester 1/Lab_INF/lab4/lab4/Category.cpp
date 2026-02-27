#include "Category.h"
#include <iostream>

using namespace std;

bool Category::add(const string& name) {
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

vector<pair<int, string>> Category::getAll() {
    vector<pair<int, string>> categories;
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
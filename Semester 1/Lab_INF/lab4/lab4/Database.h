#pragma once
#include <string>
#include <pqxx/pqxx>  
#include <vector>
#include <tuple>
#include <utility>
#include "Category.h"
#include "Dish.h"
#include "Order.h"

using namespace std;

class Database {
private:
    pqxx::connection* conn;
    bool connected;

    Category* categoryObj;
    Dish* dishObj;
    Order* orderObj;

public:
    Database();
    ~Database();

    bool connect();
    void disconnect();
    bool isConnected() const { return connected; }

    bool addCategory(const string& name) {
        return categoryObj ? categoryObj->add(name) : false;
    }
    vector<pair<int, string>> getAllCategories() {
        return categoryObj ? categoryObj->getAll() : vector<pair<int, string>>();
    }

    bool addDish(const string& name, int categoryId, double price, const string& type) {
        return dishObj ? dishObj->add(name, categoryId, price) : false;
    }

    vector<tuple<int, string, int, double>> getAllDishes() {
        return dishObj ? dishObj->getAll() : vector<tuple<int, string, int, double>>();
    }

    bool addOrder(int dishId, int quantity, double totalPrice);
    vector<tuple<int, int, string, int, double>> getAllOrders();

    vector<pair<string, double>> getRevenueByCategory();
    vector<pair<string, int>> getTopSellingDishes(int limit = 3);
    double getAverageOrderValue();
    vector<pair<string, int>> getDishOrderCount();
};
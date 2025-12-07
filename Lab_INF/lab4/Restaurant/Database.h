#pragma once
#include <string>
#include <pqxx/pqxx>  

using namespace std;

class Database {
private:
    pqxx::connection* conn; 
    bool connected;

public:
    Database();
    ~Database();

    bool connect();           
    void disconnect();        
    bool isConnected() const { return connected; }

    bool addCategory(const string& name);
    vector<pair<int, string>> getAllCategories();

    bool addDish(const string& name, int categoryId, double price, const string& type);
    vector<tuple<int, string, int, double>> getAllDishes();

    bool addOrder(int dishId, int quantity, double totalPrice);
    vector<tuple<int, int, string, int, double>> getAllOrders();

    vector<pair<string, double>> getRevenueByCategory();
    vector<pair<string, int>> getTopSellingDishes(int limit = 3);
    double getAverageOrderValue();
    vector<pair<string, int>> getDishOrderCount();

    bool testQuery();  
};
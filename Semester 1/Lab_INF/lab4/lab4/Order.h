#ifndef ORDER_H
#define ORDER_H

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

class Order {
private:
    pqxx::connection* conn;

public:
    Order(pqxx::connection* connection) : conn(connection) {}

    bool add(int dishId, int quantity, double totalPrice);
    vector<tuple<int, int, string, int, double>> getAll();
};

#endif
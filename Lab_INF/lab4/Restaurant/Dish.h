#ifndef DISH_H
#define DISH_H

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

class Dish {
protected:
    pqxx::connection* conn;
    string type;

public:
    Dish(pqxx::connection* connection, const string& dishType = "MainCourse")
        : conn(connection), type(dishType) {
    }

    virtual bool add(const string& name, int categoryId, double price);
    virtual vector<tuple<int, string, int, double>> getAll();

    string getType() const { return type; }
};

class MainCourse : public Dish {
public:
    MainCourse(pqxx::connection* connection) : Dish(connection, "MainCourse") {}
};

class Dessert : public Dish {
public:
    Dessert(pqxx::connection* connection) : Dish(connection, "Dessert") {}
};

#endif
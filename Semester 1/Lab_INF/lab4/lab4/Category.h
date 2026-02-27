
#ifndef CATEGORY_H
#define CATEGORY_H

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class Category {
private:
    pqxx::connection* conn;

public:
    Category(pqxx::connection* connection) : conn(connection) {}

    bool add(const string& name);
    vector<pair<int, string>> getAll();
};

#endif
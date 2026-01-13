#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <libpq-fe.h>
#include <pqxx/pqxx>

using namespace std;

template<typename T>
class DatabaseConnection {
private:
    unique_ptr<pqxx::connection> conn;
    pqxx::work* currentTransaction = nullptr;
    string connectionString;

public:
    explicit DatabaseConnection(const T& connStr);

    ~DatabaseConnection();

    vector<vector<string>> executeQuery(const string& sql);

    void executeNonQuery(const string& sql);
    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();
    void createFunction(const string& sql);
    void createTrigger(const string& sql);
    string getTransactionStatus() const;

    pqxx::connection* getConnection() const;
    void setCurrentUserId(int userId);

private:
    // Вспомогательный метод 
    vector<vector<string>> processResult(const pqxx::result& res);
};

extern template class DatabaseConnection<string>;
extern template class DatabaseConnection<const char*>;

#endif 
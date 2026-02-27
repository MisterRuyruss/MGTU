#include "DatabaseConnection.h"
#include <iostream>
#include <sstream>

using namespace std;


template<typename T>
DatabaseConnection<T>::DatabaseConnection(const T& connStr)
    : connectionString(connStr) {
    try {
        conn = make_unique<pqxx::connection>(connectionString);
        if (!conn->is_open()) {
            throw runtime_error("Не удалось подключиться к базе данных");
        }
        cout << "Подключение к базе данных установлено успешно." << endl;
    }
    catch (const exception& e) {
        cout << "Ошибка подключения: " << e.what() << endl;
        throw;
    }
}

template<typename T>
DatabaseConnection<T>::~DatabaseConnection() {
    if (currentTransaction) {
        try {
            currentTransaction->abort();
            delete currentTransaction;
        }
        catch (...) {
            
        }
    }
    if (conn && conn->is_open()) {
        conn->close();
        cout << "Соединение с базой данных закрыто." << endl;
    }
}

template<typename T>
vector<vector<string>> DatabaseConnection<T>::executeQuery(const string& sql) {
    try {
        if (!conn->is_open()) {
            throw runtime_error("Соединение с базой данных не установлено");
        }

        pqxx::nontransaction ntx(*conn);
        pqxx::result res = ntx.exec(sql);
        return processResult(res);

    }
    catch (const exception& e) {
        cout << "Ошибка выполнения запроса: " << e.what() << endl;
        cout << "SQL: " << sql << endl;
        throw;
    }
}

template<typename T>
void DatabaseConnection<T>::executeNonQuery(const string& sql) {
    try {
        if (!conn->is_open()) {
            throw runtime_error("Соединение с базой данных не установлено");
        }

        if (currentTransaction) {
            currentTransaction->exec(sql);
        }
        else {
            pqxx::work w(*conn);
            w.exec(sql);
            w.commit();
        }

    }
    catch (const exception& e) {
        cout << "Ошибка выполнения запроса: " << e.what() << endl;
        cout << "SQL: " << sql << endl;

        if (currentTransaction) {
            rollbackTransaction();
        }
        throw;
    }
}

template<typename T>
void DatabaseConnection<T>::beginTransaction() {
    if (currentTransaction) {
        throw runtime_error("Транзакция уже начата");
    }
    currentTransaction = new pqxx::work(*conn);
}

template<typename T>
void DatabaseConnection<T>::commitTransaction() {
    if (!currentTransaction) {
        throw runtime_error("Нет активной транзакции");
    }
    try {
        currentTransaction->commit();
        delete currentTransaction;
        currentTransaction = nullptr;
    }
    catch (const exception& e) {
        delete currentTransaction;
        currentTransaction = nullptr;
        throw;
    }
}

template<typename T>
void DatabaseConnection<T>::rollbackTransaction() {
    if (!currentTransaction) {
        throw runtime_error("Нет активной транзакции");
    }
    try {
        currentTransaction->abort();
        delete currentTransaction;
        currentTransaction = nullptr;
    }
    catch (...) {
        delete currentTransaction;
        currentTransaction = nullptr;
    }
}

template<typename T>
void DatabaseConnection<T>::createFunction(const string& sql) {
    executeNonQuery(sql);
}

template<typename T>
void DatabaseConnection<T>::createTrigger(const string& sql) {
    executeNonQuery(sql);
}

template<typename T>
string DatabaseConnection<T>::getTransactionStatus() const {
    if (!currentTransaction) {
        return "Нет активной транзакции";
    }
    return "Транзакция активна";
}

template<typename T>
pqxx::connection* DatabaseConnection<T>::getConnection() const {
    return conn.get();
}

template<typename T>
vector<vector<string>> DatabaseConnection<T>::processResult(const pqxx::result& res) {
    vector<vector<string>> result;

    for (const auto& row : res) {
        vector<string> rowData;
        for (const auto& field : row) {
            rowData.push_back(field.c_str());
        }
        result.push_back(rowData);
    }

    return result;
}
template class DatabaseConnection<string>;
template class DatabaseConnection<const char*>;
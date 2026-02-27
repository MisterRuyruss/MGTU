// DWH_Loader.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <libpq-fe.h>

using namespace std;

struct Product { int id; string name; string category; double price; };
struct Customer { int id; string name; string region; };
struct Sale { int id; string date; int product_id; int customer_id; int quantity; double amount; };


PGconn* connectDB() {
    const char* conninfo = "host=localhost port=5432 dbname=dwh_company user=postgres password=19942007";
    PGconn* conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Ошибка подключения к базе данных: " << PQerrorMessage(conn) << endl;
        return nullptr;
    }
    cout << "Успешное подключение к базе данных!" << endl;
    return conn;
}

vector<Product> loadProducts(const string& filename) {
    vector<Product> products;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не могу открыть файл: " << filename << endl;
        return products;
    }
    string line;
    getline(file, line); 
    while (getline(file, line)) {
        stringstream ss(line);
        Product p;
        string temp;
        getline(ss, temp, ',');
        p.id = stoi(temp);
        getline(ss, p.name, ',');
        getline(ss, p.category, ',');
        getline(ss, temp, ',');
        p.price = stod(temp);
        products.push_back(p);
    }
    file.close();
    cout << "Загружено товаров: " << products.size() << endl;
    return products;
}

vector<Customer> loadCustomers(const string& filename) {
    vector<Customer> customers;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не могу открыть файл: " << filename << endl;
        return customers;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        Customer c;
        string temp;
        getline(ss, temp, ',');
        c.id = stoi(temp);
        getline(ss, c.name, ',');
        getline(ss, c.region, ',');
        customers.push_back(c);
    }
    file.close();
    cout << "Загружено клиентов: " << customers.size() << endl;
    return customers;
}

vector<Sale> loadSales(const string& filename) {
    vector<Sale> sales;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не могу открыть файл: " << filename << endl;
        return sales;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        Sale s;
        string temp;
        getline(ss, temp, ',');
        s.id = stoi(temp);
        getline(ss, s.date, ',');
        getline(ss, temp, ',');
        s.product_id = stoi(temp);
        getline(ss, temp, ',');
        s.customer_id = stoi(temp);
        getline(ss, temp, ',');
        s.quantity = stoi(temp);
        getline(ss, temp, ',');
        s.amount = stod(temp);
        sales.push_back(s);
    }
    file.close();
    cout << "Загружено продаж: " << sales.size() << endl;
    return sales;
}

void insertData(PGconn* conn, const string& table, const vector<string>& values) {
    for (const auto& val : values) {
        string sql = "INSERT INTO " + table + " VALUES " + val + " ON CONFLICT DO NOTHING;";
        PGresult* res = PQexec(conn, sql.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            cerr << "Ошибка вставки: " << PQerrorMessage(conn) << endl;
        }
        PQclear(res);
    }
}

int main() {

    cout << "=== ЗАГРУЗКА ДАННЫХ В DWH ===" << endl;
    PGconn* conn = connectDB();
    if (!conn) return 1;

    auto products = loadProducts("D:/MGTU/Task_INF/Homework2/products.csv");
    auto customers = loadCustomers("D:/MGTU/Task_INF/Homework2/customers.csv");
    auto sales = loadSales("D:/MGTU/Task_INF/Homework2/sales.csv");

    vector<string> productValues;
    for (const auto& p : products) {
        productValues.push_back("(" + to_string(p.id) + ", '" + p.name + "', '" + p.category + "', " + to_string(p.price) + ")");
    }

    vector<string> customerValues;
    for (const auto& c : customers) {
        customerValues.push_back("(" + to_string(c.id) + ", '" + c.name + "', '" + c.region + "')");
    }

    vector<string> saleValues;
    for (const auto& s : sales) {
        saleValues.push_back("(" + to_string(s.id) + ", '" + s.date + "', " + to_string(s.product_id) + ", " +
            to_string(s.customer_id) + ", " + to_string(s.quantity) + ", " + to_string(s.amount) + ")");
    }

    cout << endl;
    cout << "Загрузка в БД..." << endl;
    insertData(conn, "products_dim", productValues);
    insertData(conn, "customers_dim", customerValues);
    insertData(conn, "sales_fact", saleValues);

    string sqlTime = R"(
        INSERT INTO time_dim (date, year, quarter, month, day, day_of_week)
        SELECT DISTINCT sale_date, 
               EXTRACT(YEAR FROM sale_date),
               EXTRACT(QUARTER FROM sale_date),
               EXTRACT(MONTH FROM sale_date),
               EXTRACT(DAY FROM sale_date),
               TO_CHAR(sale_date, 'Day')
        FROM sales_fact
        ON CONFLICT (date) DO NOTHING;
    )";
    PQexec(conn, sqlTime.c_str());
    cout << "Таблица времени заполнена" << endl << endl;
    
    cout << "=== РЕЗУЛЬТАТЫ ===" << endl;

    string sql1 = R"(
        SELECT p.category, SUM(s.amount) 
        FROM sales_fact s 
        JOIN products_dim p ON s.product_id = p.product_id 
        GROUP BY p.category;
    )";
    PGresult* res1 = PQexec(conn, sql1.c_str());
    if (PQresultStatus(res1) == PGRES_TUPLES_OK) {
        cout << endl;
        cout << "1. Продажи по категориям:" << endl;
        for (int i = 0; i < PQntuples(res1); i++) {
            cout << PQgetvalue(res1, i, 0) << ": " << PQgetvalue(res1, i, 1) << " руб" << endl;
        }
    }
    PQclear(res1);

    string sql2 = R"(
        SELECT c.region, COUNT(*) 
        FROM sales_fact s 
        JOIN customers_dim c ON s.customer_id = c.customer_id 
        GROUP BY c.region;
    )";
    PGresult* res2 = PQexec(conn, sql2.c_str());
    if (PQresultStatus(res2) == PGRES_TUPLES_OK) {
        cout << endl;
        cout << "2. Продажи по регионам:" << endl;
        for (int i = 0; i < PQntuples(res2); i++) {
            cout << PQgetvalue(res2, i, 0) << ": " << PQgetvalue(res2, i, 1) << " продаж" << endl;
        }
    }
    PQclear(res2);

    PQfinish(conn);
    cout << endl;
    cout << "Все данные были успешно обработаны!" << endl;

    return 0;
}
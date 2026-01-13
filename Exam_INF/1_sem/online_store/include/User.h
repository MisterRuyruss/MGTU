#ifndef USER_H
#define USER_H

#include <memory>
#include <vector>
#include <string>
#include "DatabaseConnection.h"

using namespace std;

class Order;
class Product;

class User : public enable_shared_from_this<User> {
protected:
    int userId;
    string name;
    string email;
    string role;
    vector<shared_ptr<Order>> orders;
    shared_ptr<DatabaseConnection<string>> db;  

public:
    User(int id, string name, string email, string role,
        shared_ptr<DatabaseConnection<string>> db);
    virtual ~User() = default;

    virtual void displayMenu() = 0;
    virtual bool canPerformAction(const string& action) = 0;

    virtual void viewOrderStatus(int orderId);
    virtual bool cancelOrder(int orderId);
    virtual void viewMyOrders();

    int getUserId() const { return userId; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getRole() const { return role; }

    void setDatabase(shared_ptr<DatabaseConnection<string>> database);

    void addOrder(shared_ptr<Order> order);
    vector<shared_ptr<Order>> getOrders() const;
    shared_ptr<Order> getOrderById(int orderId) const;

    void displayInfo() const;
};

#endif 
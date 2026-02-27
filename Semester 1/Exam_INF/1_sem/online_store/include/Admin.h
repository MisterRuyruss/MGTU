#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <memory>

using namespace std;

class Admin : public User {
public:
    Admin(int id, string name, string email,
        shared_ptr<DatabaseConnection<string>> db);

    void displayMenu() override;
    bool canPerformAction(const string& action) override;

    // Методы администратора с реальной работой с БД
    bool addProduct(const string& name, double price, int quantity);
    bool updateProduct(int productId, double newPrice, int newQuantity);
    bool deleteProduct(int productId);
    void viewAllOrders();
    bool updateOrderStatus(int orderId, const string& newStatus);
    void viewOrderStatusHistory(int orderId);
    void viewAuditLog();
    bool generateCSVReport();
    bool viewOrderDetails(int orderId);

private:
    void logAction(const string& operation, const string& details);
};

#endif 
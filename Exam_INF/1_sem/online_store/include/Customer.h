#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <memory>

using namespace std;

class Product;
class PaymentStrategy;

class Customer : public User {
public:
    Customer(int id, string name, string email,
        shared_ptr<DatabaseConnection<string>> db);

    void displayMenu() override;
    bool canPerformAction(const string& action) override;

    // Методы покупателя
    shared_ptr<Order> createOrder();
    bool addToOrder(int orderId, int productId, int quantity);
    bool removeFromOrder(int orderId, int productId);
    void viewMyOrders() override;
    bool makePayment(int orderId, const string& paymentMethod,
        const string& paymentDetails);
    bool returnOrder(int orderId);
    void viewOrderStatusHistory(int orderId);

    // Работа с продуктами
    vector<shared_ptr<Product>> getAvailableProducts();

private:
    bool checkProductAvailability(int productId, int quantity);
    void logAction(const string& operation, const string& details);
};

#endif 
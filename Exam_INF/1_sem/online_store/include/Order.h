#ifndef ORDER_H
#define ORDER_H

#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include "OrderItem.h"
#include "Payment.h"

using namespace std;

class User; 

class Order {
private:
    int orderId;
    weak_ptr<User> user; 
    string status;
    double totalPrice;
    chrono::system_clock::time_point orderDate;

    vector<unique_ptr<OrderItem>> items;

    unique_ptr<Payment> payment;

public:
    Order(int id, shared_ptr<User> user);

    int getOrderId() const { return orderId; }
    string getStatus() const { return status; }
    double getTotalPrice() const { return totalPrice; }
    string getOrderDate() const;
    shared_ptr<User> getUser() const;

    void addItem(shared_ptr<Product> product, int quantity);
    void removeItem(int productId);
    void updateItemQuantity(int productId, int newQuantity);
    const vector<unique_ptr<OrderItem>>& getItems() const { return items; }

    void setStatus(const string& newStatus);
    bool canBeReturned() const;

    void setPayment(unique_ptr<Payment> payment);
    bool processPayment();
    string getPaymentStatus() const;

    void recalculateTotal();

    string toString() const;
};

#endif 
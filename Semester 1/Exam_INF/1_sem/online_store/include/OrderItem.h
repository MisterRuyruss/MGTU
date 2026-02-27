#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <memory>
#include <string>

using namespace std;

class Product;

class OrderItem {
private:
    int orderItemId;
    shared_ptr<Product> product;
    int quantity;
    double priceAtTime;

public:
    OrderItem(int id, shared_ptr<Product> product, int quantity);

    int getOrderItemId() const { return orderItemId; }
    shared_ptr<Product> getProduct() const;
    int getQuantity() const { return quantity; }
    double getPriceAtTime() const { return priceAtTime; }
    double getTotalPrice() const { return priceAtTime * quantity; }

    void updateQuantity(int newQuantity);
    string toString() const;
};

#endif 
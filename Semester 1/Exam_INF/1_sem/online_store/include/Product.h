#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <memory>

using namespace std;

class Product {
private:
    int productId;
    string name;
    double price;
    int stockQuantity;

public:
    Product(int id, string name, double price, int quantity);

    int getProductId() const { return productId; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStockQuantity() const { return stockQuantity; }

    void setPrice(double newPrice);
    void setStockQuantity(int newQuantity);

    void updateStock(int delta); 
    bool isAvailable(int requested = 1) const;

    string toString() const;
};

#endif 
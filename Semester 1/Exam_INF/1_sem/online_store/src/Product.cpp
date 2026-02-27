#include "Product.h"
#include <sstream>
#include <stdexcept>

using namespace std;

Product::Product(int id, string name, double price, int quantity)
    : productId(id), name(move(name)), price(price), stockQuantity(quantity) {
    if (price <= 0) {
        throw invalid_argument("Цена должна быть положительной");
    }
    if (quantity < 0) {
        throw invalid_argument("Количество на складе не может быть отрицательным");
    }
}

void Product::setPrice(double newPrice) {
    if (newPrice <= 0) {
        throw invalid_argument("Цена должна быть положительной");
    }
    price = newPrice;
}

void Product::setStockQuantity(int newQuantity) {
    if (newQuantity < 0) {
        throw invalid_argument("Количество на складе не может быть отрицательным");
    }
    stockQuantity = newQuantity;
}

void Product::updateStock(int delta) {
    if (stockQuantity + delta < 0) {
        throw runtime_error("Недостаточно товара на складе");
    }
    stockQuantity += delta;
}

bool Product::isAvailable(int requested) const {
    return stockQuantity >= requested && requested > 0;
}

string Product::toString() const {
    ostringstream oss;
    oss << "Товар ID: " << productId
        << ", Название: " << name
        << ", Цена: " << price << " руб."
        << ", На складе: " << stockQuantity << " шт.";
    return oss.str();
}
#include "OrderItem.h"
#include "Product.h"
#include <sstream>
#include <stdexcept>

using namespace std;

OrderItem::OrderItem(int id, shared_ptr<Product> product, int quantity)
    : orderItemId(id), product(move(product)), quantity(quantity) {
    if (!this->product) {
        throw invalid_argument("Товар не может быть пустым");
    }
    if (quantity <= 0) {
        throw invalid_argument("Количество должно быть положительным");
    }
    priceAtTime = this->product->getPrice();
}

shared_ptr<Product> OrderItem::getProduct() const {
    return product;
}

void OrderItem::updateQuantity(int newQuantity) {
    if (newQuantity <= 0) {
        throw invalid_argument("Количество должно быть положительным");
    }
    quantity = newQuantity;
}

string OrderItem::toString() const {
    ostringstream oss;
    if (product) {
        oss << "Позиция заказа ID: " << orderItemId
            << ", Товар: " << product->getName()
            << " (ID: " << product->getProductId() << ")"
            << ", Количество: " << quantity << " шт."
            << ", Цена за шт.: " << priceAtTime << " руб."
            << ", Итого: " << getTotalPrice() << " руб.";
    }
    else {
        oss << "Позиция заказа ID: " << orderItemId << " (товар отсутствует)";
    }
    return oss.str();
}
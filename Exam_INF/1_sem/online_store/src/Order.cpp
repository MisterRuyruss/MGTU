#define _CRT_SECURE_NO_WARNINGS

#include "Order.h"
#include "User.h"
#include "Product.h"         
#include "OrderItem.h"         
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

// из времени в строку
string timeToString(const chrono::system_clock::time_point& tp) {
    auto time = chrono::system_clock::to_time_t(tp);
    stringstream ss;
    ss << put_time(localtime(&time), "%d.%m.%Y %H:%M:%S");
    return ss.str();
}

Order::Order(int id, shared_ptr<User> user)
    : orderId(id), user(user), status("ожидается"), totalPrice(0) {
    orderDate = chrono::system_clock::now();
}


string Order::getOrderDate() const {
    return timeToString(orderDate);
}

// Получение 
shared_ptr<User> Order::getUser() const {
    return user.lock();
}


void Order::addItem(shared_ptr<Product> product, int quantity) {
    if (!product) {
        throw invalid_argument("Товар не может быть пустым");
    }
    if (quantity <= 0) {
        throw invalid_argument("Количество должно быть положительным");
    }
    if (status != "ожидается") {
        throw runtime_error("Нельзя изменить заказ в статусе: " + status);
    }

    // есть ли уже такой товар в заказе
    auto it = find_if(items.begin(), items.end(),
        [&product](const unique_ptr<OrderItem>& item) {
            return item->getProduct()->getProductId() == product->getProductId();
        });

    if (it != items.end()) {
        // обновляем количество существующей позиции
        (*it)->updateQuantity((*it)->getQuantity() + quantity);
    }
    else {
        int newItemId = items.empty() ? 1 : items.back()->getOrderItemId() + 1;
        items.push_back(make_unique<OrderItem>(newItemId, product, quantity));
    }

    recalculateTotal();
}

void Order::removeItem(int productId) {
    if (status != "ожидается") {
        throw runtime_error("Нельзя изменить заказ в статусе: " + status);
    }

    auto it = remove_if(items.begin(), items.end(),
        [productId](const unique_ptr<OrderItem>& item) -> bool {
            if (auto p = item->getProduct()) {
                return p->getProductId() == productId;
            }
            return false;
        });

    if (it != items.end()) {
        items.erase(it, items.end());
        recalculateTotal();
    }
}

void Order::updateItemQuantity(int productId, int newQuantity) {
    if (status != "ожидается") {
        throw runtime_error("Нельзя изменить заказ в статусе: " + status);
    }

    for (auto& item : items) {
        if (item->getProduct()->getProductId() == productId) {
            item->updateQuantity(newQuantity);
            recalculateTotal();
            return;
        }
    }
    throw runtime_error("Товар с ID " + to_string(productId) + " не найден в заказе");
}

void Order::setStatus(const string& newStatus) {
    if (newStatus != "ожидается" && newStatus != "выполнен" &&
        newStatus != "отменён" && newStatus != "возвращён") {
        throw invalid_argument("Неверный статус заказа");
    }

    // Проверка на возврат
    if (newStatus == "возвращён" && !canBeReturned()) {
        throw runtime_error("Заказ не может быть возвращён");
    }

    status = newStatus;
}

// проверка возможности возврата
bool Order::canBeReturned() const {
    if (status != "выполнен") {
        return false;
    }

    auto now = chrono::system_clock::now();
    auto days = chrono::duration_cast<chrono::hours>(now - orderDate).count() / 24;

    return days <= 30;
}

// Установка платежа
void Order::setPayment(unique_ptr<Payment> payment) {
    this->payment = move(payment);
}

// Обработка платежа
bool Order::processPayment() {
    if (!payment) {
        throw runtime_error("Платеж не настроен");
    }
    return payment->processPayment();
}

// получение статуса платежа
string Order::getPaymentStatus() const {
    return payment ? payment->getStatus() : "платеж не создан";
}

// пересчет общей стоимости
void Order::recalculateTotal() {
    totalPrice = 0;
    for (const auto& item : items) {
        totalPrice += item->getTotalPrice();
    }
}

string Order::toString() const {
    ostringstream oss;
    oss << "Заказ ID: " << orderId << endl
        << "Пользователь: " << (user.lock() ? user.lock()->getName() : "неизвестен") << endl
        << "Статус: " << status << endl
        << "Общая стоимость: " << totalPrice << " руб." << endl
        << "Дата заказа: " << getOrderDate() << endl
        << "Статус оплаты: " << getPaymentStatus() << endl
        << "Количество позиций: " << items.size() << endl;

    if (!items.empty()) {
        oss << "Состав заказа:" << endl;
        for (const auto& item : items) {
            oss << "  - " << item->toString() << endl;
        }
    }

    if (payment) {
        oss << "Информация о платеже:" << endl
            << "  " << payment->toString() << endl;
    }

    return oss.str();
}
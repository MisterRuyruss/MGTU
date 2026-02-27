#define _CRT_SECURE_NO_WARNINGS
#include "Payment.h"
#include <iomanip>
#include <sstream>

using namespace std;

Payment::Payment(int id, double amount, unique_ptr<PaymentStrategy> strategy)
    : paymentId(id), amount(amount), strategy(move(strategy)) {
    status = "ожидается";
    paymentDate = chrono::system_clock::now();
}

string Payment::getPaymentDate() const {
    auto time = chrono::system_clock::to_time_t(paymentDate);
    stringstream ss;
    ss << put_time(localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool Payment::processPayment() {
    if (!strategy) {
        status = "ошибка";
        return false;
    }

    bool success = strategy->pay(amount);
    status = success ? "оплачено" : "ошибка";
    return success;
}

void Payment::setStrategy(unique_ptr<PaymentStrategy> newStrategy) {
    strategy = move(newStrategy);
}

string Payment::getPaymentMethod() const {
    return strategy ? strategy->getMethodName() : "Метод не выбран";
}

string Payment::toString() const {
    ostringstream oss;
    oss << "Платеж ID: " << paymentId
        << ", Сумма: " << amount << " руб."
        << ", Статус: " << status
        << ", Способ: " << getPaymentMethod()
        << ", Дата: " << getPaymentDate();
    if (strategy) {
        oss << ", Детали: " << strategy->getDetails();
    }
    return oss.str();
}
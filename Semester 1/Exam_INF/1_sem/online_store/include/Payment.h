#ifndef PAYMENT_H
#define PAYMENT_H

#include <memory>
#include <string>
#include <chrono>
#include "PaymentStrategy.h"

using namespace std;

class Payment {
private:
    int paymentId;
    double amount;
    string status; 
    chrono::system_clock::time_point paymentDate;
    unique_ptr<PaymentStrategy> strategy; 

public:
    Payment(int id, double amount, unique_ptr<PaymentStrategy> strategy);

    int getPaymentId() const { return paymentId; }
    double getAmount() const { return amount; }
    string getStatus() const { return status; }
    string getPaymentDate() const;

    bool processPayment();
    void setStrategy(unique_ptr<PaymentStrategy> newStrategy);
    string getPaymentMethod() const;
    string toString() const;
};

#endif 
#ifndef PAYMENTSTRATEGY_H
#define PAYMENTSTRATEGY_H

#include <string>
#include <memory>

using namespace std;

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount) = 0; 
    virtual string getMethodName() const = 0;
    virtual string getDetails() const = 0;
};

class CreditCardPayment : public PaymentStrategy {
private:
    string cardNumber;
    string expiryDate;
    string cvv;
public:
    CreditCardPayment(string card, string expiry, string cvv);
    bool pay(double amount) override;
    string getMethodName() const override; 
    string getDetails() const override;
};

class DigitalWalletPayment : public PaymentStrategy {
private:
    string walletId;
    string walletType; 
public:
    DigitalWalletPayment(string id, string type);
    bool pay(double amount) override;
    string getMethodName() const override; 
    string getDetails() const override;
};

class SBPPayment : public PaymentStrategy {
private:
    string phoneNumber;
    string bankName;
public:
    SBPPayment(string phone, string bank);
    bool pay(double amount) override;
    string getMethodName() const override; 
    string getDetails() const override;
};

#endif 
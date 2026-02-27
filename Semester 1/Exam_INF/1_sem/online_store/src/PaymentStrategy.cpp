#include "PaymentStrategy.h"
#include <iostream>
#include <sstream>

using namespace std;

CreditCardPayment::CreditCardPayment(string card, string expiry, string cvv)
    : cardNumber(move(card)), expiryDate(move(expiry)), cvv(move(cvv)) {
}

bool CreditCardPayment::pay(double amount) {
    cout << "Обработка оплаты банковской картой на сумму " << amount << " руб..." << endl;
    
    cout << "Оплата успешно проведена картой, оканчивающейся на "
        << cardNumber.substr(cardNumber.length() - 4) << endl;
    return true;
}

string CreditCardPayment::getDetails() const {
    return "Карта, оканчивающаяся на " + cardNumber.substr(cardNumber.length() - 4) +
        ", Действует до: " + expiryDate;
}

string CreditCardPayment::getMethodName() const {
    return "Банковская карта";
}

DigitalWalletPayment::DigitalWalletPayment(string id, string type)
    : walletId(move(id)), walletType(move(type)) {
}

bool DigitalWalletPayment::pay(double amount) {
    cout << "Обработка оплаты через " << walletType << " кошелёк на сумму "
        << amount << " руб..." << endl;
    cout << "ID кошелька: " << walletId << endl;
    cout << "Оплата успешно проведена через " << walletType << " кошелёк." << endl;
    return true;
}

string DigitalWalletPayment::getDetails() const {
    return walletType + " кошелёк, ID: " + walletId;
}

string DigitalWalletPayment::getMethodName() const {
    return "Электронный кошелёк";
}

SBPPayment::SBPPayment(string phone, string bank)
    : phoneNumber(move(phone)), bankName(move(bank)) {
}

bool SBPPayment::pay(double amount) {
    cout << "Обработка оплаты через СБП на сумму " << amount << " руб..." << endl;
    cout << "Телефон: " << phoneNumber << ", Банк: " << bankName << endl;
    cout << "Оплата успешно проведена через СБП." << endl;
    return true;
}

string SBPPayment::getDetails() const {
    return "СБП, Телефон: " + phoneNumber + ", Банк: " + bankName;
}

string SBPPayment::getMethodName() const {
    return "Система быстрых платежей (СБП)";
}
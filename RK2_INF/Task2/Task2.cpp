// Task2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>

using namespace std;

class BankAccount {
private:
    string accountNumber;
    string ownerName;
    double balance;

public:
    BankAccount(string accNum, string owner, double initialBalance) {
        accountNumber = accNum;
        ownerName = owner;
        if (initialBalance >= 0) {
            balance = initialBalance;
        }
        else {
            balance = 0;
            cout << "Начальный баланс не может быть отрицательным. Установлен в 0!" << endl;
        }
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Пополнение на " << amount << " успешно. Новый баланс: " << balance << endl;
        }
        else {
            cout << "Сумма пополнения должна быть положительной!" << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Снятие " << amount << " успешно. Новый баланс: " << balance << endl;
        }
        else if (amount > balance) {
            cout << "Недостаточно средств на счете!" << endl;
        }
        else {
            cout << "Сумма снятия должна быть положительной!" << endl;
        }
    }

    void display() const {
        cout << "Номер счета: " << accountNumber << endl;
        cout << "Владелец: " << ownerName << endl;
        cout << "Баланс: " << balance << endl;
    }

    double getBalance() const {
        return balance;
    }
};

class SavingsAccount : public BankAccount {
private:
    double ctavka; 

public:
    SavingsAccount(string accNum, string owner, double initialBalance, double rate)
        : BankAccount(accNum, owner, initialBalance) {
        ctavka = rate;
    }

    void addInterest() {
        double interest = getBalance() * (ctavka / 100);
        deposit(interest); 
        cout << "Начислены проценты: " << interest << endl;
    }
};

int main() {
    cout << "=== ООО Банк <ИнформатикаБанк> ===" << endl;
    BankAccount acc1("111264", "Артём Низамиев", 100000);
    acc1.display();
    acc1.deposit(500);
    acc1.withdraw(200);
    acc1.withdraw(2000); 
    acc1.display();
    cout << endl;
    cout << "=== Накопительный счёт ===" << endl;
    SavingsAccount acc2("321111", "Артём Низамиев", 200000, 12.0); 
    acc2.display();
    acc2.deposit(1000);
    acc2.addInterest(); 
    acc2.display();

    return 0;
}
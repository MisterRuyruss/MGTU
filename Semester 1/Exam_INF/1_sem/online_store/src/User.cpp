#include "User.h"
#include "Order.h"
#include <iostream>
#include <algorithm>

using namespace std;

User::User(int id, string name, string email, string role,
    shared_ptr<DatabaseConnection<string>> db)
    : userId(id), name(move(name)), email(move(email)),
    role(move(role)), db(db) {  
}

void User::viewOrderStatus(int orderId) {
    auto it = find_if(orders.begin(), orders.end(),
        [orderId](const shared_ptr<Order>& order) {
            return order->getOrderId() == orderId;
        });

    if (it != orders.end()) {
        cout << "Статус заказа ID " << orderId << ": " << (*it)->getStatus() << endl;
    }
    else {
        cout << "Заказ с ID " << orderId << " не найден." << endl;
    }
}

bool User::cancelOrder(int orderId) {
    auto it = find_if(orders.begin(), orders.end(),
        [orderId](const shared_ptr<Order>& order) -> bool {
            if (order) {
                return order->getOrderId() == orderId;
            }
            return false;
        });

    if (it != orders.end()) {
        try {
            (*it)->setStatus("отменён");
            cout << "Заказ ID " << orderId << " отменён." << endl;
            return true;  
        }
        catch (const exception& e) {
            cerr << "Ошибка при отмене заказа: " << e.what() << endl;
            return false;  
        }
    }
    else {
        cout << "Заказ с ID " << orderId << " не найден." << endl;
        return false; 
    }
}

void User::addOrder(shared_ptr<Order> order) {
    orders.push_back(order);
}

vector<shared_ptr<Order>> User::getOrders() const {
    return orders;
}

void User::displayInfo() const {
    cout << "Пользователь ID: " << userId << endl
        << "Имя: " << name << endl
        << "Email: " << email << endl
        << "Роль: " << role << endl
        << "Количество заказов: " << orders.size() << endl;
}

void User::viewMyOrders() {
    cout << "Заказы пользователя " << name << ":" << endl;
    for (const auto& order : orders) {
        if (order) {
            cout << "  Заказ ID: " << order->getOrderId()
                << ", Статус: " << order->getStatus() << endl;
        }
    }
}
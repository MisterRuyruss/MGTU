#ifndef MANAGER_H
#define MANAGER_H

#include "User.h"

using namespace std;

class Manager : public User {
public:
    Manager(int id, string name, string email,
        shared_ptr<DatabaseConnection<string>> db);

    void displayMenu() override;
    bool canPerformAction(const string& action) override;

    // Методы менеджера
    bool approveOrder(int orderId);
    bool updateStock(int productId, int newQuantity);
    void viewPendingOrders();
    void viewMyApprovedOrders();
    bool viewOrderDetails(int orderId);

private:
    void logAction(const string& operation, const string& details);
};

#endif 
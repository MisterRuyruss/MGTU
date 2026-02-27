#include "Customer.h"
#include "Order.h"
#include "Product.h"
#include "Payment.h"
#include "PaymentStrategy.h"
#include "DatabaseConnection.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>

using namespace std;

Customer::Customer(int id, string name, string email,
    shared_ptr<DatabaseConnection<string>> db)
    : User(id, move(name), move(email), "покупатель", db) {
}

void Customer::displayMenu() {
    cout << endl;
    cout << "=== МЕНЮ ПОКУПАТЕЛЯ ===" << endl;
    cout << "1. Создать новый заказ" << endl;
    cout << "2. Добавить товар в заказ" << endl;
    cout << "3. Удалить товар из заказа" << endl;
    cout << "4. Просмотр моих заказов" << endl;
    cout << "5. Просмотр статуса заказа" << endl;
    cout << "6. Оплатить заказ" << endl;
    cout << "7. Оформить возврат заказа" << endl;
    cout << "8. Просмотр истории статусов заказа" << endl;
    cout << "9. Просмотр доступных товаров" << endl;
    cout << "10. Выход" << endl;
    cout << "Выберите действие: ";
}

bool Customer::canPerformAction(const string& action) {
    if (action == "создать_заказ" || action == "оплатить_заказ" ||
        action == "вернуть_заказ" || action == "просмотреть_свои_заказы") {
        cout << "Доступ покупателя подтверждён для: " << action << endl;
        return true;
    }
    cout << "Доступ запрещён для : " << action << endl;
    return false;
}

shared_ptr<Order> Customer::createOrder() {
    try {
        // Хранимая процедура с пустыми данными для создания заказа
        stringstream sql;
        sql << "INSERT INTO orders (user_id, status, total_price) "
            << "VALUES (" << userId << ", 'pending', 0) RETURNING order_id;";

        db->executeNonQuery(sql.str());

        stringstream insertSql;
        insertSql << "INSERT INTO orders (user_id, status, total_price) "
            << "VALUES (" << userId << ", 'pending', 0) RETURNING order_id;";

        auto result = db->executeQuery(insertSql.str());

        if (!result.empty() && !result[0].empty()) {
            int orderId = stoi(result[0][0]);
            auto order = make_shared<Order>(orderId, shared_from_this());
            orders.push_back(order);

            logAction("create_order", "Создан заказ ID: " + to_string(orderId));

            cout << "Создан новый заказ ID " << orderId << endl;
            return order;
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при создании заказа: " << e.what() << endl;
    }
    return nullptr;
}

bool Customer::addToOrder(int orderId, int productId, int quantity) {
    try {
        if (!checkProductAvailability(productId, quantity)) {
            cerr << "Товар недоступен в нужном количестве." << endl;
            return false;
        }

        auto result = db->executeQuery(
            "SELECT user_id FROM orders WHERE order_id = " + to_string(orderId) + ";"
        );

        if (result.empty() || stoi(result[0][0]) != userId) {
            cerr << "Заказ не принадлежит вам или не существует." << endl;
            return false;
        }

        // Добавляем товар в заказ через транзакцию
        
        try {
            // Получаем цену товара
            auto priceResult = db->executeQuery(
                "SELECT price FROM products WHERE product_id = " + to_string(productId) + ";"
            );

            if (priceResult.empty()) {
                throw runtime_error("Товар не найден");
            }
            double price = stod(priceResult[0][0]);

            // Добавляем элемент заказа
            stringstream sql;
            sql << "INSERT INTO order_items (order_id, product_id, quantity, price) VALUES ("
                << orderId << ", " << productId << ", " << quantity << ", " << price << ");";

            db->executeNonQuery(sql.str());

            // Обновляем общую стоимость заказа
            db->executeNonQuery(
                "UPDATE orders SET total_price = ("
                "SELECT SUM(quantity * price) FROM order_items WHERE order_id = " +
                to_string(orderId) + ") WHERE order_id = " + to_string(orderId) + ";"
            );


            logAction("add_product", "Заказ ID: " + to_string(orderId) +
                ", Товар ID: " + to_string(productId));

            cout << "Товар добавлен в заказ." << endl;
            return true;
        }
        catch (...) {
            
            throw;
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при добавлении товара: " << e.what() << endl;
        return false;
    }
}

bool Customer::removeFromOrder(int orderId, int productId) {
    try {
        // Проверяем принадлежность заказа
        auto result = db->executeQuery(
            "SELECT user_id FROM orders WHERE order_id = " + to_string(orderId) + ";"
        );

        if (result.empty() || stoi(result[0][0]) != userId) {
            cerr << "Заказ не принадлежит вам или не существует." << endl;
            return false;
        }

        stringstream sql;
        sql << "DELETE FROM order_items WHERE order_id = " << orderId
            << " AND product_id = " << productId << ";";

        db->executeNonQuery(sql.str());

        db->executeNonQuery(
            "UPDATE orders SET total_price = COALESCE(("
            "SELECT SUM(quantity * price) FROM order_items WHERE order_id = " +
            to_string(orderId) + "), 0) WHERE order_id = " + to_string(orderId) + ";"
        );

        logAction("delete_product", "Заказ ID: " + to_string(orderId) +
            ", Товар ID: " + to_string(productId));

        cout << "Товар удалён из заказа." << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при удалении товара: " << e.what() << endl;
        return false;
    }
}

void Customer::viewMyOrders() {
    try {
        stringstream sql;
        sql << "SELECT o.order_id, o.status, o.total_price, "
            << "TO_CHAR(o.order_date, 'DD.MM.YYYY HH24:MI:SS') as order_date, "
            << "COUNT(oi.order_item_id) as items_count "
            << "FROM orders o "
            << "LEFT JOIN order_items oi ON o.order_id = oi.order_id "
            << "WHERE o.user_id = " << userId << " "
            << "GROUP BY o.order_id, o.status, o.total_price, o.order_date "
            << "ORDER BY o.order_date DESC;";

        auto results = db->executeQuery(sql.str());
        cout << endl;
        cout << "=== МОИ ЗАКАЗЫ ===" << endl;
        if (results.empty()) {
            cout << "У вас нет заказов." << endl;
            return;
        }

        cout << left
            << setw(20) << "ID заказа"
            << setw(23) << "Статус"
            << setw(23) << "Сумма"
            << setw(19) << "Позиций"
            << setw(28) << "Дата заказа" << endl;
        cout << string(85, '-') << endl;

        for (const auto& row : results) {
            if (row.size() >= 5) {
                cout << left
                    << setw(14) << row[0]
                    << setw(17) << row[1]
                    << setw(24) << row[2] + " руб"
                    << setw(9) << row[4]
                    << setw(24) << row[3] << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при получении заказов: " << e.what() << endl;
    }
}

bool Customer::makePayment(int orderId, const string& paymentMethod,
    const string& paymentDetails) {
    try {
        // Проверяем принадлежность и статус заказа
        auto result = db->executeQuery(
            "SELECT user_id, status, total_price FROM orders WHERE order_id = " +
            to_string(orderId) + ";"
        );

        if (result.empty()) {
            cerr << "Заказ не найден." << endl;
            return false;
        }

        if (stoi(result[0][0]) != userId) {
            cerr << "Заказ не принадлежит вам." << endl;
            return false;
        }

        string status = result[0][1];
        double totalPrice = stod(result[0][2]);

        if (status != "pending") {
            cerr << "Заказ уже оплачен или отменён." << endl;
            return false;
        }

        unique_ptr<PaymentStrategy> strategy;
        if (paymentMethod == "карта") {
            // номер_карты срок CVV
            size_t pos1 = paymentDetails.find(',');
            size_t pos2 = paymentDetails.find(',', pos1 + 1);
            if (pos1 == string::npos || pos2 == string::npos) {
                cerr << "Неверный формат данных карты." << endl;
                return false;
            }
            string card = paymentDetails.substr(0, pos1);
            string expiry = paymentDetails.substr(pos1 + 1, pos2 - pos1 - 1);
            string cvv = paymentDetails.substr(pos2 + 1);
            strategy = make_unique<CreditCardPayment>(card, expiry, cvv);
        }
        else if (paymentMethod == "кошелёк") {
            strategy = make_unique<DigitalWalletPayment>(paymentDetails, "YooMoney");
        }
        else if (paymentMethod == "сбп") {
            strategy = make_unique<SBPPayment>(paymentDetails, "Сбербанк");
        }
        else {
            cerr << "Неизвестный способ оплаты." << endl;
            return false;
        }

        // Создаём и обрабатываем платеж
        auto payment = make_unique<Payment>(orderId, totalPrice, move(strategy));
        if (payment->processPayment()) {
            db->executeNonQuery("SET app.current_user_id = '" + to_string(userId) + "'");
            db->executeNonQuery(
                "UPDATE orders SET status = 'completed' WHERE order_id = " +
                to_string(orderId) + ";"
            );

            logAction("make_payment", "Заказ ID: " + to_string(orderId) +
                ", Способ: " + paymentMethod);

            cout << "Оплата прошла успешно. Заказ выполнен." << endl;
            return true;
        }
        else {
            cout << "Ошибка оплаты." << endl;
            return false;
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при оплате: " << e.what() << endl;
        return false;
    }
}

bool Customer::returnOrder(int orderId) {
    try {
        // Проверяем возможность возврата через функцию БД
        auto result = db->executeQuery(
            "SELECT canReturnOrder(" + to_string(orderId) + ");"
        );

        if (result.empty() || result[0][0] != "t") {
            cerr << "Заказ не может быть возвращён." << endl;
            return false;
        }

        // Проверяем принадлежность заказа
        auto checkResult = db->executeQuery(
            "SELECT user_id FROM orders WHERE order_id = " + to_string(orderId) + ";"
        );

        if (checkResult.empty() || stoi(checkResult[0][0]) != userId) {
            cerr << "Заказ не принадлежит вам." << endl;
            return false;
        }

        // Обновляем статус
        db->executeNonQuery("SET app.current_user_id = '" + to_string(userId) + "'");
        db->executeNonQuery(
            "UPDATE orders SET status = 'returned' WHERE order_id = " +
            to_string(orderId) + ";"
        );

        logAction("return_order", "Возвращён заказ ID: " + to_string(orderId));

        cout << "Заказ ID " << orderId << " возвращён." << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при возврате заказа: " << e.what() << endl;
        return false;
    }
}

void Customer::viewOrderStatusHistory(int orderId) {
    try {
        auto checkResult = db->executeQuery(
            "SELECT user_id FROM orders WHERE order_id = " + to_string(orderId) + ";"
        );

        if (checkResult.empty() || stoi(checkResult[0][0]) != userId) {
            cerr << "Заказ не принадлежит вам." << endl;
            return;
        }

        stringstream sql;
        sql << "SELECT * FROM getOrderStatusHistory(" << orderId << ");";

        auto results = db->executeQuery(sql.str());
        cout << endl;
        cout << "=== ИСТОРИЯ СТАТУСОВ ЗАКАЗА " << orderId << " ===" << endl;
        if (results.empty()) {
            cout << "История не найдена." << endl;
            return;
        }

        for (const auto& row : results) {
            if (row.size() >= 7) {
                cout << "Смена: " << (row[2].empty() ? "новый" : row[2])
                    << " → " << row[3]
                    << " | Дата: " << row[4] << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при получении истории: " << e.what() << endl;
    }
}

vector<shared_ptr<Product>> Customer::getAvailableProducts() {
    vector<shared_ptr<Product>> products;

    try {
        auto results = db->executeQuery(
            "SELECT product_id, name, price, stock_quantity FROM products "
            "WHERE stock_quantity > 0 ORDER BY name;"
        );
        cout << endl;
        cout << "=== ДОСТУПНЫЕ ТОВАРЫ ===" << endl;
        cout << left
            << setw(6) << "ID" << " | "
            << setw(51) << "Название" << " | "
            << setw(22) << "Цена" << " | "
            << setw(10) << "Кол-во" << endl;
        cout << string(80, '-') << endl;

        for (const auto& row : results) {
            if (row.size() >= 4) {
                int id = stoi(row[0]);
                string name = row[1];
                double price = stod(row[2]);
                int quantity = stoi(row[3]);

                string displayName = name;
                if (displayName.length() > 50) {
                    displayName = displayName.substr(0, 47) + "...";
                }

                cout << left << setw(6) << id << " | "
                    << left << setw(51) << displayName << " | "
                    << right << setw(20) << fixed << setprecision(2) << price << " руб | "
                    << right << setw(8) << quantity << " шт."
                    << endl;

                products.push_back(make_shared<Product>(id, name, price, quantity));
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при получении товаров: " << e.what() << endl;
    }

    return products;
}

// Проверка доступности товара доп метод
bool Customer::checkProductAvailability(int productId, int quantity) {
    try {
        auto result = db->executeQuery(
            "SELECT stock_quantity FROM products WHERE product_id = " +
            to_string(productId) + ";"
        );

        if (!result.empty() && !result[0].empty()) {
            int available = stoi(result[0][0]);
            return available >= quantity;
        }
        return false;
    }
    catch (...) {
        return false;
    }
}

void Customer::logAction(const string& operation, const string& details) {
    try {
        stringstream sql;
        sql << "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
            << "VALUES ('order', 0, '" << operation << "', " << userId << ");";
        db->executeNonQuery(sql.str());
    }
    catch (...) {
        // Игнорируем ошибки логирования
    }
}
#include "Manager.h"
#include "DatabaseConnection.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

Manager::Manager(int id, string name, string email,
    shared_ptr<DatabaseConnection<string>> db)
    : User(id, move(name), move(email), "менеджер", db) {
}

void Manager::displayMenu() {
    cout << endl;
    cout << "=== МЕНЮ МЕНЕДЖЕРА ===" << endl;
    cout << "1. Просмотр заказов в ожидании утверждения" << endl;
    cout << "2. Утвердить заказ" << endl;
    cout << "3. Обновить количество товара на складе" << endl;
    cout << "4. Просмотр деталей заказа" << endl;
    cout << "5. Изменить статус заказа" << endl;
    cout << "6. Просмотр истории утверждённых заказов" << endl;
    cout << "7. Просмотр истории статусов заказов" << endl;
    cout << "8. Выход" << endl;
    cout << "Выберите действие: ";
}

bool Manager::canPerformAction(const string& action) {
    if (action == "утвердить_заказ" || action == "обновить_склад" ||
        action == "изменить_статус_заказа") {
        cout << "Доступ менеджера подтверждён для: " << action << endl;
        return true;
    }
    cout << "Доступ запрещён для: " << action << endl;
    return false;
}

bool Manager::approveOrder(int orderId) {
    try {
        // Устанавливаем текущего пользователя для триггеров
        db->executeNonQuery("SET app.current_user_id = '" + to_string(userId) + "'");

        // Используем хранимую процедуру
        stringstream sql;
        sql << "CALL approveOrder(" << orderId << ", " << userId << ");";

        db->executeNonQuery(sql.str());

        logAction("approve_order", "Утверждён заказ ID: " + to_string(orderId));

        cout << "Заказ ID " << orderId << " успешно утверждён." << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при утверждении заказа: " << e.what() << endl;
        return false;
    }
}

bool Manager::updateStock(int productId, int newQuantity) {
    try {
        if (newQuantity < 0) {
            cerr << "Количество не может быть отрицательным." << endl;
            return false;
        }

        stringstream sql;
        sql << "UPDATE products SET stock_quantity = " << newQuantity
            << " WHERE product_id = " << productId << ";";

        db->executeNonQuery(sql.str());

        logAction("update_stock", "Товар ID: " + to_string(productId) +
            " -> " + to_string(newQuantity) + " шт.");

        cout << "Количество товара ID " << productId
            << " обновлено до " << newQuantity << " шт." << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при обновлении склада: " << e.what() << endl;
        return false;
    }
}

void Manager::viewPendingOrders() {
    try {
        string sql = "SELECT o.order_id, u.name, o.total_price, "
            "TO_CHAR(o.order_date, 'DD.MM.YYYY HH24:MI:SS') as order_date "
            "FROM orders o JOIN users u ON o.user_id = u.user_id "
            "WHERE o.status = 'pending' "
            "ORDER BY o.order_date DESC;";

        auto results = db->executeQuery(sql);
        cout << endl;
        cout << "=== ЗАКАЗЫ В ОЖИДАНИИ УТВЕРЖДЕНИЯ ===" << endl;
        if (results.empty()) {
            cout << "Нет заказов в ожидании." << endl;
            return;
        }

        cout << left
            << setw(10) << "ID заказа"
            << setw(50) << "Покупатель"
            << setw(25) << "Сумма"
            << setw(20) << "Дата создания" << endl;
        cout << string(80, '-') << endl;

        for (const auto& row : results) {
            if (row.size() >= 4) {
                cout << left
                    << setw(10) << row[0]
                    << setw(50) << (row[1].length() > 49 ? row[1].substr(0, 46) + "..." : row[1])
                    << setw(25) << row[2] + " руб"
                    << setw(20) << row[3] << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при получении заказов: " << e.what() << endl;
    }
}

void Manager::viewMyApprovedOrders() {
    try {
        stringstream sql;
        sql << "SELECT DISTINCT o.order_id, u.name, o.status, o.total_price, o.order_date "
            << "FROM orders o "
            << "JOIN users u ON o.user_id = u.user_id "
            << "JOIN order_status_history h ON o.order_id = h.order_id "
            << "WHERE h.changed_by = " << userId << " "
            << "ORDER BY o.order_date DESC;";

        auto results = db->executeQuery(sql.str());
        cout << endl;
        cout << "=== ЗАКАЗЫ, УТВЕРЖДЁННЫЕ МНОЙ ===" << endl;
        if (results.empty()) {
            cout << "Вы ещё не утвердили ни одного заказа." << endl;
            return;
        }

        cout << left
            << setw(10) << "ID заказа"
            << setw(50) << "Покупатель"
            << setw(25) << "Статус"
            << setw(25) << "Сумма"
            << setw(20) << "Дата" << endl;
        cout << string(80, '-') << endl;

        for (const auto& row : results) {
            if (row.size() >= 5) {
                cout << left
                    << setw(10) << row[0]
                    << setw(50) << (row[1].length() > 48 ? row[1].substr(0, 45) + "..." : row[1])
                    << setw(25) << row[2]
                    << setw(25) << row[3] + " руб"
                    << setw(20) << row[4] << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при получении заказов: " << e.what() << endl;
    }
}

void Manager::logAction(const string& operation, const string& details) {
    try {
        stringstream sql;
        sql << "INSERT INTO audit_log (entity_type, entity_id, operation, performed_by) "
            << "VALUES ('system', 0, '" << operation << "', " << userId << ");";
        db->executeNonQuery(sql.str());
    }
    catch (...) {
        // Игнорируем ошибки логирования
    }
}

bool Manager::viewOrderDetails(int orderId) {
    try {
        // Есть ли у менеджера доступ к заказу?
        // Менеджер может видеть заказы в ожидании или те, которые он утверждал
        stringstream checkSql;
        checkSql << "SELECT o.order_id, o.status, u.name as customer "
            << "FROM orders o "
            << "JOIN users u ON o.user_id = u.user_id "
            << "WHERE o.order_id = " << orderId << " "
            << "AND (o.status = 'pending' "
            << "OR EXISTS (SELECT 1 FROM order_status_history h "
            << "WHERE h.order_id = o.order_id AND h.changed_by = " << userId << "));";

        auto checkResult = db->executeQuery(checkSql.str());

        if (checkResult.empty()) {
            cout << "Заказ с ID " << orderId << " не найден или у вас нет доступа." << endl;
            cout << "Менеджер может просматривать только:" << endl;
            cout << "1. Заказы в статусе <ожидается>" << endl;
            cout << "2. Заказы, которые вы утверждали" << endl;
            return false;
        }

        cout << endl;
        cout << "=== ДЕТАЛИ ЗАКАЗА ID " << orderId << " ===" << endl;
        cout << "Покупатель: " << checkResult[0][2] << endl;
        cout << "Статус: " << checkResult[0][1] << endl;

        stringstream sql;
        sql << "SELECT o.order_id, u.name as customer, u.email, o.status, "
            << "o.total_price, o.order_date, "
            << "COUNT(oi.order_item_id) as items_count "
            << "FROM orders o "
            << "JOIN users u ON o.user_id = u.user_id "
            << "LEFT JOIN order_items oi ON o.order_id = oi.order_id "
            << "WHERE o.order_id = " << orderId << " "
            << "GROUP BY o.order_id, u.name, u.email, o.status, o.total_price, o.order_date;";

        auto orderResult = db->executeQuery(sql.str());

        if (!orderResult.empty()) {
            cout << "Email покупателя: " << orderResult[0][2] << endl;
            cout << "Общая сумма: " << orderResult[0][4] << " руб." << endl;
            cout << "Дата заказа: " << orderResult[0][5] << endl;
            cout << "Количество позиций: " << orderResult[0][6] << endl;
        }

        // Получаем состав заказа
        sql.str("");
        sql << "SELECT p.product_id, p.name, oi.quantity, oi.price, "
            << "(oi.quantity * oi.price) as total, p.stock_quantity "
            << "FROM order_items oi "
            << "JOIN products p ON oi.product_id = p.product_id "
            << "WHERE oi.order_id = " << orderId << " "
            << "ORDER BY p.name;";

        auto itemsResult = db->executeQuery(sql.str());

        if (!itemsResult.empty()) {
            cout << endl;
            cout << "Состав заказа:" << endl;
            cout << left
                << setw(8) << "ID"
                << setw(25) << "Товар"
                << setw(10) << "Кол-во"
                << setw(12) << "Цена"
                << setw(12) << "Итого"
                << setw(12) << "На складе" << endl;
            cout << string(79, '-') << endl;

            double itemsTotal = 0;
            for (const auto& row : itemsResult) {
                if (row.size() >= 6) {
                    double itemTotal = stod(row[4]);
                    itemsTotal += itemTotal;

                    cout << left
                        << setw(8) << row[0]
                        << setw(25) << (row[1].length() > 22 ? row[1].substr(0, 19) + "..." : row[1])
                        << setw(10) << row[2] + " шт."
                        << setw(12) << row[3] + " руб."
                        << setw(12) << to_string(itemTotal) + " руб."
                        << setw(12) << row[5] + " шт." << endl;
                }
            }
            cout << string(79, '-') << endl;
            cout << right << setw(67) << "Итого по позициям: " << itemsTotal << " руб." << endl;
        }

        // История изменений статусов (только если менеджер участвовал)
        sql.str("");
        sql << "SELECT h.old_status, h.new_status, h.changed_at, u.name "
            << "FROM order_status_history h "
            << "LEFT JOIN users u ON h.changed_by = u.user_id "
            << "WHERE h.order_id = " << orderId << " "
            << "ORDER BY h.changed_at DESC "
            << "LIMIT 5;";

        auto historyResult = db->executeQuery(sql.str());

        if (!historyResult.empty()) {
            cout << endl;
            cout << "Последние изменения статуса:" << endl;
            for (const auto& row : historyResult) {
                if (row.size() >= 4) {
                    cout << "  " << (row[0].empty() ? "[создан]" : row[0])
                        << " → " << row[1]
                        << " | " << row[3]
                        << " | " << row[2] << endl;
                }
            }
        }

        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при получении деталей заказа: " << e.what() << endl;
        return false;
    }
}
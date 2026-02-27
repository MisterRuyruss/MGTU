#include "Admin.h"
#include "DatabaseConnection.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <fstream> 
#include <stdexcept> 

using namespace std;

Admin::Admin(int id, string name, string email,
    shared_ptr<DatabaseConnection<string>> db)
    : User(id, move(name), move(email), "администратор", db) {
}

void Admin::displayMenu() {
    cout << endl;
    cout << "=== МЕНЮ АДМИНИСТРАТОРА ===" << endl;
    cout << "1. Добавить новый продукт" << endl;
    cout << "2. Обновить информацию о продукте" << endl;
    cout << "3. Удалить продукт" << endl;
    cout << "4. Просмотр всех заказов" << endl;
    cout << "5. Просмотр деталей заказа" << endl;
    cout << "6. Изменить статус заказа" << endl;
    cout << "7. Просмотр истории статусов заказа" << endl;
    cout << "8. Просмотр журнала аудита" << endl;
    cout << "9. Сформировать отчёт (CSV)" << endl;
    cout << "10. Выход" << endl;
    cout << "Выберите действие: ";
}


bool Admin::canPerformAction(const string& action) {
    cout << "Доступ администратора подтверждён для: " << action << endl;
    return true;
}

bool Admin::addProduct(const string& name, double price, int quantity) {
    try {
        stringstream sql;
        sql << "INSERT INTO products (name, price, stock_quantity) VALUES ('"
            << name << "', " << price << ", " << quantity << ");";

        db->executeNonQuery(sql.str());

        logAction("add_product", "Добавлен товар: " + name);

        cout << "Товар '" << name << "' успешно добавлен." << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при добавлении товара: " << e.what() << endl;
        return false;
    }
}

bool Admin::updateProduct(int productId, double newPrice, int newQuantity) {
    try {
        stringstream sql;
        sql << "UPDATE products SET price = " << newPrice
            << ", stock_quantity = " << newQuantity
            << " WHERE product_id = " << productId << ";";

        db->executeNonQuery(sql.str());

        logAction("update_product", "Обновлён товар ID: " + to_string(productId));

        cout << "Товар ID " << productId << " успешно обновлён." << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при обновлении товара: " << e.what() << endl;
        return false;
    }
}

bool Admin::deleteProduct(int productId) {
    try {
        stringstream sql;
        sql << "DELETE FROM products WHERE product_id = " << productId << ";";

        db->executeNonQuery(sql.str());

        logAction("delete_product", "Удалён товар ID: " + to_string(productId));

        cout << "Товар ID " << productId << " успешно удалён." << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при удалении товара: " << e.what() << endl;
        return false;
    }
}

void Admin::viewAllOrders() {
    try {
        string sql = "SELECT o.order_id, u.name, o.status, o.total_price, "
            "TO_CHAR(o.order_date, 'DD.MM.YYYY HH24:MI:SS') as order_date "
            "FROM orders o JOIN users u ON o.user_id = u.user_id "
            "ORDER BY o.order_date DESC;";

        auto results = db->executeQuery(sql);
        cout << endl;
        cout << "=== ВСЕ ЗАКАЗЫ ===" << endl;
        cout << left
            << setw(15) << "ID заказа"
            << setw(50) << "Покупатель"
            << setw(20) << "Статус"
            << setw(20) << "Сумма"
            << setw(25) << "Дата" << endl;
        cout << string(80, '-') << endl;

        for (const auto& row : results) {
            if (row.size() >= 5) {
                cout << left
                    << setw(15) << row[0]
                    << setw(50) << (row[1].length() > 48 ? row[1].substr(0, 45) + "..." : row[1])
                    << setw(15) << row[2]
                    << setw(15) << row[3] + " руб."
                    << setw(20) << row[4] << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при получении заказов: " << e.what() << endl;
    }
}

bool Admin::updateOrderStatus(int orderId, const string& newStatus) {
    try {
        db->executeNonQuery("SET app.current_user_id = '" + to_string(userId) + "'");

        stringstream sql;
        sql << "UPDATE orders SET status = '" << newStatus
            << "' WHERE order_id = " << orderId << ";";

        db->executeNonQuery(sql.str());

        logAction("update_order_status", "Заказ ID: " + to_string(orderId) +
            " -> " + newStatus);

        cout << "Статус заказа ID " << orderId
            << " изменён на '" << newStatus << "'" << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при изменении статуса : " << e.what() << endl;
        return false;
    }
}

void Admin::viewOrderStatusHistory(int orderId) {
    try {
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
                cout << "Смена: " << row[2] << " → " << row[3]
                    << " | Изменил: " << row[6]
                    << " | Дата: " << row[4] << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при получении истории: " << e.what() << endl;
    }
}

void Admin::viewAuditLog() {
    try {
        string sql = "SELECT a.log_id, a.entity_type, a.entity_id, "
            "a.operation, u.name, TO_CHAR(a.performed_at, 'DD.MM.YYYY HH24:MI:SS') as performed_at "
            "FROM audit_log a LEFT JOIN users u ON a.performed_by = u.user_id "
            "ORDER BY a.performed_at DESC LIMIT 50;";

        auto results = db->executeQuery(sql);

        cout << "=== ЖУРНАЛ АУДИТА (последние 50 записей) ===" << endl;
        cout << left
            << setw(8) << "ID"
            << setw(19) << "Сущность"
            << setw(25) << "ID сущности"
            << setw(33) << "Операция"
            << setw(37) << "Пользователь"
            << setw(20) << "Дата" << endl;
        cout << string(100, '-') << endl;

        for (const auto& row : results) {
            if (row.size() >= 6) {
                cout << left
                    << setw(8) << row[0]
                    << setw(16) << row[1]
                    << setw(12) << row[2]
                    << setw(25) << row[3]
                    << setw(37) << (row[4].empty() ? "система" : row[4])
                    << setw(20) << row[5] << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка при получении журнала аудита: " << e.what() << endl;
    }
}

bool Admin::generateCSVReport() {
    try {
        cout << "Генерация отчёта..." << endl;

        string reportPath = "D:/MGTU/Exam_INF/1_sem/online_store/reports/audit_report.csv";
        system(("mkdir \"" + reportPath.substr(0, reportPath.find_last_of("/\\")) + "\" 2>nul").c_str());

        string sql = R"(
            COPY (
                SELECT 
                    o.order_id,
                    CONVERT_FROM(convert_to(u.name, 'UTF-8'), 'UTF-8') as customer,
                    o.status,
                    o.total_price,
                    o.order_date,
                    h.new_status,
                    h.changed_at,
                    u2.name as changed_by_name,
                    a.operation,
                    a.performed_at as audit_time
                FROM orders o
                LEFT JOIN users u ON o.user_id = u.user_id
                LEFT JOIN order_status_history h ON o.order_id = h.order_id
                LEFT JOIN users u2 ON h.changed_by = u2.user_id
                LEFT JOIN audit_log a ON o.order_id = a.entity_id AND a.entity_type = 'order'
                ORDER BY o.order_date DESC
            ) TO ')" + reportPath + R"(' 
            WITH (FORMAT CSV, HEADER true, DELIMITER ',', ENCODING 'UTF8');
        )";

        db->executeNonQuery(sql);

        logAction("generate_report", "Сгенерирован CSV отчёт");

        cout << "Отчёт успешно сохранён в " << reportPath << endl;
        cout << "Записей: " << reportPath.size() << endl;

        

        return true;
    }
    catch (const exception& e) {
        cerr << "Ошибка при генерации отчёта: " << e.what() << endl;
        return false;
    }
}

      

void Admin::logAction(const string& operation, const string& details) {
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

bool Admin::viewOrderDetails(int orderId) {
    try {
        stringstream sql;
        sql << "SELECT o.order_id, u.name as customer, o.status, "
            << "o.total_price, o.order_date, "
            << "COUNT(oi.order_item_id) as items_count "
            << "FROM orders o "
            << "JOIN users u ON o.user_id = u.user_id "
            << "LEFT JOIN order_items oi ON o.order_id = oi.order_id "
            << "WHERE o.order_id = " << orderId << " "
            << "GROUP BY o.order_id, u.name, o.status, o.total_price, o.order_date;";

        auto orderResult = db->executeQuery(sql.str());

        if (orderResult.empty()) {
            cout << "Заказ с ID " << orderId << " не найден." << endl;
            return false;
        }

        // Вывод основной информации о заказе
        cout << endl;
        cout << "=== ДЕТАЛИ ЗАКАЗА ID " << orderId << " ===" << endl;
        cout << "Покупатель: " << orderResult[0][1] << endl;
        cout << "Статус: " << orderResult[0][2] << endl;
        cout << "Общая сумма: " << orderResult[0][3] << " руб." << endl;
        cout << "Дата заказа: " << orderResult[0][4] << endl;
        cout << "Количество позиций: " << orderResult[0][5] << endl;

        // Получаем состав заказа
        sql.str("");
        sql << "SELECT p.product_id, p.name, oi.quantity, oi.price, "
            << "(oi.quantity * oi.price) as total "
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
                << setw(12) << "Итого" << endl;
            cout << string(67, '-') << endl;

            for (const auto& row : itemsResult) {
                if (row.size() >= 5) {
                    cout << left
                        << setw(8) << row[0]
                        << setw(25) << (row[1].length() > 22 ? row[1].substr(0, 19) + "..." : row[1])
                        << setw(10) << row[2] + " шт."
                        << setw(12) << row[3] + " руб."
                        << setw(12) << row[4] + " руб." << endl;
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
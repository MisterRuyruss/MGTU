# Система интернет-магазина на C++ с PostgreSQL

## 1. Описание задачи
### Цель работы
Разработка полнофункциональной системы интернет-магазина с использованием принципов объектно-ориентированного программирования, умных указателей, транзакций PostgreSQL и автоматического аудита действий пользователей.

### Краткое описание реализованной системы
Система предоставляет три уровня доступа:
1. **Администратор** - полный контроль над системой
2. **Менеджер** - управление заказами и складом
3. **Покупатель** - оформление и отслеживание заказов

### Используемые технологии
- **C++17** с использованием STL и умных указателей
- **PostgreSQL 14+** как система управления базами данных
- **libpqxx 7.10+** как клиентская библиотека для C++
- **CMake 3.15+** для сборки проекта

## 2. Архитектура проекта

### Описание классов и их взаимосвязей
```
User (абстрактный)
├── Admin
├── Manager
└── Customer
    └── владеет → Order*
        ├── содержит → OrderItem* (композиция)
        └── содержит → Payment* (композиция)
            └── использует → PaymentStrategy (стратегия)
Product
DatabaseConnection<T> (шаблонный)
```

### Применение принципов ООП

#### Наследование
```cpp
class User { /* базовый класс */ };
class Admin : public User { /* производный */ };
class Manager : public User { /* производный */ };
class Customer : public User { /* производный */ };
```

#### Полиморфизм
```cpp
class User {
public:
    virtual void displayMenu() = 0; // чисто виртуальная функция
    virtual bool canPerformAction(const string& action) = 0;
};
```

#### Композиция
```cpp
class Order {
private:
    vector<unique_ptr<OrderItem>> items; // OrderItem не существует без Order
    unique_ptr<Payment> payment; // Payment создаётся и удаляется с Order
};
```

#### Агрегация
```cpp
class User {
private:
    vector<shared_ptr<Order>> orders; // Заказы могут существовать отдельно
};
```

### Использование шаблонного класса DatabaseConnection<T>
```cpp
template<typename T>
class DatabaseConnection {
private:
    unique_ptr<pqxx::connection> conn;
public:
    explicit DatabaseConnection(const T& connStr);
    vector<vector<string>> executeQuery(const string& sql);
    void executeNonQuery(const string& sql);
    // ... другие методы
};

// Использование:
auto db = make_shared<DatabaseConnection<string>>(
    "host=localhost user=postgres dbname=online_store_db"
);
```

## 3. Работа с базой данных

### Структура базы данных

#### Список таблиц
1. **users** - пользователи системы
2. **products** - каталог товаров
3. **orders** - заказы
4. **order_items** - элементы заказов
5. **order_status_history** - история изменений статусов
6. **audit_log** - журнал аудита действий

### Хранимые процедуры
1. **createOrder** - создание заказа с транзакцией
   ```sql
   CREATE PROCEDURE createOrder(
       p_user_id INTEGER,
       p_product_ids INTEGER[],
       p_quantities INTEGER[],
       OUT p_order_id INTEGER,
       OUT p_total_price DECIMAL,
       OUT p_message VARCHAR
   )
   ```

2. **updateOrderStatus** - обновление статуса с записью в историю
3. **approveOrder** - утверждение заказа менеджером
4. **cancelOrder** - отмена заказа с возвратом товаров

### Функции
1. **getOrderStatus(order_id)** - возвращает статус заказа
2. **getUserOrderCount()** - количество заказов по пользователям
3. **getTotalSpentByUser(user_id)** - общая сумма покупок
4. **canReturnOrder(order_id)** - проверка возможности возврата
5. **getOrderStatusHistory(order_id)** - история статусов
6. **getAuditLogByUser(user_id)** - журнал действий пользователя

### Триггеры
1. **trigger_log_order_status** - автоматическая запись в order_status_history
2. **trigger_audit_orders** - логирование операций с заказами
3. **trigger_audit_products** - логирование изменений товаров
4. **trigger_audit_users** - логирование операций с пользователями
5. **trigger_update_order_prices** - пересчёт цен при изменении стоимости товара

### Механизм транзакций и отката при ошибках
```cpp
db->beginTransaction();
try {
    db->executeNonQuery("INSERT INTO orders ...");
    db->executeNonQuery("INSERT INTO order_items ...");
    db->commitTransaction();
} catch (const exception& e) {
    db->rollbackTransaction();
    // запись в audit_log
}
```

## 4. Умные указатели и STL

### Использование unique_ptr и shared_ptr
```cpp
// unique_ptr - эксклюзивное владение
unique_ptr<Payment> payment = make_unique<Payment>(...);

// shared_ptr - разделяемое владение
vector<shared_ptr<Order>> orders;
auto order = make_shared<Order>(...);
orders.push_back(order);
```

### Примеры использования STL алгоритмов

#### find_if с лямбда-выражением
```cpp
auto it = find_if(orders.begin(), orders.end(),
    [orderId](const shared_ptr<Order>& order) {
        return order->getOrderId() == orderId;
    });
```

#### copy_if для фильтрации
```cpp
vector<shared_ptr<Order>> pendingOrders;
copy_if(allOrders.begin(), allOrders.end(), 
        back_inserter(pendingOrders),
        [](const shared_ptr<Order>& o) {
            return o->getStatus() == "pending";
        });
```

#### accumulate для агрегации
```cpp
double totalRevenue = accumulate(orders.begin(), orders.end(), 0.0,
    [](double sum, const shared_ptr<Order>& order) {
        return sum + order->getTotalPrice();
    });
```

#### Лямбда-выражения для проверки прав
```cpp
auto checkPermission = [user](const string& action) -> bool {
    if (user->getRole() == "admin") return true;
    if (action == "view_orders" && user->getRole() == "manager") return true;
    return false;
};
```

## 5. Логика ролей и прав доступа

### Возможности администратора
- Полный доступ ко всем данным
- Управление каталогом товаров (добавление, обновление, удаление)
- Просмотр всех заказов и их истории
- Генерация отчётов в формате CSV
- Просмотр полного журнала аудита

### Возможности менеджера
- Утверждение заказов
- Обновление информации о наличии товаров
- Просмотр заказов в обработке
- Изменение статусов заказов
- Доступ только к связанным с заказами записям аудита

### Возможности покупателя
- Создание и управление своими заказами
- Добавление/удаление товаров в корзине
- Оплата заказа (карта, электронный кошелёк, СБП)
- Возврат товаров в течение 30 дней
- Просмотр истории только своих заказов
- Нет доступа к журналу аудита

### Ограничения доступа
```cpp
bool Customer::canPerformAction(const string& action) {
    if (action == "view_audit_log") return false;
    if (action == "view_all_orders") return false;
    return action == "create_order" || action == "view_my_orders";
}
```

### Реализация проверки прав доступа
```cpp
bool User::canPerformAction(const string& action) {
    // Лямбда-функция для проверки
    auto hasPermission = [this, &action]() {
        if (role == "admin") return true;
        if (role == "manager") {
            return action == "approve_order" || 
                   action == "update_stock" || 
                   action == "view_pending_orders";
        }
        if (role == "customer") {
            return action == "create_order" || 
                   action == "make_payment" || 
                   action == "view_my_orders";
        }
        return false;
    };
    return hasPermission();
}
```

## 6. Аудит и история изменений

### Таблицы аудита

#### order_status_history
```sql
CREATE TABLE order_status_history (
    history_id SERIAL PRIMARY KEY,
    order_id INTEGER REFERENCES orders(order_id),
    old_status VARCHAR(50),
    new_status VARCHAR(50) NOT NULL,
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changed_by INTEGER REFERENCES users(user_id)
);
```

#### audit_log
```sql
CREATE TABLE audit_log (
    log_id SERIAL PRIMARY KEY,
    entity_type VARCHAR(50) CHECK (entity_type IN ('order', 'product', 'user')),
    entity_id INTEGER NOT NULL,
    operation VARCHAR(50) CHECK (operation IN ('insert', 'update', 'delete')),
    performed_by INTEGER REFERENCES users(user_id),
    performed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### Механизм автоматического логирования
```cpp
// Триггер в PostgreSQL
CREATE TRIGGER trigger_audit_orders
AFTER INSERT OR UPDATE OR DELETE ON orders
FOR EACH ROW EXECUTE FUNCTION log_audit_event();

// Установка ID пользователя перед операцией
void setCurrentUser(pqxx::connection* conn, int user_id) {
    pqxx::work w(*conn);
    w.exec("SET app.current_user_id = '" + to_string(user_id) + "'");
    w.commit();
}
```

### Примеры записей аудита
```
log_id | entity_type | entity_id | operation | performed_by | performed_at
-------|-------------|-----------|-----------|--------------|-------------
1      | product     | 5         | insert    | 1 (admin)    | 2026-01-12 10:00:00
2      | order       | 100       | update    | 2 (manager)  | 2026-01-13 11:30:00
3      | user        | 3         | delete    | 1 (admin)    | 2026-01-14 14:45:00
```

## 7. Отчёт в формате CSV

### Описание отчёта «История изменений заказов и действий пользователей»
Отчёт объединяет данные из трёх таблиц:
- `orders` - информация о заказах
- `order_status_history` - изменения статусов
- `audit_log` - действия пользователей

### SQL-запрос для формирования отчёта
```sql
COPY (
    SELECT 
        o.order_id,
        u.name as customer_name,
        o.status as current_status,
        o.total_price,
        o.order_date,
        h.old_status,
        h.new_status,
        h.changed_at as status_change_time,
        u2.name as changed_by_user,
        a.operation,
        a.performed_at as audit_time,
        u3.name as performer
    FROM orders o
    LEFT JOIN users u ON o.user_id = u.user_id
    LEFT JOIN order_status_history h ON o.order_id = h.order_id
    LEFT JOIN audit_log a ON o.order_id = a.entity_id AND a.entity_type = 'order'
    LEFT JOIN users u2 ON h.changed_by = u2.user_id
    LEFT JOIN users u3 ON a.performed_by = u3.user_id
    WHERE o.order_date >= CURRENT_DATE - INTERVAL '30 days'
    ORDER BY o.order_date DESC, h.changed_at DESC
) TO '/path/to/report.csv' 
WITH CSV HEADER DELIMITER ',' ENCODING 'UTF8';
```

### Пример содержимого CSV-файла
```csv
order_id,customer_name,current_status,total_price,order_date,old_status,new_status,status_change_time,changed_by_user,operation,audit_time,performer
100,Иванов Иван,completed,29999.50,2026-01-10,pending,completed,2026-01-11 09:30:00,Петров Петр,update,2026-01-11 09:30:00,Петров Петр
101,Сидорова Мария,pending,54999.00,2026-01-12,NULL,pending,2026-01-12 14:20:00,NULL,insert,2026-01-12 14:20:00,Сидорова Мария
```

## 8. Сборка и запуск проекта

### Требования к окружению
- **Компилятор:** Visual Studio 2026 
- **PostgreSQL:** версия 14 или выше
- **libpqxx:** версия 7.10 или выше
- **CMake:** версия 3.15 или выше

### Инструкции по сборке

#### Visual Studio
1. Открыть папку `online_store` в Visual Studio
2. Выбрать конфигурацию `x64-Debug` или `x64-Release`
3. Нажать `Ctrl+Shift+B` для сборки
4. Запустить программу `F5`

### Инструкция по запуску программы

1. **Запустить PostgreSQL** сервер
2. **Выполнить SQL-скрипты** в порядке:
   ```bash
   psql -U postgres -d online_store_db -f sql/tables.sql
   psql -U postgres -d online_store_db -f sql/functions.sql
   psql -U postgres -d online_store_db -f sql/procedures.sql
   psql -U postgres -d online_store_db -f sql/triggers.sql
   psql -U postgres -d online_store_db -f sql/sample_data.sql
   ```
3. **Запустить программу:**
   ```bash
   # Windows
   online_store.exe
   
   # Linux/Mac
   ./online_store
   ```

### Пример запуска программы
```
========================================
     СИСТЕМА ИНТЕРНЕТ-МАГАЗИНА
========================================

Подключение к базе данных...
Подключение к базе данных установлено успешно.

=== ГЛАВНОЕ МЕНЮ ===
1. Войти как Администратор
2. Войти как Менеджер
3. Войти как Покупатель
4. Выход
Выберите роль: 1

=== ВХОД АДМИНИСТРАТОРА ===
Добро пожаловать, Администратор Системы!

=== МЕНЮ АДМИНИСТРАТОРА ===
1. Добавить новый продукт
2. Обновить информацию о продукте
3. Удалить продукт
4. Просмотр всех заказов
5. Просмотр деталей заказа
6. Изменить статус заказа
7. Просмотр истории статусов заказа
8. Просмотр журнала аудита
9. Сформировать отчёт (CSV)
10. Выход
Выберите действие: 4

=== ВСЕ ЗАКАЗЫ ===
ID заказа  Покупатель            Статус       Сумма       Дата
------------------------------------------------------------------------
100        Иванов Иван           completed    29999.50    2026-01-10
101        Сидорова Мария        pending      54999.00    2026-01-12
```

### Примеры работы меню для разных ролей

#### Менеджер
```
=== МЕНЮ МЕНЕДЖЕРА ===
1. Просмотр заказов в ожидании утверждения
2. Утвердить заказ
3. Обновить количество товара на складе
4. Просмотр деталей заказа
5. Изменить статус заказа
6. Просмотр истории утверждённых заказов
7. Просмотр истории статусов заказов
8. Выход
```

#### Покупатель
```
=== МЕНЮ ПОКУПАТЕЛЯ ===
1. Создать новый заказ
2. Добавить товар в заказ
3. Удалить товар из заказа
4. Просмотр моих заказов
5. Просмотр статуса заказа
6. Оплатить заказ
7. Оформить возврат заказа
8. Просмотр истории статусов заказа
9. Просмотр доступных товаров
10. Выход
```

### Примеры логов и истории изменений заказов

#### История статусов заказа #100
```
=== ИСТОРИЯ СТАТУСОВ ЗАКАЗА 100 ===
Смена: pending → processing | Изменил: Менеджер Иванов | Дата: 2026-01-11 09:30:00
Смена: processing → completed | Изменил: Система | Дата: 2026-01-11 10:15:00
```

#### Журнал аудита (последние записи)
```
=== ЖУРНАЛ АУДИТА ===
ID  Сущность   ID сущности  Операция  Пользователь        Дата
--------------------------------------------------------------------
45  order      100          update    Администратор       2026-01-12 10:00:00
46  product    5            insert    Менеджер Иванов     2026-01-12 11:30:00
47  user       8            delete    Администратор       2026-01-12 14:45:00
```

---

## Заключение
Проект демонстрирует применение современных подходов к разработке программного обеспечения: принципов ООП, шаблонов проектирования, работы с базами данных через транзакции, автоматического аудита и безопасного управления памятью через умные указатели. Система готова к промышленному использованию и может быть расширена дополнительной функциональностью.

---
*Храмцов Артём ИУ1-12Б*

*Разработано в рамках экзамена по информатике*

*МГТУ им. Н.Э. Баумана, 2026 год*
```


INSERT INTO users (name, email, role, password_hash, loyalty_level) VALUES
('Администратор Системы', 'admin@store.com', 'admin', '$2y$10$hashedpassword1', 1),
('Менеджер Иванов', 'manager@store.com', 'manager', '$2y$10$hashedpassword2', 1),
('Покупатель Петров', 'customer1@store.com', 'customer', '$2y$10$hashedpassword3', 0),
('Покупатель Сидорова', 'customer2@store.com', 'customer', '$2y$10$hashedpassword4', 1),
('Менеджер Смирнова', 'manager2@store.com', 'manager', '$2y$10$hashedpassword5', 1),
('Покупатель Козлов', 'customer3@store.com', 'customer', '$2y$10$hashedpassword6', 0);

INSERT INTO products (name, price, stock_quantity) VALUES
('Ноутбук ASUS VivoBook', 54999.99, 10),
('Смартфон iPhone 15', 89999.50, 25),
('Наушники Sony WH-1000XM5', 29999.00, 50),
('Клавиатура механическая', 7499.99, 100),
('Монитор 27" 4K', 32999.00, 15),
('Мышь беспроводная', 2499.50, 200),
('Планшет Samsung Galaxy Tab', 45999.00, 30),
('Фитнес-браслет', 5999.99, 150);

INSERT INTO orders (user_id, status, total_price, order_date) VALUES
(3, 'completed', 94999.49, '2026-01-15 10:30:00'),
(3, 'pending', 37999.00, '2026-01-20 14:45:00'),
(4, 'completed', 59999.99, '2026-01-10 09:15:00'),
(4, 'canceled', 29999.00, '2026-01-12 16:20:00'),
(6, 'returned', 89999.50, '2026-01-05 11:10:00'),
(6, 'completed', 104999.49, '2026-01-18 13:25:00');


INSERT INTO order_items (order_id, product_id, quantity, price) VALUES
--  Ноутбук + Наушники
(1, 1, 1, 54999.99),
(1, 3, 1, 29999.00),

-- Монитор
(2, 5, 1, 32999.00),
(2, 6, 2, 2499.50),

-- Смартфон
(3, 2, 1, 89999.50),

-- Наушники (отменён)
(4, 3, 1, 29999.00),

--  (возвращён)
(5, 2, 1, 89999.50),

-- Ноутбук + Планшет
(6, 1, 1, 54999.99),
(6, 7, 1, 45999.00),
(6, 4, 1, 7499.99);

-- пример истории заказов
INSERT INTO order_status_history (order_id, old_status, new_status, changed_at, changed_by) VALUES
(1, 'pending', 'completed', '2024-01-16 09:00:00', 2),
(4, 'pending', 'canceled', '2024-01-13 10:30:00', 3),
(5, 'completed', 'returned', '2024-01-10 14:20:00', 4),
(6, 'pending', 'completed', '2024-01-19 11:45:00', 5);

-- записи аудита
INSERT INTO audit_log (entity_type, entity_id, operation, performed_by, performed_at) VALUES
('product', 1, 'insert', 1, '2024-01-01 10:00:00'),
('product', 2, 'insert', 1, '2024-01-01 10:05:00'),
('order', 1, 'insert', 3, '2024-01-15 10:30:00'),
('order', 1, 'update', 2, '2024-01-16 09:00:00'),
('user', 3, 'insert', 1, '2024-01-01 12:00:00'),
('product', 3, 'update', 1, '2024-01-02 14:30:00'),
('order', 4, 'update', 3, '2024-01-13 10:30:00');

-- Обновляем итоговые суммы заказов на основе элементов (корректировка)
UPDATE orders o
SET total_price = (
    SELECT SUM(oi.quantity * oi.price)
    FROM order_items oi
    WHERE oi.order_id = o.order_id
)
WHERE o.order_id IN (1, 2, 3, 4, 5, 6);


SELECT '=== ПОЛЬЗОВАТЕЛИ ===' AS info;
SELECT user_id, name, email, role, loyalty_level FROM users ORDER BY user_id;

SELECT '=== ПРОДУКТЫ ===' AS info;
SELECT product_id, name, price, stock_quantity FROM products ORDER BY product_id;

SELECT '=== ЗАКАЗЫ ===' AS info;
SELECT order_id, user_id, status, total_price, order_date FROM orders ORDER BY order_id;

SELECT '=== ЭЛЕМЕНТЫ ЗАКАЗОВ ===' AS info;
SELECT order_item_id, order_id, product_id, quantity, price FROM order_items ORDER BY order_id, product_id;

SELECT '=== ИСТОРИЯ СТАТУСОВ ===' AS info;
SELECT history_id, order_id, old_status, new_status, changed_at, changed_by 
FROM order_status_history ORDER BY history_id;

SELECT '=== АУДИТ ЛОГ ===' AS info;
SELECT log_id, entity_type, entity_id, operation, performed_by, performed_at 
FROM audit_log ORDER BY log_id;



/*
-- общая выручка за месяц
SELECT c.category_name, SUM(o.total_price) AS total_revenue
FROM orders o
JOIN dishes d ON o.dish_id = d.id
JOIN categories c ON d.category_id = c.id
WHERE EXTRACT(MONTH FROM o.order_date) = 3 AND EXTRACT(YEAR FROM o.order_date) = 2024
GROUP BY c.category_name;

-- повышение цен на 10%
UPDATE dishes
SET price = price * 1.10
WHERE category_id = (SELECT id FROM categories WHERE category_name = 'Десерты');

-- проверка
SELECT name, price FROM dishes WHERE category_id = 4;

-- удаление блюда
DELETE FROM orders WHERE dish_id = 1;
DELETE FROM dishes WHERE id = 1;
-- проверка 
SELECT * FROM dishes;
SELECT * FROM dishes;
*/

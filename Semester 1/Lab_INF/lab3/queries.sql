/*
SELECT d.name, d.price
FROM dishes d
JOIN categories c ON d.category_id = c.id
WHERE c.category_name = 'Основные блюда';

SELECT o.order_id, d.name, c.category_name, o.order_date, o.quantity, o.total_price
FROM orders o
JOIN dishes d ON o.dish_id = d.id
JOIN categories c ON d.category_id = c.id
WHERE o.order_date BETWEEN '2024-03-01' AND '2024-03-03'
ORDER BY o.order_date;

SELECT SUM(total_price) AS total_revenue, COUNT(*) AS total_orders
FROM orders
WHERE order_date = '2024-03-02';

SELECT d.name, c.category_name, SUM(o.quantity) AS total_ordered, SUM(o.total_price) AS total_revenue
FROM orders o
JOIN dishes d ON o.dish_id = d.id
JOIN categories c ON d.category_id = c.id
GROUP BY d.name, c.category_name
ORDER BY total_ordered DESC;

*/


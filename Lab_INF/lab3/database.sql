CREATE DATABASE restaurant_menu;


CREATE TABLE categories (
    id SERIAL PRIMARY KEY,
    category_name VARCHAR(100) NOT NULL
);

CREATE TABLE dishes (
    id SERIAL PRIMARY KEY,
    name VARCHAR(200) NOT NULL,
    category_id INTEGER NOT NULL,
    price DECIMAL(10, 2) NOT NULL,
    FOREIGN KEY (category_id) REFERENCES categories(id)
);

CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    dish_id INTEGER NOT NULL,
    order_date DATE NOT NULL,
    quantity INTEGER NOT NULL CHECK (quantity > 0),
    total_price DECIMAL(10, 2) NOT NULL,
    FOREIGN KEY (dish_id) REFERENCES dishes(id)
);

INSERT INTO categories (category_name) VALUES
('Супы'),
('Основные блюда'),
('Закуски'),
('Десерты'),
('Напитки'),
('Салаты');

INSERT INTO dishes (name, category_id, price) VALUES
('Борщ', 1, 250.00),
('Том Ям', 1, 350.00),
('Куриный суп', 1, 200.00),
('Стейк Рибай', 2, 1200.00),
('Лосось на гриле', 2, 800.00),
('Паста Карбонара', 2, 450.00),
('Курица Терияки', 2, 550.00),
('Брускетта', 3, 280.00),
('Кальмары в кляре', 3, 320.00),
('Спринг-роллы', 3, 290.00),
('Цезарь', 6, 350.00),
('Греческий', 6, 300.00),
('Оливье', 6, 280.00),
('Тирамису', 4, 300.00),
('Чизкейк', 4, 350.00),
('Мороженое', 4, 200.00),
('Кола', 5, 150.00),
('Сок апельсиновый', 5, 180.00),
('Чай зеленый', 5, 120.00);

INSERT INTO orders (dish_id, order_date, quantity, total_price) VALUES
(1, '2024-03-01', 2, 500.00),
(2, '2024-03-01', 1, 350.00),
(5, '2024-03-01', 1, 1200.00),
(12, '2024-03-01', 3, 1050.00),
(3, '2024-03-02', 2, 400.00),
(6, '2024-03-02', 1, 450.00),
(8, '2024-03-02', 2, 1100.00),
(15, '2024-03-02', 4, 1200.00),
(4, '2024-03-03', 1, 550.00),
(7, '2024-03-03', 3, 960.00),
(10, '2024-03-03', 2, 640.00),
(17, '2024-03-03', 2, 600.00),
(9, '2024-03-04', 1, 290.00),
(11, '2024-03-04', 2, 560.00),
(13, '2024-03-04', 1, 350.00),
(16, '2024-03-04', 3, 360.00);
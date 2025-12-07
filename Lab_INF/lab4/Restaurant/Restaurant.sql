


CREATE TABLE categories (
    id SERIAL PRIMARY KEY,
    category_name VARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE dishes (
    id SERIAL PRIMARY KEY,
    name VARCHAR(200) NOT NULL,
    category_id INTEGER NOT NULL,
    price DECIMAL(10, 2) NOT NULL CHECK (price >= 0),
    FOREIGN KEY (category_id) REFERENCES categories(id)
);

CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    dish_id INTEGER NOT NULL,
    order_date DATE NOT NULL DEFAULT CURRENT_DATE,
    quantity INTEGER NOT NULL CHECK (quantity > 0),
    total_price DECIMAL(10, 2) NOT NULL,
    FOREIGN KEY (dish_id) REFERENCES dishes(id)
);

CREATE INDEX idx_categories_name ON categories(category_name);
CREATE INDEX idx_orders_date ON orders(order_date);

INSERT INTO categories (category_name) VALUES 
    ('Салаты'),
    ('Супы'),
    ('Основные блюда'),
    ('Десерты');

INSERT INTO dishes (name, category_id, price) VALUES
    ('Цезарь', 1, 450.00),
    ('Борщ', 2, 300.00),
    ('Стейк', 3, 1200.00),
    ('Тирамису', 4, 400.00);

INSERT INTO orders (dish_id, order_date, quantity, total_price) VALUES
    (1, '2025-12-01', 2, 900.00),
    (3, '2025-12-01', 1, 1200.00);



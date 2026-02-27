CREATE TABLE IF NOT EXISTS users (
    user_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    role VARCHAR(20) CHECK (role IN ('admin', 'manager', 'customer')) NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    loyalty_level INTEGER DEFAULT 0 CHECK (loyalty_level IN (0, 1))
);


CREATE TABLE IF NOT EXISTS products (
    product_id SERIAL PRIMARY KEY,
    name VARCHAR(200) NOT NULL,
    price DECIMAL(10, 2) CHECK (price > 0) NOT NULL,
    stock_quantity INTEGER CHECK (stock_quantity >= 0) NOT NULL
);


CREATE TABLE IF NOT EXISTS orders (
    order_id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(user_id) ON DELETE CASCADE,
    status VARCHAR(50) CHECK (status IN ('pending', 'completed', 'canceled', 'returned')) DEFAULT 'pending',
    total_price DECIMAL(10, 2) DEFAULT 0,
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


CREATE TABLE IF NOT EXISTS order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id INTEGER REFERENCES orders(order_id) ON DELETE CASCADE,
    product_id INTEGER REFERENCES products(product_id) ON DELETE CASCADE,
    quantity INTEGER CHECK (quantity > 0) NOT NULL,
    price DECIMAL(10, 2) CHECK (price >= 0) NOT NULL
);


CREATE TABLE IF NOT EXISTS order_status_history (
    history_id SERIAL PRIMARY KEY,
    order_id INTEGER REFERENCES orders(order_id) ON DELETE CASCADE,
    old_status VARCHAR(50),
    new_status VARCHAR(50) NOT NULL,
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changed_by INTEGER REFERENCES users(user_id) ON DELETE SET NULL
);


CREATE TABLE IF NOT EXISTS audit_log (
    log_id SERIAL PRIMARY KEY,
    entity_type VARCHAR(50) CHECK (entity_type IN ('order', 'product', 'user')) NOT NULL,
    entity_id INTEGER NOT NULL,
    operation VARCHAR(50) CHECK (operation IN ('insert', 'update', 'delete')) NOT NULL,
    performed_by INTEGER REFERENCES users(user_id) ON DELETE SET NULL,
    performed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
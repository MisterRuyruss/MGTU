CREATE TABLE products_dim (
    product_id INTEGER PRIMARY KEY,
    product_name TEXT NOT NULL,
    category TEXT,
    price NUMERIC(10,2)
);

CREATE TABLE customers_dim (
    customer_id INTEGER PRIMARY KEY,
    customer_name TEXT NOT NULL,
    region TEXT
);

CREATE TABLE time_dim (
    time_id SERIAL PRIMARY KEY,
    date DATE UNIQUE NOT NULL,
    year INTEGER,
    quarter INTEGER,
    month INTEGER,
    day INTEGER,
    day_of_week TEXT
);

CREATE TABLE sales_fact (
    sale_id INTEGER PRIMARY KEY,
    sale_date DATE NOT NULL,
    product_id INTEGER REFERENCES products_dim(product_id),
    customer_id INTEGER REFERENCES customers_dim(customer_id),
    quantity INTEGER,
    amount NUMERIC(10,2)
);
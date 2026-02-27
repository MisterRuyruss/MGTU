
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


CREATE INDEX idx_sales_fact_product ON sales_fact(product_id);
CREATE INDEX idx_sales_fact_customer ON sales_fact(customer_id);
CREATE INDEX idx_sales_fact_date ON sales_fact(sale_date);
CREATE INDEX idx_products_dim_category ON products_dim(category);
CREATE INDEX idx_customers_dim_region ON customers_dim(region);


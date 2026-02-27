-- 1 процедура - создание заказа с транзакцией
CREATE OR REPLACE PROCEDURE createOrder(
    p_user_id INTEGER,
    p_product_ids INTEGER[],
    p_quantities INTEGER[],
    OUT p_order_id INTEGER,
    OUT p_total_price DECIMAL,
    OUT p_message VARCHAR
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_order_id INTEGER;
    v_product_id INTEGER;
    v_quantity INTEGER;
    v_price DECIMAL(10,2);
    v_product_name VARCHAR;
    v_available INTEGER;
    v_item_total DECIMAL(10,2);
    v_final_total DECIMAL(10,2) := 0;
    i INTEGER;
BEGIN
    BEGIN
        IF NOT EXISTS (SELECT 1 FROM users WHERE user_id = p_user_id) THEN
            RAISE EXCEPTION 'Пользователь с ID % не найден', p_user_id;
        END IF;

        IF array_length(p_product_ids, 1) != array_length(p_quantities, 1) THEN
            RAISE EXCEPTION 'Массивы продуктов и количеств не совпадают по длине';
        END IF;

        INSERT INTO orders (user_id, status, total_price)
        VALUES (p_user_id, 'pending', 0)
        RETURNING order_id INTO v_order_id;

        p_order_id := v_order_id;

        FOR i IN 1..array_length(p_product_ids, 1) LOOP
            v_product_id := p_product_ids[i];
            v_quantity := p_quantities[i];

            SELECT name, price, stock_quantity 
            INTO v_product_name, v_price, v_available
            FROM products 
            WHERE product_id = v_product_id;

            IF NOT FOUND THEN
                RAISE EXCEPTION 'Продукт с ID % не найден', v_product_id;
            END IF;

            IF v_available < v_quantity THEN
                RAISE EXCEPTION 'Недостаточно товара "%" (ID: %). Доступно: %, запрошено: %',
                    v_product_name, v_product_id, v_available, v_quantity;
            END IF;

            v_item_total := v_price * v_quantity;
            v_final_total := v_final_total + v_item_total;

            INSERT INTO order_items (order_id, product_id, quantity, price)
            VALUES (v_order_id, v_product_id, v_quantity, v_price);

            UPDATE products 
            SET stock_quantity = stock_quantity - v_quantity
            WHERE product_id = v_product_id;
        END LOOP;

        UPDATE orders 
        SET total_price = v_final_total
        WHERE order_id = v_order_id;

        p_total_price := v_final_total;
        p_message := 'Заказ успешно создан. ID заказа: ' || v_order_id;

        COMMIT;

    EXCEPTION
        WHEN OTHERS THEN
            ROLLBACK;
            p_order_id := 0;
            p_total_price := 0;
            p_message := 'Ошибка при создании заказа: ' || SQLERRM;
            RAISE NOTICE '%', p_message;
    END;
END;
$$;

-- 2 процедура - обновление статуса заказа с записью в историю
CREATE OR REPLACE PROCEDURE updateOrderStatus(
    p_order_id INTEGER,
    p_new_status VARCHAR,
    p_changed_by INTEGER
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_old_status VARCHAR;
BEGIN
   
    SELECT status INTO v_old_status
    FROM orders
    WHERE order_id = p_order_id;

    IF NOT FOUND THEN
        RAISE EXCEPTION 'Заказ с ID % не найден', p_order_id;
    END IF;

    IF v_old_status = p_new_status THEN
        RAISE NOTICE 'Статус заказа % уже установлен в %', p_order_id, p_new_status;
        RETURN;
    END IF;

    UPDATE orders 
    SET status = p_new_status
    WHERE order_id = p_order_id;

    INSERT INTO order_status_history (order_id, old_status, new_status, changed_by)
    VALUES (p_order_id, v_old_status, p_new_status, p_changed_by);

    COMMIT;
    
    RAISE NOTICE 'Статус заказа % изменён с % на %', p_order_id, v_old_status, p_new_status;
END;
$$;

-- 3 процедура - утверждение заказа менеджером
CREATE OR REPLACE PROCEDURE approveOrder(
    p_order_id INTEGER,
    p_manager_id INTEGER
)
LANGUAGE plpgsql
AS $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM users WHERE user_id = p_manager_id AND role = 'manager') THEN
        RAISE EXCEPTION 'Пользователь с ID % не является менеджером', p_manager_id;
    END IF;

    IF NOT EXISTS (SELECT 1 FROM orders WHERE order_id = p_order_id) THEN
        RAISE EXCEPTION 'Заказ с ID % не найден', p_order_id;
    END IF;

    CALL updateOrderStatus(p_order_id, 'completed', p_manager_id);

    INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
    VALUES ('order', p_order_id, 'update', p_manager_id);

    COMMIT;
    
    RAISE NOTICE 'Заказ % утверждён менеджером %', p_order_id, p_manager_id;
END;
$$;

-- 4 процедура - отмена заказа
CREATE OR REPLACE PROCEDURE cancelOrder(
    p_order_id INTEGER,
    p_user_id INTEGER
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_user_role VARCHAR;
    v_order_user_id INTEGER;
BEGIN
    SELECT role INTO v_user_role FROM users WHERE user_id = p_user_id;
    SELECT user_id INTO v_order_user_id FROM orders WHERE order_id = p_order_id;

    IF NOT FOUND THEN
        RAISE EXCEPTION 'Заказ с ID % не найден', p_order_id;
    END IF;

    IF v_user_role NOT IN ('admin', 'manager') AND v_order_user_id != p_user_id THEN
        RAISE EXCEPTION 'Недостаточно прав для отмены этого заказа';
    END IF;

    UPDATE products p
    SET stock_quantity = p.stock_quantity + oi.quantity
    FROM order_items oi
    WHERE oi.order_id = p_order_id 
      AND p.product_id = oi.product_id;

    CALL updateOrderStatus(p_order_id, 'canceled', p_user_id);

    INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
    VALUES ('order', p_order_id, 'update', p_user_id);

    COMMIT;
    
    RAISE NOTICE 'Заказ % отменён пользователем %', p_order_id, p_user_id;
END;
$$;

-- проверка
SELECT routine_name, routine_type 
FROM information_schema.routines 
WHERE routine_schema = 'public';
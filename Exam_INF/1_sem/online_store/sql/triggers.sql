-- Триггер для автоматического обновления order_date при изменении статуса заказа
CREATE OR REPLACE FUNCTION update_order_date_on_status_change()
RETURNS TRIGGER AS $$
BEGIN
    IF OLD.status IS NOT NULL AND NEW.status != OLD.status THEN
        NEW.order_date = CURRENT_TIMESTAMP;
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_update_order_date
    BEFORE UPDATE ON orders
    FOR EACH ROW
    EXECUTE FUNCTION update_order_date_on_status_change();

-- Триггер для учета изменений цен продуктов
-- При изменении цены продукта  обновляется total_price всех заказов с этим продуктом
CREATE OR REPLACE FUNCTION update_order_prices_on_product_price_change()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.price != OLD.price THEN
        UPDATE orders o
        SET total_price = (
            SELECT SUM(oi.quantity * 
                CASE 
                    WHEN oi.product_id = NEW.product_id THEN NEW.price
                    ELSE (SELECT price FROM products WHERE product_id = oi.product_id)
                END)
            FROM order_items oi
            WHERE oi.order_id = o.order_id
        )
        WHERE o.order_id IN (
            SELECT DISTINCT order_id 
            FROM order_items 
            WHERE product_id = NEW.product_id
        )
        AND o.status IN ('pending', 'completed');
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_update_order_prices
    AFTER UPDATE OF price ON products
    FOR EACH ROW
    EXECUTE FUNCTION update_order_prices_on_product_price_change();

-- Триггер аудита для истории статусов заказов
-- Автоматически записывает изменения статусов в order_status_history
CREATE OR REPLACE FUNCTION log_order_status_change()
RETURNS TRIGGER AS $$
BEGIN
    IF OLD.status IS NOT NULL AND NEW.status != OLD.status THEN
        INSERT INTO order_status_history (order_id, old_status, new_status, changed_by)
        VALUES (NEW.order_id, OLD.status, NEW.status, 
                CASE 
                    WHEN TG_OP = 'UPDATE' THEN 
                        COALESCE(current_setting('app.current_user_id', TRUE)::INTEGER, 0)
                    ELSE 0 
                END);
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_log_order_status
    AFTER UPDATE OF status ON orders
    FOR EACH ROW
    EXECUTE FUNCTION log_order_status_change();

-- Триггер аудита для логирования операций (audit_log)
-- Логирует изменения в таблицах: users, products, orders
CREATE OR REPLACE FUNCTION log_audit_event()
RETURNS TRIGGER AS $$
DECLARE
    v_entity_type VARCHAR;
    v_operation VARCHAR;
    v_user_id INTEGER;
    v_entity_id INTEGER;
BEGIN
    v_entity_type := CASE TG_TABLE_NAME
        WHEN 'users' THEN 'user'
        WHEN 'products' THEN 'product'
        WHEN 'orders' THEN 'order'
        ELSE TG_TABLE_NAME
    END;
    
    v_operation := LOWER(TG_OP);
    
    v_user_id := NULL;
    BEGIN
        v_user_id := current_setting('app.current_user_id', TRUE)::INTEGER;
    EXCEPTION WHEN OTHERS THEN
    END;
    
    IF TG_OP = 'DELETE' THEN
        CASE TG_TABLE_NAME
            WHEN 'users' THEN v_entity_id := OLD.user_id;
            WHEN 'products' THEN v_entity_id := OLD.product_id;
            WHEN 'orders' THEN v_entity_id := OLD.order_id;
            ELSE v_entity_id := 0;
        END CASE;
    ELSE
        CASE TG_TABLE_NAME
            WHEN 'users' THEN v_entity_id := NEW.user_id;
            WHEN 'products' THEN v_entity_id := NEW.product_id;
            WHEN 'orders' THEN v_entity_id := NEW.order_id;
            ELSE v_entity_id := 0;
        END CASE;
    END IF;

	
    INSERT INTO audit_log (entity_type, entity_id, operation, performed_by)
    VALUES (v_entity_type, v_entity_id, v_operation, v_user_id);
    
    RETURN COALESCE(NEW, OLD);
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_audit_users
    AFTER INSERT OR UPDATE OR DELETE ON users
    FOR EACH ROW
    EXECUTE FUNCTION log_audit_event();

CREATE TRIGGER trigger_audit_products
    AFTER INSERT OR UPDATE OR DELETE ON products
    FOR EACH ROW
    EXECUTE FUNCTION log_audit_event();

CREATE TRIGGER trigger_audit_orders
    AFTER INSERT OR UPDATE OR DELETE ON orders
    FOR EACH ROW
    EXECUTE FUNCTION log_audit_event();

-- Триггер для проверки доступности товара при добавлении в заказ
CREATE OR REPLACE FUNCTION check_product_availability()
RETURNS TRIGGER AS $$
DECLARE
    v_stock_quantity INTEGER;
BEGIN
    SELECT stock_quantity INTO v_stock_quantity
    FROM products
    WHERE product_id = NEW.product_id;
    
    IF v_stock_quantity < NEW.quantity THEN
        RAISE EXCEPTION 'Недостаточно товара (ID: %). Доступно: %, запрошено: %', 
            NEW.product_id, v_stock_quantity, NEW.quantity;
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_check_availability
    BEFORE INSERT ON order_items
    FOR EACH ROW
    EXECUTE FUNCTION check_product_availability();

-- Триггер для автоматического уменьшения количества товара на складе при добавлении в заказ
CREATE OR REPLACE FUNCTION decrease_stock_on_order_item()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE products
    SET stock_quantity = stock_quantity - NEW.quantity
    WHERE product_id = NEW.product_id;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_decrease_stock
    AFTER INSERT ON order_items
    FOR EACH ROW
    EXECUTE FUNCTION decrease_stock_on_order_item();

-- Триггер для увеличения количества товара при удалении из заказа
CREATE OR REPLACE FUNCTION increase_stock_on_order_item_delete()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE products
    SET stock_quantity = stock_quantity + OLD.quantity
    WHERE product_id = OLD.product_id;
    
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_increase_stock
    AFTER DELETE ON order_items
    FOR EACH ROW
    EXECUTE FUNCTION increase_stock_on_order_item_delete();



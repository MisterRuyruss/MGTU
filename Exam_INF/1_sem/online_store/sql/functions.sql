-- 1 функция - возвращает статус заказа по его id
CREATE OR REPLACE FUNCTION getOrderStatus(p_order_id INTEGER)
RETURNS VARCHAR AS $$
DECLARE
    v_status VARCHAR;
BEGIN
    SELECT status INTO v_status
    FROM orders
    WHERE order_id = p_order_id;
    
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Заказ с ID % не найден', p_order_id;
    END IF;
    
    RETURN v_status;
END;
$$ LANGUAGE plpgsql;

-- 2 функция - возвращает количество заказов для каждого пользователя
CREATE OR REPLACE FUNCTION getUserOrderCount()
RETURNS TABLE(user_id INTEGER, user_name VARCHAR, order_count BIGINT) AS $$
BEGIN
    RETURN QUERY
    SELECT 
        u.user_id,
        u.name AS user_name,
        COUNT(o.order_id) AS order_count
    FROM users u
    LEFT JOIN orders o ON u.user_id = o.user_id
    GROUP BY u.user_id, u.name
    ORDER BY order_count DESC;
END;
$$ LANGUAGE plpgsql;

-- 3 функция - возвращает общую сумму денег, потраченную пользователем
CREATE OR REPLACE FUNCTION getTotalSpentByUser(p_user_id INTEGER)
RETURNS DECIMAL AS $$
DECLARE
    v_total DECIMAL(10,2);
BEGIN
    SELECT COALESCE(SUM(total_price), 0) INTO v_total
    FROM orders
    WHERE user_id = p_user_id
      AND status = 'completed';
    
    RETURN v_total;
END;
$$ LANGUAGE plpgsql;

-- 4 функция - проверяет, возможен ли возврат заказа(30 дней прошло или нет)
CREATE OR REPLACE FUNCTION canReturnOrder(p_order_id INTEGER)
RETURNS BOOLEAN AS $$
DECLARE
    v_status VARCHAR;
    v_order_date TIMESTAMP;
    v_days_passed INTEGER;
BEGIN
    SELECT status, order_date INTO v_status, v_order_date
    FROM orders
    WHERE order_id = p_order_id;
    
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Заказ с ID % не найден', p_order_id;
    END IF;
   
    IF v_status != 'completed' THEN
        RETURN FALSE;
    END IF;
    
    v_days_passed := EXTRACT(DAY FROM (CURRENT_TIMESTAMP - v_order_date));
    
    IF v_days_passed > 30 THEN
        RETURN FALSE;
    END IF;
    
    RETURN TRUE;
END;
$$ LANGUAGE plpgsql;

-- 5 функция - возвращает полную историю изменения статусов выбранного заказа
CREATE OR REPLACE FUNCTION getOrderStatusHistory(p_order_id INTEGER)
RETURNS TABLE(
    history_id INTEGER,
    order_id INTEGER,
    old_status VARCHAR,
    new_status VARCHAR,
    changed_at TIMESTAMP,
    changed_by INTEGER,
    changed_by_name VARCHAR
) AS $$
BEGIN
    RETURN QUERY
    SELECT 
        h.history_id,
        h.order_id,
        h.old_status,
        h.new_status,
        h.changed_at,
        h.changed_by,
        u.name AS changed_by_name
    FROM order_status_history h
    LEFT JOIN users u ON h.changed_by = u.user_id
    WHERE h.order_id = p_order_id
    ORDER BY h.changed_at DESC;
END;
$$ LANGUAGE plpgsql;

-- 6 функция - возвращает список всех действий, выполненных указанным пользователем
CREATE OR REPLACE FUNCTION getAuditLogByUser(p_user_id INTEGER)
RETURNS TABLE(
    log_id INTEGER,
    entity_type VARCHAR,
    entity_id INTEGER,
    operation VARCHAR,
    performed_by INTEGER,
    performed_at TIMESTAMP,
    performer_name VARCHAR
) AS $$
BEGIN
    RETURN QUERY
    SELECT 
        a.log_id,
        a.entity_type,
        a.entity_id,
        a.operation,
        a.performed_by,
        a.performed_at,
        u.name AS performer_name
    FROM audit_log a
    LEFT JOIN users u ON a.performed_by = u.user_id
    WHERE a.performed_by = p_user_id
    ORDER BY a.performed_at DESC;
END;
$$ LANGUAGE plpgsql;
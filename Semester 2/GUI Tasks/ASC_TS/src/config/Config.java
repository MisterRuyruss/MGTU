package config;


public class Config {

    // Сколько дней работает симуляция
    public static int SIMULATION_DAYS = 7;

    // Начальное количество колонок
    public static int INITIAL_PUMPS = 3; 

    // Максимально допустимое количество колонок
    public static final int MAX_PUMPS = 12;

    // Скорость заправки (литров в минуту)
    public static final double DEFAULT_FLOW_RATE = 20.0;
    
    // Если ожидание в очереди превышает этот порог —
    // планируется открытие новой колонки
    public static final double QUEUE_THRESHOLD = 5.0; 

    // Через сколько минут после решения открывается колонка 
    public static final double EXPANSION_DELAY = 1440; 
    
    
    // Начальные запасы топлива
    public static final double INITIAL_AI92 = 6000.0;
    public static final double INITIAL_AI95 = 4000.0;
    public static final double INITIAL_DIESEL = 5000.0;

    // Максимальная вместимость резервуаров
    public static final double MAX_AI92 = 8000.0;
    public static final double MAX_AI95 = 6000.0;
    public static final double MAX_DIESEL = 7000.0;

    // Объём поставки топлива
    public static final double DELIVERY_AI92 = 4000.0;
    public static final double DELIVERY_AI95 = 3000.0;
    public static final double DELIVERY_DIESEL = 3500.0;

    // Интервал поставки (раз в сутки)
    public static final double DELIVERY_INTERVAL = 1440;

    // Чтобы при каждом запуске получались одинаковые результаты
    public static final long RANDOM_SEED = 42;

    // Интенсивность потока машин в разное время суток
    // час начала / час конца / машин в минуту
    public static final double[][] INTENSITY_BY_HOUR = {
        {0, 6, 0.05}, 
        {6, 10, 0.40}, 
        {10, 16, 0.20}, 
        {16, 20, 0.60}, 
        {20, 24, 0.10}
    };
}
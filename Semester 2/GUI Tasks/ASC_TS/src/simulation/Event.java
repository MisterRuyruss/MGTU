package simulation;

import model.Car;
import model.FuelPump;

// движение симуляции по времени от события к событию (приезд машины, конец заправки и т.д.)
public class Event implements Comparable<Event> {
    
    // типы событий
    public enum EventType { CAR_ARRIVAL, SERVICE_END, FUEL_DELIVERY, PUMP_OPEN }
    
    private EventType type;
    private double time;     // в какую минуту происходит
    private Car car;         // какая машина используется/0
    private FuelPump pump;   // какая колонка используется/0

    public Event(EventType type, double time, Car car, FuelPump pump) {
        this.type = type;
        this.time = time;
        this.car = car;
        this.pump = pump;
    }

    // доп конструкторы
    public Event(EventType type, double time, Car car) { this(type, time, car, null); }
    public Event(EventType type, double time) { this(type, time, null, null); }

    public EventType getType() { return type; }
    public double getTime() { return time; }
    public Car getCar() { return car; }
    public FuelPump getPump() { return pump; }

    // метот очереди/кто раньше тот и первыый
    @Override
    public int compareTo(Event other) {
        int timeCmp = Double.compare(this.time, other.time);
        if (timeCmp != 0) return timeCmp;
        // при одинаковом времени, сортируем по типу события 
        return this.type.ordinal() - other.type.ordinal();
    }
}
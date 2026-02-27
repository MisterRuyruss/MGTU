package simulation;

import model.*;
import config.Config;
import java.util.*;

public class GasStation {
    private List<FuelPump> pumps = new ArrayList<>();
    
    // очереди разделены по типам топлива
    private Map<FuelType, Queue<Car>> queues = new HashMap<>();
    
    // текущие запасы топлива
    private Map<FuelType, Double> reserves = new HashMap<>();

    public GasStation() {
        // создание пустых очередей
        for (FuelType type : FuelType.values()) {
            queues.put(type, new LinkedList<>());
        }
        
        // стартовый объём бенза
        reserves.put(FuelType.AI92, Config.INITIAL_AI92);
        reserves.put(FuelType.AI95, Config.INITIAL_AI95);
        reserves.put(FuelType.DIESEL, Config.INITIAL_DIESEL);

        // строим колонки и распределям топливо
        for (int i = 0; i < Config.INITIAL_PUMPS; i++) {
            addNewPump(0, FuelType.values()[i % FuelType.values().length]);
        }
    }

    // +1 колонка
    public void addNewPump(double time, FuelType type) {
        if (pumps.size() < Config.MAX_PUMPS) {
            pumps.add(new FuelPump(Config.DEFAULT_FLOW_RATE, (int)(time/1440), type));
        }
    }

    // поиск нужной колонки с нужным топливом
    public FuelPump findFreePump(FuelType type) {
        for (FuelPump p : pumps) {
            if (!p.isBusy() && p.getFuelType() == type) return p;
        }
        return null;
    }

    // проверка на наличия необходимого топлива
    public boolean hasFuel(FuelType type, double amount) {
        return reserves.get(type) >= amount;
    }

    // заправленный бенз вычитаем из запасов 
    public void spendFuel(FuelType type, double amount) {
        reserves.put(type, reserves.get(type) - amount);
    }

    // заправка бензовозом
    public void deliverFuel() {
        reserves.put(FuelType.AI92, Math.min(reserves.get(FuelType.AI92) + Config.DELIVERY_AI92, Config.MAX_AI92));
        reserves.put(FuelType.AI95, Math.min(reserves.get(FuelType.AI95) + Config.DELIVERY_AI95, Config.MAX_AI95));
        reserves.put(FuelType.DIESEL, Math.min(reserves.get(FuelType.DIESEL) + Config.DELIVERY_DIESEL, Config.MAX_DIESEL));
    }

    public List<FuelPump> getPumps() { return pumps; }
    public Queue<Car> getQueue(FuelType type) { return queues.get(type); }
    public double getReserve(FuelType type) { return reserves.get(type); }
    
    // все машины во всех очердях
    public int getTotalQueueSize() {
        return queues.values().stream().mapToInt(Collection::size).sum();
    }
}
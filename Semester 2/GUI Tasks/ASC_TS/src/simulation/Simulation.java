package simulation;

import model.*;
import config.Config;
import generator.CarGenerator;
import java.util.*;

public class Simulation {
    // очередь событий
    private PriorityQueue<Event> events = new PriorityQueue<>();
    
    private GasStation station = new GasStation();
    private CarGenerator gen = new CarGenerator(Config.RANDOM_SEED);
    private Statistics stats = new Statistics();
    
    private double currentTime = 0; // текущее время
    private boolean expansionPending = false; //ждём ли новую колонку

    public Statistics run() {
        // планирование приезд первой машины и бензовоза
        events.add(new Event(Event.EventType.CAR_ARRIVAL, gen.generateNextInterval(0)));
        events.add(new Event(Event.EventType.FUEL_DELIVERY, Config.DELIVERY_INTERVAL));

        double lastVizTime = 0; // таймер для вывода

        // пока есть события и время не вышло
        while (!events.isEmpty() && currentTime < Config.SIMULATION_DAYS * 1440) {
            Event e = events.poll(); // берем событие
            currentTime = e.getTime(); // мотаем время

            // пишем данные для графиков
            stats.recordState(
                currentTime, station.getTotalQueueSize(),
                station.getReserve(FuelType.AI92), station.getReserve(FuelType.AI95), station.getReserve(FuelType.DIESEL)
            );

            // выводим раз в 4 часа игрового времени в консоль
            if (currentTime - lastVizTime > 240) {
                System.out.printf("[День %d, %02d:%02d] Топливо: 92=%.0f, 95=%.0f, ДТ=%.0f | Очередь: %d\n", 
                    (int)(currentTime/1440) + 1, (int)((currentTime%1440)/60), (int)(currentTime%60),
                    station.getReserve(FuelType.AI92), station.getReserve(FuelType.AI95), 
                    station.getReserve(FuelType.DIESEL), station.getTotalQueueSize());
                lastVizTime = currentTime;
            }

            // обработка события
            switch (e.getType()) {
                case CAR_ARRIVAL: handleArrival(); break;
                case SERVICE_END: handleEnd(e); break;
                case FUEL_DELIVERY:
                    station.deliverFuel();
                    // планирование бензовоза
                    events.add(new Event(Event.EventType.FUEL_DELIVERY, currentTime + Config.DELIVERY_INTERVAL));
                    break;
                case PUMP_OPEN:
                    // смотрим где большая очередь
                    FuelType bestType = Arrays.stream(FuelType.values())
                        .max(Comparator.comparingInt(t -> station.getQueue(t).size())).orElse(FuelType.AI92);
                    station.addNewPump(currentTime, bestType);
                    expansionPending = false; 
                    break;
            }
        }
        return stats; // возвращаем статистику
    }

    // приехала новая машина
    private void handleArrival() {
        Car car = gen.generateCar(currentTime);
        stats.recordArrival();
        
        // планы на вторую машину
        events.add(new Event(Event.EventType.CAR_ARRIVAL, currentTime + gen.generateNextInterval(currentTime)));

        // проверка на наличие бензина
        if (!station.hasFuel(car.getFuelType(), car.getDesiredFuel())) {
            stats.recordRefusal(); // отказ
            return;
        }

        FuelPump p = station.findFreePump(car.getFuelType());
        if (p != null) {
            // если колонка свободна заправляем
            p.startService(car, currentTime);
            events.add(new Event(Event.EventType.SERVICE_END, car.getEndServiceTime(), car, p));
        } else {
            // если занята ставим в очередь
            station.getQueue(car.getFuelType()).add(car);
            stats.updateMaxQueue(station.getTotalQueueSize());
        }
    }

    // машина заправилась и уезжает
    private void handleEnd(Event e) {
        FuelPump p = e.getPump();
        Car finishedCar = p.release();
        
        // списываем бенз по факту окончания заправки
        station.spendFuel(finishedCar.getFuelType(), finishedCar.getDesiredFuel());
        stats.recordService(finishedCar);
        stats.updatePumpBusy(p.getId(), finishedCar.getEndServiceTime() - finishedCar.getStartServiceTime());

        // проверка есть ли кто то на заправке
        Queue<Car> q = station.getQueue(p.getFuelType());
        if (!q.isEmpty()) {
            Car next = q.poll();
            
            // если машина ждала очень долго строим новую колонку
            if (next.getWaitTime() > Config.QUEUE_THRESHOLD && !expansionPending) {
                events.add(new Event(Event.EventType.PUMP_OPEN, currentTime + Config.EXPANSION_DELAY));
                expansionPending = true;
            }
            
            // заправляем машину из очереди
            p.startService(next, currentTime);
            events.add(new Event(Event.EventType.SERVICE_END, next.getEndServiceTime(), next, p));
        }
    }
}
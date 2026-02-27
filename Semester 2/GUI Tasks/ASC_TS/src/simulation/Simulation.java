package simulation;

import model.*;
import config.Config;
import generator.CarGenerator;
import java.util.*;

public class Simulation {
    private PriorityQueue<Event> events = new PriorityQueue<>();
    private GasStation station = new GasStation();
    private CarGenerator gen = new CarGenerator(Config.RANDOM_SEED);
    private Statistics stats = new Statistics();
    
    private double currentTime = 0;
    private boolean expansionPending = false;
    private int deliveriesCount = 0; 

    public Statistics run() {
        events.add(new Event(Event.EventType.CAR_ARRIVAL, gen.generateNextInterval(0)));
        events.add(new Event(Event.EventType.FUEL_DELIVERY, Config.DELIVERY_INTERVAL));

        double lastVizTime = 0;

        while (!events.isEmpty() && currentTime < Config.SIMULATION_DAYS * 1440) {
            Event e = events.poll();
            currentTime = e.getTime();

            stats.recordState(
                currentTime, station.getTotalQueueSize(),
                station.getReserve(FuelType.AI92), station.getReserve(FuelType.AI95), station.getReserve(FuelType.DIESEL)
            );

            if (currentTime - lastVizTime > 240) {
                System.out.printf("[День %d, %02d:%02d] Топливо: 92=%.0f, 95=%.0f, ДТ=%.0f | Очередь: %d\n", 
                    (int)(currentTime/1440) + 1, (int)((currentTime%1440)/60), (int)(currentTime%60),
                    station.getReserve(FuelType.AI92), station.getReserve(FuelType.AI95), 
                    station.getReserve(FuelType.DIESEL), station.getTotalQueueSize());
                lastVizTime = currentTime;
            }

            switch (e.getType()) {
                case CAR_ARRIVAL: handleArrival(); break;
                case SERVICE_END: handleEnd(e); break;
                case FUEL_DELIVERY:
                    station.deliverFuel();
                    deliveriesCount++; 
                    events.add(new Event(Event.EventType.FUEL_DELIVERY, currentTime + Config.DELIVERY_INTERVAL));
                    break;
                case PUMP_OPEN:
                    FuelType bestType = Arrays.stream(FuelType.values())
                        .max(Comparator.comparingInt(t -> station.getQueue(t).size())).orElse(FuelType.AI92);
                    station.addNewPump(currentTime, bestType);
                    expansionPending = false;
                    break;
            }
        }
        
        stats.printReport(currentTime, Config.INITIAL_PUMPS, station.getPumps().size(), deliveriesCount);
        
        return stats;
    }

    private void handleArrival() {
        Car car = gen.generateCar(currentTime);
        stats.recordArrival();
        events.add(new Event(Event.EventType.CAR_ARRIVAL, currentTime + gen.generateNextInterval(currentTime)));

        if (!station.hasFuel(car.getFuelType(), car.getDesiredFuel())) {
            stats.recordRefusal();
            return;
        }

        FuelPump p = station.findFreePump(car.getFuelType());
        if (p != null) {
            p.startService(car, currentTime);
            events.add(new Event(Event.EventType.SERVICE_END, car.getEndServiceTime(), car, p));
        } else {
            station.getQueue(car.getFuelType()).add(car);
            stats.updateMaxQueue(station.getTotalQueueSize());
        }
    }

    private void handleEnd(Event e) {
        FuelPump p = e.getPump();
        Car finishedCar = p.release();
        
        station.spendFuel(finishedCar.getFuelType(), finishedCar.getDesiredFuel());
        
        double serviceDuration = finishedCar.getEndServiceTime() - finishedCar.getStartServiceTime();
        stats.recordService(finishedCar, serviceDuration);
        stats.updatePumpBusy(p.getId(), serviceDuration);

        Queue<Car> q = station.getQueue(p.getFuelType());
        if (!q.isEmpty()) {
            Car next = q.poll();
            
            // если ожидание долгое - новая колонка
            if (next.getWaitTime() > Config.QUEUE_THRESHOLD && !expansionPending) {
                events.add(new Event(Event.EventType.PUMP_OPEN, currentTime + Config.EXPANSION_DELAY));
                expansionPending = true;
            }
            p.startService(next, currentTime);
            events.add(new Event(Event.EventType.SERVICE_END, next.getEndServiceTime(), next, p));
        }
    }
}
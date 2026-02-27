package simulation;

import model.*;
import java.util.*;


public class Statistics {
    
    public int arrivals = 0;
    public int serviced = 0;
    public int refused = 0;

    private List<Double> waitTimes = new ArrayList<>();    // время ожидания в очереди
    private List<Double> serviceTimes = new ArrayList<>(); // время самой заправки
    private List<Double> systemTimes = new ArrayList<>();  // общее время (очередь + заправка)
    
    private Map<Integer, Double> busyTimes = new HashMap<>(); // Сколько минут работала каждая колонка
    
    private double maxQueue = 0;
    private double minTotalFuel = Double.MAX_VALUE; // минимальный зафиксированный остаток
    private double sumTotalFuel = 0; // сумма всех замеров топлива 
    private int fuelMeasurements = 0; // сколько раз мерили топливо

    private List<Double> timeHistory = new ArrayList<>();
    private List<Integer> queueHistory = new ArrayList<>();
    private List<Double> fuel92History = new ArrayList<>();
    private List<Double> fuel95History = new ArrayList<>();
    private List<Double> fuelDieselHistory = new ArrayList<>();

    public void recordArrival() { arrivals++; }
    public void recordRefusal() { refused++; }
    
    public void recordService(Car car, double serviceDuration) {
        serviced++;
        waitTimes.add(car.getWaitTime());
        serviceTimes.add(serviceDuration);
        systemTimes.add(car.getWaitTime() + serviceDuration); // Время в системе
    }
    public void updatePumpBusy(int id, double time) {
        busyTimes.put(id, busyTimes.getOrDefault(id, 0.0) + time);
    }
    
    public void updateMaxQueue(int size) { 
        if(size > maxQueue) maxQueue = size; 
    }

   
    public void recordState(double time, int queueSize, double f92, double f95, double fdiesel) {
        timeHistory.add(time);
        queueHistory.add(queueSize);
        fuel92History.add(f92);
        fuel95History.add(f95);
        fuelDieselHistory.add(fdiesel);
        
        // общий остаток топлива
        double totalFuel = f92 + f95 + fdiesel;
        if (totalFuel < minTotalFuel) minTotalFuel = totalFuel;
        sumTotalFuel += totalFuel;
        fuelMeasurements++;
    }

    // расчёт среднего и макс значения
    private double getAvg(List<Double> list) {
        return list.stream().mapToDouble(d -> d).average().orElse(0.0);
    }
    
    private double getMax(List<Double> list) {
        return list.stream().mapToDouble(d -> d).max().orElse(0.0);
    }

    public void printReport(double simulationTime, int startPumps, int endPumps, int deliveries) {
        System.out.println("\n==========================================");
        System.out.println("       ИТОГОВЫЙ ОТЧЕТ СИМУЛЯЦИИ");
        System.out.println("==========================================");
        
        System.out.printf("Всего автомобилей:       %d\n", arrivals);
        System.out.printf("Обслужено:               %d\n", serviced);
        System.out.printf("Отказов (нет топлива):   %d\n", refused);
        System.out.println("------------------------------------------");
        
        System.out.printf("Среднее ожидание:        %.2f мин\n", getAvg(waitTimes));
        System.out.printf("Макс ожидание:           %.2f мин\n", getMax(waitTimes));
        System.out.printf("Среднее время обсл:      %.2f мин\n", getAvg(serviceTimes));
        System.out.printf("Среднее время в системе: %.2f мин\n", getAvg(systemTimes));
        System.out.println("------------------------------------------");
        
        System.out.printf("Макс длина очереди:      %.0f\n", maxQueue);
        System.out.printf("Колонок:                 %d -> %d (Добавлено: %d)\n", startPumps, endPumps, (endPumps - startPumps));
        System.out.printf("Доставок топлива:        %d\n", deliveries);
        
        System.out.printf("Мин остаток топлива:     %.1f л\n", minTotalFuel);
        System.out.printf("Средний остаток:         %.1f л\n", (fuelMeasurements > 0 ? sumTotalFuel / fuelMeasurements : 0));
        System.out.println("------------------------------------------");
        
        System.out.println("Загрузка колонок (процент времени работы):");
       
        List<Integer> pumpIds = new ArrayList<>(busyTimes.keySet());
        Collections.sort(pumpIds);
        
        for (int id : pumpIds) {
            double busyPercent = (busyTimes.get(id) / simulationTime) * 100.0;
            System.out.printf("  Колонка #%d: %.1f%%\n", id, busyPercent);
        }
        System.out.println("==========================================\n");
    }

    public List<Double> getTimeHistory() { return timeHistory; }
    public List<Integer> getQueueHistory() { return queueHistory; }
    public List<Double> getFuel92History() { return fuel92History; }
    public List<Double> getFuel95History() { return fuel95History; }
    public List<Double> getFuelDieselHistory() { return fuelDieselHistory; }
}
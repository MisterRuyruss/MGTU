package simulation;

import model.*;
import java.util.*;

public class Statistics {
    public int arrivals = 0, serviced = 0, refused = 0; 
    private List<Double> waits = new ArrayList<>();     
    private Map<Integer, Double> busyTimes = new HashMap<>(); 
    private double maxQueue = 0;                     

    // списки для графиков
    private List<Double> timeHistory = new ArrayList<>();
    private List<Integer> queueHistory = new ArrayList<>();
    private List<Double> fuel92History = new ArrayList<>();
    private List<Double> fuel95History = new ArrayList<>();
    private List<Double> fuelDieselHistory = new ArrayList<>();

    public void recordArrival() { arrivals++; }
    public void recordRefusal() { refused++; }
    
    public void recordService(Car car) {
        serviced++;
        waits.add(car.getWaitTime());
    }
    
    public void updatePumpBusy(int id, double time) {
        busyTimes.put(id, busyTimes.getOrDefault(id, 0.0) + time);
    }
    
    public void updateMaxQueue(int size) { 
        if(size > maxQueue) maxQueue = size; 
    }

    // запись состояния в текущую минуту (вызывается на каждом шаге)
    public void recordState(double time, int queueSize, double f92, double f95, double fdiesel) {
        timeHistory.add(time);
        queueHistory.add(queueSize);
        fuel92History.add(f92);
        fuel95History.add(f95);
        fuelDieselHistory.add(fdiesel);
    }

    // среднее время ожидания
    public double getAvgWait() { 
        return waits.stream().mapToDouble(d->d).average().orElse(0); 
    }

    // вывод итогов в консоль
    public void printShortReport(int initialPumps) {
        System.out.printf("Колонок: %d | Обслужено: %d | Отказы: %d | Ср. Ожидание: %.2f мин | Очередь макс: %.0f\n",
                initialPumps, serviced, refused, getAvgWait(), maxQueue);
    }

    // геттеры для графиков
    public List<Double> getTimeHistory() { return timeHistory; }
    public List<Integer> getQueueHistory() { return queueHistory; }
    public List<Double> getFuel92History() { return fuel92History; }
    public List<Double> getFuel95History() { return fuel95History; }
    public List<Double> getFuelDieselHistory() { return fuelDieselHistory; }
}
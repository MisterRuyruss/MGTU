package model;

import java.util.Random;

// данные о машине и времени её обслуживания.
public class Car {

    private static int nextId = 1; // автоматическая нумерация машин

    private int id;
    private FuelType fuelType;
    private double desiredFuel;
    private double arrivalTime;
    private double startServiceTime;
    private double endServiceTime;

    public Car(double arrivalTime, Random random) {
        this.id = nextId++;
        this.arrivalTime = arrivalTime;

        // рандом топлива
        this.fuelType = FuelType.values()[random.nextInt(FuelType.values().length)];
        
        // рандом бак
        double tankCapacity = (4 + random.nextInt(5)) * 10; // 40–80 литров
        double currentFuel = tankCapacity * (0.1 + random.nextDouble() * 0.4);

        // проверка на свободное место в баке
        this.desiredFuel = Math.min(tankCapacity - currentFuel,
                                    5 + random.nextDouble() * 40);
    }

    public static void resetIdCounter() { nextId = 1; }

    public int getId() { return id; }
    public FuelType getFuelType() { return fuelType; }
    public double getDesiredFuel() { return desiredFuel; }
    public double getArrivalTime() { return arrivalTime; }

    public double getStartServiceTime() { return startServiceTime; }
    public void setStartServiceTime(double t) { this.startServiceTime = t; }

    public double getEndServiceTime() { return endServiceTime; }
    public void setEndServiceTime(double t) { this.endServiceTime = t; }

    // время в очереди
    public double getWaitTime() {
        return startServiceTime - arrivalTime;
    }
}
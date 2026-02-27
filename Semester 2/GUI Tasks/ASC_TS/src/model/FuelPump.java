package model;


public class FuelPump {
    private static int nextId = 1; // счётчик для колонки
    private int id;
    private FuelType fuelType;     // тип бенза
    private double flowRate;       // скорость заправки
    private boolean busy;          // занята?
    private Car currentCar;        // какая машина щас заправляется
    private int openedAtDay;       // в какой день открыли

    public FuelPump(double flowRate, int openedAtDay, FuelType type) {
        this.id = nextId++;
        this.flowRate = flowRate;
        this.openedAtDay = openedAtDay;
        this.fuelType = type;
    }

    // начало заправки
    public void startService(Car car, double currentTime) {
        this.busy = true;
        this.currentCar = car;
        car.setStartServiceTime(currentTime);
        
        // время когда машина уедет = текущее время + сколько нужно залить / скорость
        car.setEndServiceTime(currentTime + car.getDesiredFuel() / flowRate);
    }

    // завершение заправки
    public Car release() {
        Car car = currentCar;
        this.busy = false;
        this.currentCar = null;
        return car;
    }

    public static void resetIdCounter() { nextId = 1; }
    public int getId() { return id; }
    public boolean isBusy() { return busy; }
    public FuelType getFuelType() { return fuelType; }
    public int getOpenedAtDay() { return openedAtDay; }
}
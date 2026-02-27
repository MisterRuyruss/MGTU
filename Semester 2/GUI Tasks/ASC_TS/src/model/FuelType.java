package model;

public enum FuelType {
    AI92("АИ-92"), AI95("АИ-95"), DIESEL("ДТ");
    
    private final String name;
    
    FuelType(String name) { this.name = name; }
    public String getName() { return name; }
}
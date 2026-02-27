import simulation.*;
import config.Config;
import model.*;

public class Main {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("   АЗС СИМУЛЯЦИЯ ");
        System.out.println("=================================================\n");

        // Запуск с 3 колонками для наглядной демонстрации дефицитов и очередей
        Config.INITIAL_PUMPS = 3;
        
        Car.resetIdCounter();
        FuelPump.resetIdCounter();

        Simulation sim = new Simulation();
        Statistics stats = sim.run();

        System.out.println("\n--- ИТОГИ СИМУЛЯЦИИ ---");
        stats.printShortReport(Config.INITIAL_PUMPS);

        System.out.println("\nОткрытие графиков");
        ChartGenerator.showCharts(stats);
    }
}
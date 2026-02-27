import simulation.*;
import config.Config;
import model.*;

public class Main {
    public static void main(String[] args) {
        System.out.println("=================================================");
        System.out.println("   АЗС СИМУЛЯЦИЯ ");
        System.out.println("=================================================\n");

        
        Config.INITIAL_PUMPS = 3;
        
        Car.resetIdCounter();
        FuelPump.resetIdCounter();

        Simulation sim = new Simulation();
        Statistics stats = sim.run();


        ChartGenerator.showCharts(stats);
    }
}
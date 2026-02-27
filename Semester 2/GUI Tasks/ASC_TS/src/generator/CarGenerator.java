package generator;

import model.Car;
import config.Config;
import java.util.Random;


public class CarGenerator {

    private final Random random;

    public CarGenerator(long seed) {
        this.random = new Random(seed);
    }

    // Интесивность в зависимости от времени
    private double getIntensity(double timeMinutes) {
        double hour = (timeMinutes / 60.0) % 24.0;

        for (double[] period : Config.INTENSITY_BY_HOUR) {
            if (hour >= period[0] && hour < period[1]) {
                return period[2];
            }
        }
        return Config.INTENSITY_BY_HOUR[0][2];
    }

    // Интервал до след машины
    public double generateNextInterval(double currentTimeMinutes) {
        double lambda = getIntensity(currentTimeMinutes);
        if (lambda <= 0) return Double.POSITIVE_INFINITY;

        double u = random.nextDouble();
        if (u == 0.0) u = Double.MIN_VALUE;

        return -Math.log(u) / lambda;
    }

    // новая машина
    public Car generateCar(double arrivalTimeMinutes) {
        return new Car(arrivalTimeMinutes, random);
    }
}
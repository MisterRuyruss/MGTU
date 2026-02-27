package simulation;

import org.knowm.xchart.*;
import org.knowm.xchart.style.markers.None;

public class ChartGenerator {

    public static void showCharts(Statistics stats) {
        if (stats.getTimeHistory().isEmpty()) return;

        // график очереди
        XYChart queueChart = new XYChartBuilder()
                .width(800).height(500)
                .title("Длина очереди во времени")
                .xAxisTitle("Время (мин)")
                .yAxisTitle("Размер очереди").build();

        queueChart.addSeries("Очередь", stats.getTimeHistory(), stats.getQueueHistory()).setMarker(new None());
        new SwingWrapper<>(queueChart).displayChart();

        // график топлива
        XYChart fuelChart = new XYChartBuilder()
                .width(800).height(500)
                .title("Остаток топлива во времени")
                .xAxisTitle("Время (мин)")
                .yAxisTitle("Литры").build();

        fuelChart.addSeries("АИ-92", stats.getTimeHistory(), stats.getFuel92History()).setMarker(new None());
        fuelChart.addSeries("АИ-95", stats.getTimeHistory(), stats.getFuel95History()).setMarker(new None());
        fuelChart.addSeries("ДТ", stats.getTimeHistory(), stats.getFuelDieselHistory()).setMarker(new None());

        new SwingWrapper<>(fuelChart).displayChart();
    }
}
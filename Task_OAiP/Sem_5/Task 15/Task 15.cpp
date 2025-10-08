// Task 15.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <random>
#include <cmath>

using namespace std;

int main() {
    vector<vector<double>> temp(5, vector<double>(30));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 30; j++) {
            temp[i][j] = (rand() % 400 + 500) / 10.0; 
        }
    }

    vector<vector<double>> pressure(5, vector<double>(100));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 100; j++) {
            pressure[i][j] = (rand() % 600 + 800) / 10.0; 
        }
    }

    // Средняя температура для каждого канала
    vector<double> cr_temp(5);
    transform(temp.begin(), temp.end(), cr_temp.begin(),
        [](const vector<double>& chan) {
            return accumulate(chan.begin(), chan.end(), 0.0) / chan.size();
        });

    cout << "Средние температуры по каналам: " << endl;
    for_each(cr_temp.begin(), cr_temp.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    // Минимальное и максимальное давление для каждого канала
    vector<pair<double, double>> mima_pressures(5);
    transform(pressure.begin(), pressure.end(), mima_pressures.begin(),
        [](const vector<double>& channel) {
            auto mima = minmax_element(channel.begin(), channel.end());
            return make_pair(*mima.first, *mima.second);
        });

    cout << "Минимальное и максимальное давление по каналам: " << endl;
    for_each(mima_pressures.begin(), mima_pressures.end(), [](const pair<double, double>& p) {
        cout << "(" << p.first << ", " << p.second << ") ";
        });
    cout << endl;

    // Разности между соседними измерениями для температуры
    vector<vector<double>> temp_ras(5);
    for (int i = 0; i < 5; i++) {
        temp_ras[i].resize(temp[i].size() - 1);
        transform(temp[i].begin(), temp[i].end() - 1, temp[i].begin() + 1, temp_ras[i].begin(),
            [](double a, double b) { return b - a; });
    }

    cout << "Разности температур" << endl;
    for_each(temp_ras[0].begin(), temp_ras[0].begin() + min(5, (int)temp_ras[0].size()),
        [](double x) { cout << x << " "; });
    cout << endl;

    // Канал с наибольшей вариацией температуры
    vector<double> temp1(5);
    transform(temp.begin(), temp.end(), temp1.begin(),
        [](const vector<double>& chan) {
            auto mima = minmax_element(chan.begin(), chan.end());
            return *mima.second - *mima.first;
        });

    auto max_1 = max_element(temp1.begin(), temp1.end());
    int max_1_chan = distance(temp1.begin(), max_1);
    cout << "Канал с наибольшей вариацией температуры: " << max_1_chan
        << " (" << *max_1 << ")" << endl;

    // Нормализация давления для каждого канала
    vector<vector<double>> normal_pressure(5);
    for (int i = 0; i < 5; i++) {
        normal_pressure[i].resize(pressure[i].size());
        double min_p = mima_pressures[i].first;
        double max_p = mima_pressures[i].second;

        transform(pressure[i].begin(), pressure[i].end(), normal_pressure[i].begin(),
            [min_p, max_p](double value) {
                return (value - min_p) / (max_p - min_p);
            });
    }

    // Вывод пиковых значений (> 0.9) после нормализации
    cout << "Пиковые значения давления (> 0.9): " << endl;
    for_each(normal_pressure.begin(), normal_pressure.end(),
        [](const vector<double>& channel) {
            for_each(channel.begin(), channel.end(),
                [](double value) {
                    if (value > 0.9) {
                        cout << value << " ";
                    }
                });
        });
    cout << endl;

    // Сортировка средних температур и поиск канала с максимальной температурой
    vector<pair<double, int>> temp_index(5);
    for (int i = 0; i < 5; i++) {
        temp_index[i] = make_pair(cr_temp[i], i);
    }

    sort(temp_index.begin(), temp_index.end(),
        [](const pair<double, int>& a, const pair<double, int>& b) {
            return a.first > b.first;
        });

    cout << "Каналы по убыванию средней температуры: " << endl;
    for_each(temp_index.begin(), temp_index.end(),
        [](const pair<double, int>& p) {
            cout << "Канал " << p.second << ": " << p.first << "°C" << endl;
        });

    cout << "Канал с максимальной средней температурой: " << temp_index[0].second
        << " (" << temp_index[0].first << "°C)" << endl;
}
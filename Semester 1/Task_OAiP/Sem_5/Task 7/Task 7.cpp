// Task 7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <random>

using namespace std;

int main() {
    vector<double> temps(30);

    for (int i = 0; i < 30; i++) {
        temps[i] = (rand() % 1201 - 600) / 10.0; 
    }

    cout << "Исходные температуры: " << endl;
    for_each(temps.begin(), temps.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    auto r50 = remove_if(temps.begin(), temps.end(), [](double x) {return x < -50 || x > 50; });
    temps.erase(r50, temps.end());

    cout << "После удаления вне [-50, 50]: " << endl;
    for_each(temps.begin(), temps.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    auto mima = minmax_element(temps.begin(), temps.end());
    double cr = accumulate(temps.begin(), temps.end(), 0.0) / temps.size();

    cout << "Минимальная температура: " << *mima.first << endl;
    cout << "Максимальная температура: " << *mima.second << endl;
    cout << "Средняя температура: " << cr << endl;

    
    bool h10 = all_of(temps.begin(), temps.end(), [](double x) {return x > -10;});

    if (h10) {
        cout << "Все температуры превышают -10 градусов" << endl;
    }
    else {
        cout << "Не все температуры превышают -10 градусов" << endl;
    }
}
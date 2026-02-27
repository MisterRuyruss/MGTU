// Task 10.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <random>

using namespace std;

int main()
{
    vector<double> energy(30);

    for (int i = 0; i < 30; i++) {
        energy[i] = (rand() % 500 + 100) / 1.0; 
    }

    cout << "Потребление за 30 дней: " << endl;
    for_each(energy.begin(), energy.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    
    double total_energy = accumulate(energy.begin(), energy.end(), 0.0);
    cout << "Общее потребление за месяц: " << total_energy << " кВт·ч" << endl;

    
    double cr = total_energy / energy.size();
    cout << "Среднесуточное потребление: " << cr << " кВт·ч" << endl;

    
    auto mima_energy = minmax_element(energy.begin(), energy.end());
    cout << "Минимальное потребление: " << *mima_energy.first << " кВт·ч" << endl;
    cout << "Максимальное потребление: " << *mima_energy.second << " кВт·ч" << endl;

    
    double t = cr * 1.2;
    vector<double> peak_days;
    copy_if(energy.begin(), energy.end(), back_inserter(peak_days), [t](double x) {return x > t;});

    cout << "Порог пиковых дней: " << t << " кВт·ч" << endl;
    cout << "Пиковые дни: " << endl;
    for_each(peak_days.begin(), peak_days.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

}
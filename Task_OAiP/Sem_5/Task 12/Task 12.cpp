// Task 12.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <random>
#include <cmath>

using namespace std;

int main()
{
    vector<double> fuel_levels(50);

    for (int i = 0; i < 50; i++) {
        fuel_levels[i] = (rand() % 800 + 200) / 10.0; 
    }

    cout << "Уровни топлива: " << endl;
    for_each(fuel_levels.begin(), fuel_levels.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    auto mima_fuel = minmax_element(fuel_levels.begin(), fuel_levels.end());
    cout << "Минимальный уровень: " << *mima_fuel.first << endl;
    

    double t = *mima_fuel.second * 0.05;
    bool never = all_of(fuel_levels.begin(), fuel_levels.end(), [t](double x) {return x >= t;});
    if (never) {
        cout << "Никогда не падал ниже 5% от максимума" << endl;

    }
    else {
        cout << "Падал ниже 5% от максимума" << endl;

    }
    
    
    double crf = fuel_levels.front() - fuel_levels.back();
    cout << "Среднее потребление: " << crf << " за период" << endl;

    
    vector<double> sorted_fuel = fuel_levels;
    sort(sorted_fuel.begin(), sorted_fuel.end());
    double median_fuel;
    median_fuel = (sorted_fuel[sorted_fuel.size() / 2 - 1] + sorted_fuel[sorted_fuel.size() / 2]) / 2.0;
    cout << "Медиана уровня топлива: " << median_fuel << endl << endl;

    
}
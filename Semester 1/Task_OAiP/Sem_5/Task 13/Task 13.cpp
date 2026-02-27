// Task 13.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
    vector<double> davl(50);

    for (int i = 0; i < 50; i++) {
        davl[i] = (rand() % 100 + 10) / 10.0; 
    }

    cout << "Исходные измерения давления: " << endl;
    for_each(davl.begin(), davl.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    auto rem = remove_if(davl.begin(), davl.end(), [](double x) {return x < 0 || x > 12; });
    davl.erase(rem,davl.end());

    cout << "После удаления ошибок: " << endl;
    for_each(davl.begin(), davl.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    
    double cr = accumulate(davl.begin(), davl.end(), 0.0) / davl.size();
    cout << "Среднее давление: " << cr << " бар" << endl;

    
    auto mima_davl = minmax_element(davl.begin(), davl.end());
    cout << "Минимальное давление: " << *mima_davl.first << " бар" << endl;
    cout << "Максимальное давление: " << *mima_davl.second << " бар" << endl;

    
    cout << "Значения превышающие 8 бар: " << endl;
    for_each(davl.begin(), davl.end(), [](double x) {
        if (x > 8) {
            cout << x << " ";
        }
        });
    cout << endl << endl;

    

}
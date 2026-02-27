// Task 9.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <random>

using namespace std;

int main()
{
    
    vector<double> powers(20);

    for (int i = 0; i < 20; i++) {
        powers[i] = (rand() % 1200 - 100) / 10.0; // от -10.0 до 110.0
    }

    cout << "Исходные измерения мощности: " << endl;
    for_each(powers.begin(), powers.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    auto rem = remove_if(powers.begin(), powers.end(), [](double x) {return x < 0; });
    powers.erase(rem, powers.end());

    cout << "После удаления ошибок: " << endl;
    for_each(powers.begin(), powers.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    
    auto mima_power = minmax_element(powers.begin(), powers.end());
    double cr_power = accumulate(powers.begin(), powers.end(), 0.0) / powers.size();

    cout << "Минимальная мощность: " << *mima_power.first << " кВт" << endl;
    cout << "Максимальная мощность: " << *mima_power.second << " кВт" << endl;
    cout << "Средняя мощность: " << cr_power << " кВт" << endl;

    
    bool ifrn = all_of(powers.begin(), powers.end(), [](double x) {return x >= 10 && x <= 90;});
    if (ifrn) {
        cout << "Двигатель всегда работал в пределах 10-90 кВт" << endl;
    }
    else {
        cout << "Двигатель не всегда работал в пределах 10-90 кВт" << endl;
    }
    
    
    sort(powers.begin(), powers.end());
    double med_power;
    med_power = (powers[powers.size() / 2 - 1] + powers[powers.size() / 2]) / 2.0;
 
    cout << "Медиана: " << med_power << " кВт" << endl << endl;

    
}


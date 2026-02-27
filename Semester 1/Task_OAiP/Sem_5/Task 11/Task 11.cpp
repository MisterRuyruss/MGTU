// Task 11.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <random>
#include <cmath>

using namespace std;

int main() {
    vector<double> vibrations(50);

    for (int i = 0; i < 50; i++) {
        vibrations[i] = (rand() % 200) / 100.0; 
    }

    cout << "Исходные амплитуды вибрации: " << endl;
    for_each(vibrations.begin(), vibrations.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    auto vib = remove_if(vibrations.begin(), vibrations.end(), [](double x) {return x < 0.1; });
    vibrations.erase(vib, vibrations.end());
   

    cout << "После удаления шумов: " << endl;
    for_each(vibrations.begin(), vibrations.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    double cr = accumulate(vibrations.begin(), vibrations.end(), 0.0) / vibrations.size();
    cout << "Средняя амплитуда: " << cr << endl;
    double ma_vibration = *max_element(vibrations.begin(), vibrations.end());
    cout << "Максимальная амплитуда: " << ma_vibration << endl;

    
    bool al = all_of(vibrations.begin(), vibrations.end(), [](double x) {return x > 0.5;});
    if (al) {
        cout << "Все значения превышают 0.5" << endl;
    }
    else {
        cout << "Не все значения превышают 0.5" << endl;
    }
    

    vector<double> srt_vibrations = vibrations;
    sort(srt_vibrations.begin(), srt_vibrations.end(), greater<double>());

    cout << "10 наибольших амплитуд: " << endl;
    for_each(srt_vibrations.begin(), srt_vibrations.begin() + min(10, (int)srt_vibrations.size()), [](double x) {
        cout << x << " ";
        });
    cout << endl << endl;

    
}
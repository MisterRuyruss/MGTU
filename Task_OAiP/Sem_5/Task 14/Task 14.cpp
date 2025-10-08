// Task 14.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
    vector<double> potr(24);

    for (int i = 0; i < 24; i++) {
        potr[i] = (rand() % 400 + 100); 
    }

    cout << "Потребление по часам: " << endl;
    for_each(potr.begin(), potr.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    
    double total_potr = accumulate(potr.begin(), potr.end(), 0.0);
    double cr_potr = total_potr / potr.size();
    cout << "Суммарное потребление: " << total_potr << " кВт·ч" << endl;
    cout << "Среднее потребление: " << cr_potr << " кВт·ч" << endl;

   
    auto ph = max_element(potr.begin(), potr.end());
    int ind = distance(potr.begin(), ph);
    cout << "Пиковое потребление: " << *ph << " кВт·ч в час " << ind << endl;

    
    vector<double> v2(24);
    transform(potr.begin(), potr.end(), v2.begin(),
        [cr_potr](double x) { return x - cr_potr; });

    cout << "Отклонения от среднего: " << endl;
    for_each(v2.begin(), v2.end(), [](double x) {
        cout << x << " ";
        });
    cout << endl;

    
    vector<double> srt_potr = potr;
    sort(srt_potr.begin(), srt_potr.end());

    cout << "Топ-5 минимальных часов потребления: " << endl;
    for_each(srt_potr.begin(), srt_potr.begin() + min(5, (int)srt_potr.size()), [](double x) {
        cout << x << " ";
        });
    cout << endl;

}
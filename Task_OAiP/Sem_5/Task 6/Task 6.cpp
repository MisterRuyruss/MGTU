// Task 6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>

using namespace std;

int main() {
    vector<int> v(20);
    for (int i = 0; i < 10; i++) v[i] = i + 1;
    for (int i = 9; i < 20; i++) v[i] = 5;
    cout << "Наш вектор: " << endl;

    for_each(v.begin(), v.end(), [](int x) {
        cout << x << " ";
        });
    cout << endl;

    auto fnd = find(v.begin(), v.end(), 7);
    if (fnd != v.end()) {
        cout << "Число 7 найдено" << endl;
    }
    else {
        cout << "Число 7 не найдено" << endl;
    }

    sort(v.begin(), v.end());
    auto l = unique(v.begin(), v.end());
    v.erase(l, v.end());

    for_each(v.begin(), v.end(), [](int x) {
        cout << x << " ";
    });
    cout << endl ;




}
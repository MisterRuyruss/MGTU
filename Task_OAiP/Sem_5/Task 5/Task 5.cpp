// Task 5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>

using namespace std;

int main() {
    vector<int> v(20);
    for (int i = 0; i < 20; i++) v[i] = i + 2;

    cout << "Наш вектор: " << endl;

    for_each(v.begin(), v.end(), [](int x) {
        cout << x << " ";
        });
    cout << endl;
    
    auto mima = minmax_element(v.begin(), v.end());
    cout << "Минимальное: " << *mima.first << endl;
    cout << "Максимальное: " << *mima.second << endl;

    double cr = accumulate(v.begin(), v.end(), 0.0) / v.size();
    cout << "Среднее: " << cr << endl;

    vector<int> st = v;
    sort(st.begin(), st.end());
    double med; 
    if (st.size() % 2 == 0) {
        med = (st[st.size() / 2 - 1] + st[st.size() / 2]) / 2.0;
    }
    else {
        med = st[st.size() / 2];
    }
    cout << "Медиана: " << med << endl;

    cout << "Отсортированный вектор: " << endl;
    for_each(st.begin(), st.end(), [](int x) {
        cout << x << " ";
        });


}
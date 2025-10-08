// Task 4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 
#include <random>

using namespace std;

int main() {
    vector<int> v(30);

    
    for (int i = 0; i < 30; i++) v[i] = int(rand() % 201) - 100;
    
    cout << "До: " << endl;

    for_each(v.begin(), v.end(), [](int x) {
        cout << x << " ";
        });

    auto p = remove_if(v.begin(), v.end(), [](int x) { return x < 0; });
    v.erase(p, v.end());

    cout << endl;
    cout << "После: " << endl;

    for_each(v.begin(), v.end(), [](int x) {
        cout << x << " ";
        });


}
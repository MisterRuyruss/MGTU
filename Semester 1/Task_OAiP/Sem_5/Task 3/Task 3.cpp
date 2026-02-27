// Task 3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{   
    vector<int> v(20);

    for (int i = 0; i < 20; i++) v[i] = i + 1;

    auto mx = max_element(v.begin(), v.end(), [](int a, int b) { return a < b; });
    
    cout << "Максимальный элемент: " << *mx << endl;
}

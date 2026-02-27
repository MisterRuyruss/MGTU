// Task 2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm> 

using namespace std;

int main() {
   vector<int> v(10);

    for (int i = 0; i < 10; i++)
        v[i] = i + 1;

    for_each(v.begin(), v.end(), [](int x) {x *= 2; cout << x << " "; });


}

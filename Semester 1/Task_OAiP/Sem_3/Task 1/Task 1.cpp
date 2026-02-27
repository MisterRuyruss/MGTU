// Task 1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int n;
    int count = 0;
    int A[10000];
    cout << "Введите n: ";
    cin >> n;
    
    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) cin >> A[i];

    
    for (int i = 0; i < n; i++) {
        if (A[i] > pow(2, i + 1)) count++;
    }
    cout << "Количество элементов: " << count << endl;
    
}

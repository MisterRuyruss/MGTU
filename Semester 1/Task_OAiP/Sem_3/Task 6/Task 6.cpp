// Task 6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;

int main() {
    double A[15];
    cout << "Введите 15 чисел: ";
    for (int i = 0; i < 15; i++) cin >> A[i];

    int min = 0, max = 0;
    for (int i = 0; i < 15; i++) {
        if (A[i] < A[min]) min = i;
        if (A[i] > A[max]) max = i;
    }

    if (min == 1 && max == 0) {
        for (int i = 0; i < 15; i++) A[i] /= 2;
    }

    cout << "Результат: ";
    for (int i = 0; i < 15; i++) cout << A[i] << " ";
    
}

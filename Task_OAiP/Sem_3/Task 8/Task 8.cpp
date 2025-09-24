// Task 8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;

int main() {
    int A[10];
    cout << "Введите 10 чисел: ";
    for (int i = 0; i < 10; i++) cin >> A[i];

    int min = 0, max = 0;
    for (int i = 0; i < 10; i++) {
        if (A[i] < A[min]) min = i;
        if (A[i] > A[max]) max = i;
    }

    if (max == 1 && min == 4) {
        for (int i = min; i < 10; i++) A[i] = A[max];
    }

    cout << "Результат: ";
    for (int i = 0; i < 10; i++) cout << A[i] << " ";
    
}

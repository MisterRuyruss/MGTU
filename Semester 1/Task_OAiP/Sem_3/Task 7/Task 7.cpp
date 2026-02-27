// Task 7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;

int main() {
    int A[15];
    cout << "Введите 15 чисел: ";
    for (int i = 0; i < 15; i++) cin >> A[i];

    int min = 0, max = 0;
    for (int i = 0; i < 15; i++) {
        if (A[i] < A[min]) min = i;
        if (A[i] > A[max]) max = i;
    }

    if (A[min] == 0 || A[max] == 0) {
        for (int i = 0; i < min; i++) A[i] = 0;
    }

    cout << "Результат: ";
    for (int i = 0; i < 15; i++) cout << A[i] << " ";
    
}

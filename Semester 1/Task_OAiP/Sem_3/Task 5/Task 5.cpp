// Task 5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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

    if (min == 4 && max == 9) {
        cout << "Среднее арифметическое = " << (A[min] + A[max]) / 2.0 << endl;
    }
    else {
        cout << "Условие не выполняется" << endl;
    }
    
}

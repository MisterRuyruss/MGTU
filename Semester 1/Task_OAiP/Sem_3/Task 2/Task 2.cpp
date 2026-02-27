// Task 2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;

int main() {
    int n;
    int A[10000];
    int ma = -10000000; 
    int c = 0;
    cout << "Введите n: ";
    cin >> n;
    
    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) cin >> A[i];

   
    for (int i = 0; i < n; i++) {
        if (A[i] % 2 == 0) c++;
        else if (A[i] > ma) ma = A[i];
    }
    cout << "Наибольшее нечётное: " << ma << endl;
    cout << "Количество чётных: " << c << endl;
    
}

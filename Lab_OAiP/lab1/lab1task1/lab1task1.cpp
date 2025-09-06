// Вариант 4


#include <iostream>

using namespace std;


int main()
{
    const int size = 10;
    int array[size];
    int suma = 0;
    int max = -10 ^ 10;

    for (int i = 0; i < size; i++) {
        cout << "Введите " << i + 1 << " элемент: ";
        cin >> array[i];
    }
    cout << "Ваш введённый масив:" << endl;

    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < size; i++) {
        suma += array[i];
    }
    cout << "Сумма элементов вашего массива: " << suma << endl;
    
    for (int i = 0; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    cout << "Максимальный элемент вашего массива: " << max << endl;

    for (int i = 0; i < size; i++) {
        for (int j = i; j > 0 && array[j - 1] < array[j]; j--) {
            swap(array[j], array[j - 1]);
        }
    }
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}



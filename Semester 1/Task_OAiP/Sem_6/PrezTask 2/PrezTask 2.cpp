// PrezTask 2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char text[1000];
    char symbol;

    cout << "Введите строку: ";
    cin.getline(text, 1000);

    cout << "Введите символ для удаления: ";
    cin >> symbol;

    char result[1000];
    int j = 0;

    for (int i = 0; i < strlen(text); i++) {
        if (text[i] != symbol) {
            result[j++] = text[i];
        }
    }
    result[j] = '\0';

    cout << "Результат: " << result << endl;
}
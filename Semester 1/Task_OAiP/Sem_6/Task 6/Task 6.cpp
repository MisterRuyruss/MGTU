// Task 6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char str[100];
    int shift;
    cout << "Исходная строка: ";
    cin.getline(str, 100);
    cout << "Сдвиг: ";
    cin >> shift;

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = (str[i] - 'A' + shift) % 26 + 'A';
        }
        else if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = (str[i] - 'a' + shift) % 26 + 'a';
        }
    }

    cout << "Зашифрованная строка: " << str << endl;
}

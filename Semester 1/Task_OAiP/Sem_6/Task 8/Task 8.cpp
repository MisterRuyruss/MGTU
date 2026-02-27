// Task 8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char str[100];
    cout << "Исходная строка: ";
    cin.getline(str, 100);

    bool isnew = true;

    for (int i = 0; i < strlen(str); i++) {
        if (isnew && str[i] >= 'a' && str[i] <= 'z') {
            str[i] = toupper(str[i]);
            isnew = false;
        }
        else if (!isnew && str[i] >= 'A' && str[i] <= 'z') {
            str[i] = tolower(str[i]);
        }

        if (str[i] == '.' || str[i] == '!' || str[i] == '?') {
            isnew = true;
        }
    }

    cout << "Исправленная строка: " << str << endl;
}

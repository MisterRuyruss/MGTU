// Task 5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char str[100];
    cout << "Строка: ";
    cin.getline(str, 100);
    int glac = 0, cogl = 0;

    for (int i = 0; i < strlen(str); i++) {
        str[i] = toupper(str[i]);

        if (str[i] >= 'A' && str[i] <= 'Z') {
            if (str[i] == 'A' || str[i] == 'E' || str[i] == 'I' || str[i] == 'O' || str[i] == 'U' || str[i] == 'Y') {
                glac++;
            }
            else {
                cogl++;
            }
        }
    }

    cout << "Гласных: " << glac << endl;
    cout << "Согласных: " << cogl << endl;
}


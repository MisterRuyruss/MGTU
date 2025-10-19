// PrezTask 3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char text[1000], start[100], end[100];

    cout << "Введите строку: ";
    cin.getline(text, 1000);

    cout << "Введите подстроку для замены: ";
    cin.getline(start, 100);

    cout << "Введите новую подстроку: ";
    cin.getline(end, 100);

    char result[2000] = "";

    for (int i = 0; i < strlen(text); i++) {
        bool found = true;

        for (int j = 0; j < strlen(start); j++) {
            if (text[i + j] != start[j]) {
                found = false;
                break;
            }
        }

        if (found) {
            strcat_s(result, end);
            i += strlen(start) - 1;
        }
        else {
            char x[2] = { text[i], '\0' };
            strcat_s(result, x);
        }
    }

    cout << "Результат: " << result << endl;
}
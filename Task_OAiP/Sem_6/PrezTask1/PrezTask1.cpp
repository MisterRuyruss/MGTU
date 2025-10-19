// PrezTask1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char text[1000];
    cout << "Введите строку: ";
    cin.getline(text, 1000);

    char lword[100] = "";
    char now[100] = "";
    int malen = 0;
    int j = 0;

    for (int i = 0; i <= strlen(text); i++) {
        if (text[i] != ' ' && text[i] != '\0') {
            now[j++] = text[i];
        }
        else {
            now[j] = '\0';
            if (strlen(now) > malen) {
                malen = strlen(now);
                strcpy_s(lword, now);
            }
            j = 0;
            now[0] = '\0';
        }
    }

    cout << "Самое длинное слово: " << lword << endl;
}
// PrezTask 4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

int main() {
    char str1[100], str2[100];

    cout << "Введите первую строку: ";
    cin.getline(str1, 100);

    cout << "Введите вторую строку: ";
    cin.getline(str2, 100);

    // Удаляем пробелы
    char t1[100] = "", t2[100] = "";
    int i1 = 0, i2 = 0;

    for (int i = 0; i < strlen(str1); i++) {
        if (str1[i] != ' ') {
            t1[i1++] = str1[i];
        }
    }
    t1[i1] = '\0';

    for (int i = 0; i < strlen(str2); i++) {
        if (str2[i] != ' ') {
            t2[i2++] = str2[i];
        }
    }
    t2[i2] = '\0';

    sort(t1, t1 + strlen(t1));
    sort(t2, t2 + strlen(t2));

    if (strcmp(t1, t2) == 0) {
        cout << "Да" << endl;
    }
    else {
        cout << "Нет" << endl;
    }

}
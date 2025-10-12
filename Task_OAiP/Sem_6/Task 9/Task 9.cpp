// Task 9.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char str1[100], str2[100];
    cout << "Строка 1: ";
    cin.getline(str1, 100);
    cout << "Строка 2: ";
    cin.getline(str2, 100);

    int malen = 0; 
    int endi = 0; 

    for (int i = 0; i < strlen(str1); i++) {
        for (int j = 0; j < strlen(str2); j++) {
            int len = 0;

            while (i + len < strlen(str1) && j + len < strlen(str2) &&
                str1[i + len] == str2[j + len]) {
                len++;
            }

            if (len > malen) {
                malen = len;
                endi = i + len;
            }
        }
    }

    if (malen > 0) {
        cout << "Самая длинная общая подстрока: ";
        for (int i = endi - malen; i < endi; i++) {
            cout << str1[i];
        }
        cout << endl;
    }
    else {
        cout << "Общих подстрок не найдено" << endl;
    }

}
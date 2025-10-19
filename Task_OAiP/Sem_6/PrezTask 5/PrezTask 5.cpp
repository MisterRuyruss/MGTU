// PrezTask 5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char text[1000];
    cout << "Введите строку: ";
    cin.getline(text, 1000);

    char word[100];
    int j = 0;

    cout << "Слова:" << endl;

    for (int i = 0; i <= strlen(text); i++) {
        if (text[i] != ' ' && text[i] != '\0') {
            word[j++] = text[i];
        }
        else if (j > 0) {
            word[j] = '\0';
            cout << word << endl;
            j = 0;
        }
    }

}
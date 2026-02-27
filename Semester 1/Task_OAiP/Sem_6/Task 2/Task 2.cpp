// Task 2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char str[100];
    cout << "Строка: ";
    cin.getline(str, 100);

    int c = 0;
    int lenstr = strlen(str);
    cout << "Уникальные символы: ";
    for (int i = 0; i < lenstr; i++) {
        bool isnew = true;
        for (int j = 0; j < i; j++) {
            if (str[i] == str[j]) {
                isnew = false;
                break;
            }
        }
        if (isnew) {
            c++;
            cout << str[i] << " ";

        }
    }
    cout << endl;
    cout << "Уникальных символов: " << c << endl << endl;

    
}


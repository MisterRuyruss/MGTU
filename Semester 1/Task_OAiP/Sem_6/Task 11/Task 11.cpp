// Task 11.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char str[100];
    int k;
    cout << "Строка: ";
    cin.getline(str, 100);
    cout << "k: ";
    cin >> k;
    cout << "Результат:" << endl;

    for (int i = 0; i <= strlen(str) - k; i++) {
        bool notone = false;

        for (int j = i; j < i + k; j++) {
            for (int m = j + 1; m < i + k; m++) {
                if (str[j] == str[m]) {
                    notone = true;
                    break;
                }
            }
            if (notone) break;
        }

        if (!notone) {
            for (int j = i; j < i + k; j++) {
                cout << str[j];
            }
            cout << endl;
        }
    }

}

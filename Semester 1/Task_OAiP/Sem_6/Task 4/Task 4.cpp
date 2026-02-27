// Task 4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char A[100], B[100];
    cout << "Строка A: ";
    cin.getline(A, 100);
    cout << "Строка B: ";
    cin.getline(B, 100);

    bool isok = true;

    for (int i = 0; i < strlen(B); i++) {
        bool found = false;

        for (int j = 0; j < strlen(A); j++) {
            if (B[i] == A[j]) {
                found = true;
                break;
            }
        }

        if (!found) {
            isok = false;
            break;
        }
    }
    if (isok) {
        cout << "Все символы строки B содержатся в строке A" << endl;
    }
    else {
        cout << "Не все символы строки B содержатся в строке A" << endl;
    }
}


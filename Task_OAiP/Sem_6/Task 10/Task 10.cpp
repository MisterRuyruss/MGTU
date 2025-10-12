// Task 10.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char str[100];
    cout << "Строка: ";
    cin.getline(str, 100);

    int start = 0;

    for (int i = 0; i <= strlen(str); i++) {
        if (str[i] == ' ' || str[i] == '\0') {
            int end = i - 1;
            while (start < end) {
                char x = str[start];
                str[start] = str[end];
                str[end] = x;
                start++;
                end--;
            }
            start = i + 1;
        }
    }

    cout << "Перевернутая строка: " << str << endl;
}


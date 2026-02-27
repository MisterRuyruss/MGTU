// Task 7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char str[100];
    cout << "Строка: ";
    cin.getline(str, 100);

    char res[100] = "";
    int resi = 0;

    int i = 0;
    while (i < strlen(str)) {
        char now = str[i];
        int count = 1;

        while (i + count < strlen(str) && str[i + count] == now) {
            count++;
        }

        res[resi] = now;
        resi++;

        res[resi] = '0' + count;
        resi++;

        i += count; 
    }

    res[resi] = '\0'; 

    cout << "Сжатая строка: " << res << endl;
}


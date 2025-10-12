// Task 3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main()
{
    char str[100];
    cout << "Строка: ";
    cin.getline(str, 100);

    char newstr[100];
    int j = 0; 

    for (int i = 0; i < strlen(str); i++) {
        if (!(str[i] >= '0' && str[i] <= '9')) {
            newstr[j] = str[i]; 
            j++;
        }
    }
    newstr[j] = '\0'; 

    cout << "Результат: " << newstr << endl;

}
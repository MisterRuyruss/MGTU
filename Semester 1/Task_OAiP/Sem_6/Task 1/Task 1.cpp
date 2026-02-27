// Task 1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstring>

using namespace std;

int main() {
    char str[100];
    cout << "Строка: ";
    cin.getline(str, 100);
   
    bool isPal = true;
    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        while (str[left] == ' ') left++;
        while (str[right] == ' ') right--;

        char leftChar = tolower(str[left]);
        char rightChar = tolower(str[right]);

        
        if (leftChar != rightChar) {
            isPal = false;
            break;
        }

        left++;
        right--;
    }
    if (isPal) {
        cout << "Палиндром" << endl;
    }
    else {
        cout << "Не Палиндром" << endl;
    }
    

}


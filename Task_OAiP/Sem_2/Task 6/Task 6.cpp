// Проверка возраста

#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Введите ваш возраст: ";
    cin >> a;
    if (a < 18) {
        cout << "Вы подросток!";
    }
    else if (a > 60) {
        cout << "Вы пожилой человек!";
    }
    else {
        cout << "Вы взрослый и молодой!";
    }
}


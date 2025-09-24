// Проверка четного или нечетного числа

#include <iostream>

using namespace std;

int main()
{
    int a;
    cout << "Ваше число: ";
    cin >> a;
    if (a % 2 == 1) {
        cout << "Число нечетно!";
    }
    else
    {
        cout << "Число чётно!";
    }
}



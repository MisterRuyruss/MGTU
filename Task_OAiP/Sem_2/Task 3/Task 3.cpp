// Конвертация градусов Цельсия в Фаренгейты

#include <iostream>

using namespace std;

int main()
{
    double a;
    cout << "Градусы Целься: ";
    cin >> a;
    cout << "Фаренгейты: " << (a * 9)/5 + 32 << endl;
}

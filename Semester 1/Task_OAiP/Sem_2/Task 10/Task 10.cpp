// Проверка положительного, отрицательного или нуля

#include <iostream>

using namespace std;

int main()
{
    int n;
    cout << "Введите ваше число: ";
    cin >> n;
    if (n == 0) { cout << "Ваше число равно нулю!"; }
    else if (n < 0) { cout << "Ваше число отрицательное!"; }
    else { cout << "Ваше число положительное!"; }
}


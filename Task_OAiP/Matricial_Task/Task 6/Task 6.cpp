// Task 6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;

int main()
{
    double a11 = 2, a12 = 3, b1 = 13;
    double a21 = 5, a22 = -1, b2 = 9;

    double D = a11 * a22 - a12 * a21;
    double Dx = b1 * a22 - a12 * b2;
    double Dy = a11 * b2 - b1 * a21;

    double x = Dx / D;
    double y = Dy / D;

    cout << "x = " << x << ", y = " << y << endl;
}


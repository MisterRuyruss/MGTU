// PrezTask1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int x1, y1, x2, y2, x3, y3;

    cout << "Введите координаты точки A: ";
    cin >> x1 >> y1;

    cout << "Введите координаты точки B: ";
    cin >> x2 >> y2;

    cout << "Введите координаты точки C: ";
    cin >> x3 >> y3;
    double opr, area;
    opr = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    area = abs(opr) / 2.0;

    
    cout << "Площадь треугольника: " << area << endl;
}


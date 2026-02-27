// PrezTask2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int x, y;
    int grad;

    cout << "Введите координаты точки: ";
    cin >> x >> y;

    cout << "Введите угол поворота в градусах: ";
    cin >> grad;

    double rad;
    rad = grad * acos(-1.0) / 180.0;

    double x1, y1;
    x1 = x * cos(rad) + y * sin(rad);
    y1 = -x * sin(rad) + y * cos(rad);

    cout << "Угол поворота: " << grad << " градусов" << endl;
    cout << "Новые координаты: " << x1 << ", " << y1 << endl;
}

// Task 5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

using namespace std;

int main()
{
    double ver[3][2];
    double X;

    cout << "Введите координаты точек по очереди:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i + 1 << " точка:" << endl;
        cin >> ver[i][0] >> ver[i][1];
    }
    X = (ver[1][0] - ver[0][0]) * (ver[2][1] - ver[0][1]) - (ver[1][1] - ver[0][1]) * (ver[2][0] - ver[0][0]);
    if (X == 0) {
        cout << "Точки лежат на одной прямой";
    }
    else {
        cout << "Точки не лежат на одной прямой";
    }


}


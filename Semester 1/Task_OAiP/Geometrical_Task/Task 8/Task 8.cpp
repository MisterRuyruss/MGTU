// Task 8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    double ver[2][2];
    double dx, dy,dist;

    cout << "Введите координаты отрезка по очереди:" << endl;
    for (int i = 0; i < 2; i++) {
        cout << i + 1 << " координата:" << endl;
        cin >> ver[i][0] >> ver[i][1];
    }
    dx = ver[0][0] - ver[1][0];
    dy = ver[0][1] - ver[1][1];
    dist = sqrt(pow(dx, 2) + pow(dy, 2));
    
    cout << "Длина отрезка: " << dist;


}


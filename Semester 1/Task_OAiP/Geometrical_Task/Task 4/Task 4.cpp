// Task 4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>

using namespace std;

int dist(double a1, double b1, double a2, double b2) {
    return sqrt(pow(a2 - a1, 2) + (pow(b2 - b1, 2)));
}


int main()
{
    double ver[4][2];
    double k1, ma = -10;
    double per[6];

    cout << "Введите координаты вершин четырёхугольника по очереди:" << endl;
    for (int i = 0; i < 4; i++) {
        cout << i + 1 << " вершина:" << endl;
        cin >> ver[i][0] >> ver[i][1];
    }

    per[0] = dist(ver[0][0], ver[0][1], ver[1][0], ver[1][1]);
    per[1] = dist(ver[1][0], ver[1][1], ver[2][0], ver[2][1]);
    per[2] = dist(ver[2][0], ver[2][1], ver[3][0], ver[3][1]);
    per[3] = dist(ver[3][0], ver[3][1], ver[0][0], ver[0][1]);
    per[4] = dist(ver[0][0], ver[0][1], ver[2][0], ver[2][1]);
    per[5] = dist(ver[1][0], ver[1][1], ver[3][0], ver[3][1]);
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (per[i] == per[j] && ma < per[i]) {
                ma = per[i];
            }
        }      
    }

    cout << "Длина диагонали равна: " << ma;

}

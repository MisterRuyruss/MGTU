// Task 2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
    double P1,P2,P3,P4;

    cout << "Введите координаты вершин четырёхугольника по очереди:" << endl;
    for (int i = 0; i < 4; i++) {
        cout << i + 1 << " вершина:" << endl;
        cin >> ver[i][0] >> ver[i][1];
    }
    
    P1 = dist(ver[0][0],ver[0][1],ver[1][0],ver[1][1]);
    P2 = dist(ver[1][0], ver[1][1], ver[2][0], ver[2][1]);
    P3 = dist(ver[2][0], ver[2][1], ver[3][0], ver[3][1]);
    P4 = dist(ver[3][0], ver[3][1], ver[0][0], ver[0][1]);

    cout << "Периметр заданного четырёхугольника: " << P1+P2+P3+P4;


}

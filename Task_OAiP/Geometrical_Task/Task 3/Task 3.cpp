// Task 3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    double ver[3][2];
    double Cx,Cy;

    cout << "Введите координаты вершин по очереди:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i + 1 << " вершина:" << endl;
        cin >> ver[i][0] >> ver[i][1];
    }

    Cx = (ver[0][0] + ver[1][0] + ver[2][0]) / 3;
    Cy = (ver[0][1] + ver[1][1] + ver[2][1]) / 3;
    cout << "Координаты центра масс: " << Cx << " " << Cy;


}


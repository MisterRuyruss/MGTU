// Task 7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;

double orient(double x1, double y1, double x2, double y2, double x3, double y3) {
    return (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);
}


int main() {
    double otr1[2][2], otr2[2][2];
    double OT1, OT2, OT3, OT4;

    cout << "Введите координаты первого отрезка:" << endl;
    for (int i = 0; i < 2; i++) {
        cout << i + 1 << " вершина:" << endl;
        cin >> otr1[i][0] >> otr1[i][1];
    }

    cout << "Введите координаты второго отрезка:" << endl;
    for (int i = 0; i < 2; i++) {
        cout << i + 1 << " вершина:" << endl;
        cin >> otr2[i][0] >> otr2[i][1];
    }

    OT1 = orient(otr1[0][0], otr1[0][1], otr1[1][0], otr1[1][0], otr2[0][0], otr2[0][1]);
    OT2 = orient(otr1[0][0], otr1[0][1], otr1[1][0], otr1[1][0], otr2[1][0], otr2[1][1]);
    OT3 = orient(otr2[0][0], otr2[0][1], otr2[1][0], otr2[1][1], otr1[0][0], otr1[0][1]);
    OT4 = orient(otr2[0][0], otr2[0][1], otr2[1][0], otr2[1][1], otr1[1][0], otr1[1][0]);
    cout << OT1 << " " << OT2 << " " << OT3 << " " << OT4 << endl;
    if (OT1 * OT2 < 0 && OT3 * OT4 < 0) {
        cout << "Отрезки пересекаются" << endl;
    }
    else {
        cout << "Отрезки не пересекаются" << endl;
    }
    

}



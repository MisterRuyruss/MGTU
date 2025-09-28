// Task 8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;


double det3(double m[3][3]) {
    return m[0][0]*(m[1][1]*m[2][2] - m[1][2]*m[2][1])
         - m[0][1]*(m[1][0]*m[2][2] - m[1][2]*m[2][0])
         + m[0][2]*(m[1][0]*m[2][1] - m[1][1]*m[2][0]);
}

int main() {
    double A[3][3] = {
        { 3, -1,  2 },
        { 2,  4, -1 },
        { 1, -3,  1 }
    };

    double b[3] = {5, 6, 4};
    double D = det3(A);

    // Определитель для x
    double Ax[3][3] = {
        { b[0], -1,  2 },
        { b[1],  4, -1 },
        { b[2], -3,  1 }
    };

    // Определитель для y
    double Ay[3][3] = {
        {  3, b[0],  2 },
        {  2, b[1], -1 },
        {  1, b[2],  1 }
    };

    // Определитель для z
    double Az[3][3] = {
        {  3, -1, b[0] },
        {  2,  4, b[1] },
        {  1, -3, b[2] }
    };

    double Dx = det3(Ax);
    double Dy = det3(Ay);
    double Dz = det3(Az);

    double x = Dx / D;
    double y = Dy / D;
    double z = Dz / D;

    cout << "x = " << x << ", y = " << y << ", z = " << z << "\n";
    return 0;
}

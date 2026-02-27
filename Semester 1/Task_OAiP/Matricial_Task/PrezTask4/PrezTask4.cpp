// PrezTask4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
using namespace std;

void gaussJordan(double matrix[4][5]) {
    int n = 4;
    for (int i = 0; i < n; i++) {
        double pivot = matrix[i][i];
        for (int j = 0; j <= n; j++) {
            matrix[i][j] /= pivot;
        }
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = matrix[k][i];
                for (int j = 0; j <= n; j++) {
                    matrix[k][j] -= factor * matrix[i][j];
                }
            }
        }
    }
}

int main() {
    double x[4], y[4], z[4];

    for (int i = 0; i < 4; i++) {
        cout << "Введите координаты точки " << char('A' + i) << ": ";
        cin >> x[i] >> y[i] >> z[i];
    }

    double matrix[4][5];
    for (int i = 0; i < 4; i++) {
        matrix[i][0] = x[i];
        matrix[i][1] = y[i];
        matrix[i][2] = z[i];
        matrix[i][3] = 1;
        matrix[i][4] = -(x[i] * x[i] + y[i] * y[i] + z[i] * z[i]);
    }

    gaussJordan(matrix);
    double A, B, C, D, x0, y0, z0, r;

    A = matrix[0][4];
    B = matrix[1][4];
    C = matrix[2][4];
    D = matrix[3][4];

    x0 = -A / 2;
    y0 = -B / 2;
    z0 = -C / 2;
    r = sqrt(x0 * x0 + y0 * y0 + z0 * z0 - D);

    cout << "Центр сферы: " << x0 << ", " << y0 << ", " << z0 << endl;
    cout << "Радиус сферы: " << r << endl;

    cout << "Уравнение cферы:" << endl;
    cout << r << "^2 = " << r * r << " = (x - (" << x0 << "))^2 + (y - (" << y0 << "))^2 + (z - (" << z0 << "))^2" << endl;

    return 0;
}
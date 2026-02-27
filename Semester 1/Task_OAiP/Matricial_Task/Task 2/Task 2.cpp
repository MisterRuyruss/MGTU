#include <iostream>
#include <vector>
using namespace std;

void print(const vector<vector<int>>& M) {
    int N = M.size();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    int N;
    cout << "Введите размер матрицы NхN: ";
    cin >> N;
    vector<vector<int>> A(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            cout << "Введите элемент, находящийся на " << j + 1 << " строке и в " << i + 1 << " столбце: ";
            cin >> A[i][j];
        }
    cout << "Ваша матрица: " << endl;
    print(A);

    int sumstr = 0;
    for (int j = 0; j < N; j++) sumstr += A[0][j]; // сумма первой строки
    bool ok = true;

    // Проверяем строки
    for (int i = 0; i < N; i++) {
        int sum = 0;
        for (int j = 0; j < N; j++) sum += A[i][j];
        if (sum != sumstr) ok = false;
    }

    // Проверяем столбцы
    for (int j = 0; j < N; j++) {
        int sum = 0;
        for (int i = 0; i < N; i++) sum += A[i][j];
        if (sum != sumstr) ok = false;
    }

    // Проверяем диагонали  
    int d1 = 0, d2 = 0;
    for (int i = 0; i < N; i++) {
        d1 += A[i][i];
        d2 += A[i][N - 1 - i];
    }
    if (d1 != sumstr || d2 != sumstr) ok = false;

    if (ok) cout << "YES " << sumstr;
    else cout << "NO";
    return 0;
}

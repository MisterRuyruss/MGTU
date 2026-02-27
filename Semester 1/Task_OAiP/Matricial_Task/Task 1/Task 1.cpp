// Task 1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
using namespace std;

bool Matrix(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int N = A.size();
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (A[i][j] != B[i][j]) return false;
    return true;
}


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

int main()
{
    int N;
    cout << "Введите размер матрицы NхN: ";
    cin >> N;
    vector<vector<int>> A(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++){
            cout << "Введите элемент, находящийся на " << j + 1 << " строке и в " << i + 1 << " столбце: ";
            cin >> A[i][j];
        }
    cout << "Ваша матрица: " << endl;
    print(A);
 
    vector<vector<int>> B(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            B[i][j] = A[j][i];
    cout << "Отражение относительно главной диагонали: " << endl;
    print(B);

    vector<vector<int>> C(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            C[i][j] = A[N - 1 - j][N - 1 - i];
    cout << "Отражение относительно побочной диагонали: " << endl;
    print(C);

    vector<vector<int>> D(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            D[i][j] = A[i][N - 1 - j];
    cout << "Отражение по вертикали: " << endl;
    print(D);

    vector<vector<int>> E(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            E[i][j] = A[N - 1 - i][j];
    cout << "Отражение по горизонтали: " << endl;
    print(E);
 
    bool found = false;
    vector<vector<int>> mats[4] = {B, C, D, E};
    for (int i = 0; i < 4; i++)
        for (int j = i + 1; j < 4; j++)
            if (Matrix(mats[i], mats[j])) {
                cout << i + 1 << " " << j + 1 << endl;
                found = true;
            }
    if (!found) cout << "NONE";


}


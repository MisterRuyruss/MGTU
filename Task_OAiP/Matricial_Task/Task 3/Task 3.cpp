// Task 3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
using namespace std;


void print(const vector<vector<int>>& M) {
    int N = M.size();
    int B = M[0].size();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < B; j++) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    int N, M;
    cout << "Введите размер матрицы NхM: ";
    cin >> N >> M;
    vector<vector<int>> A(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            cout << "Введите элемент, находящийся на " << j + 1 << " строке и в " << i + 1 << " столбце: ";
            cin >> A[i][j];
        }
    cout << "Ваша матрица: " << endl;
    print(A);

    vector<int> rowMin(N, 1e9), rowMax(N, -1e9);
    vector<int> colMin(M, 1e9), colMax(M, -1e9);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            rowMin[i] = min(rowMin[i], A[i][j]);
            rowMax[i] = max(rowMax[i], A[i][j]);
            colMin[j] = min(colMin[j], A[i][j]);
            colMax[j] = max(colMax[j], A[i][j]);
        }

    bool found = false;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            if (A[i][j] == rowMin[i] && A[i][j] == colMax[j]) {
                cout << "A " << A[i][j] << " " << i + 1 << " " << j + 1 << endl;
                found = true;
            }
            if (A[i][j] == rowMax[i] && A[i][j] == colMin[j]) {
                cout << "B " << A[i][j] << " " << i + 1 << " " << j + 1 << endl;
                found = true;
            }
        }
    if (!found) cout << "NONE";

}


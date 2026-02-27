// Task 5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
    vector<vector<int>> A(N, vector<int>(M));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            cout << "Введите элемент, находящийся на " << j + 1 << " строке и в " << i + 1 << " столбце: ";
            cin >> A[i][j];
        }
    cout << "Ваша матрица: " << endl;
    print(A);
            


    int verx = 0, niz = N - 1, left = 0, right = M - 1;
    while (verx <= niz && left <= right) {
        for (int j = left; j <= right; j++) cout << A[verx][j] << " ";
        verx++;
        for (int i = verx; i <= niz; i++) cout << A[i][right] << " ";
        right--;
        if (verx <= niz) {
            for (int j = right; j >= left; j--) cout << A[niz][j] << " ";
            niz--;
        }
        if (left <= right) {
            for (int i = niz; i >= verx; i--) cout << A[i][left] << " ";
            left++;
        }
    }


}

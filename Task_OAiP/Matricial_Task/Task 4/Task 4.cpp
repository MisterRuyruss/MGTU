// Task 4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
using namespace std;



int main()
{
    int N;
    cout << "Введите размер матрицы NхN: ";
    cin >> N;
    vector<vector<int>> A(N, vector<int>(N));
    
    int verx = 0, niz = N - 1, left = 0, right = N - 1, num = 1;
    while (verx <= niz && left <= right) {
        for (int j = left; j <= right; j++) A[verx][j] = num++;
        verx++;
        for (int i = verx; i <= niz; i++) A[i][right] = num++;
        right--;
        if (verx <= niz) {
            for (int j = right; j >= left; j--) A[niz][j] = num++;
            niz--;
        }
        if (left <= right) {
            for (int i = niz; i >= verx; i--) A[i][left] = num++;
            left++;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) cout << A[i][j] << " ";
        cout << endl;
        
    }




}


// Таблица квадратов чисел

#include <iostream>

using namespace std;

int main()
{
    int n;
    cout << "Ваше число: ";
    cin >> n;
    cout << "Квадраты чисел от 1 до " << n << endl;
    for (int i = 1; i <= n; i++) {
        cout << "(" << i << ")" << i * i << " ";
    }
    cout << endl;
}


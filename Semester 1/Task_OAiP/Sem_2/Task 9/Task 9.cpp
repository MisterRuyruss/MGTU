// Сумма чисел от 1 до n, кратных 2 или 5

#include <iostream>

using namespace std;

int main()
{
	int n;
	int sum = 0;
	cout << "Введите ваше число: " ;
	cin >> n;
	for (int i = 0; i <= n; i++) {
		if (i % 2 == 0 || i % 5 == 0) {
			sum += i;
		}
	}
	cout << "Сумма чисел:" << sum;
}


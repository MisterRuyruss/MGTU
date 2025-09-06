// Вариант 4
// Номер 2

#include <iostream>
using namespace std;

int main()
{
	int x;
	int kol = 1;
	cout << "Сколько машин можно произвести: ";

	cin >> x;

	while (x > 0) {
		kol *= x;
		x -= 1;
	}
	cout << "Производительность: " << kol;



}

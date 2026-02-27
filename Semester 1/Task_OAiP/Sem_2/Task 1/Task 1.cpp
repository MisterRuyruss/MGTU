// Перевод сантиметров в метры и сантиметры

#include <iostream>
using namespace std;


int main()
{
    int dlina;

    cout << "Введите длину измеряемого объекта в см:" << endl;
    cin >> dlina;
    cout << "Размеры измерямого объекта составляют " << dlina / 100 << " метра(ов) и " << dlina % 100 << " сантиметра(ов)";

}


// Вариант 4
// Номер 1

#include <iostream>
using namespace std;

int main()
{
    
    float x;

    cout << "Введите вашу скорость: ";
    cin >> x;
    
    if (x < 60.0) {
        cout << "Скорость в пределах нормы";
    }
    else {
        if (x > 80.0) {
            cout << "Превышение скорости!";
        }
        else {
            cout << "Вы приближаетесь к превышению скорости";
        }
    }


    
}

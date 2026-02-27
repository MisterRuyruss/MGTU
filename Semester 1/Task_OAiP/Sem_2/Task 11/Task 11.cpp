// – Вывод таблицы ASCII
#include <iostream>

using namespace std;

int main()
{
    cout << "Таблица ASCII (от 32 до 126):" << endl;
    cout << "Код     Символ" << endl;
    cout << "-----------------" << endl;

    for (int i = 32; i <= 126; i++) {
        cout <<  i << "     " << char(i) << endl;
    }

}

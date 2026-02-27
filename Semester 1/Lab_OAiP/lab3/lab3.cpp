// lab3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {

    int num;
    string fname = "menu.txt";
    string fout = "output.txt";

    do {
        cout << endl;
        cout << "=== Меню ресторана ===" << endl;
        cout << "1 - Добавить блюда" << endl;
        cout << "2 - Поиск блюда по названию" << endl;
        cout << "3 - Сортировка блюд" << endl;
        cout << "4 - Вывод блюд по цене" << endl;
        cout << "5 - Показать все блюда" << endl;
        cout << "0 - Выход" << endl;
        cout << "Выберите действие: ";
        cin >> num;

        switch (num) {
        case 1: {
            ofstream file(fname, ios::app);
            ofstream output(fout, ios::app);

            output << "=== Добавление блюд ===" << endl;

            cout << "Добавление блюд" << endl;

            while (true) {
                string name, type;
                double price;
                cin.ignore();

                cout << "Название блюда: ";
                getline(cin, name);

                if (name.empty()) break;

                cout << "Тип (Закуски/Основное блюдо/Десерт/Напиток/Для детей): ";
                getline(cin, type);

                cout << "Цена: ";
                cin >> price;
                cin.ignore();

                file << name << "," << type << "," << price << endl;
                output << "Добавлено: " << name << "," << type << "," << price << endl;

                cout << "Блюдо добавлено!" << endl;
            }

            file.close();
            output.close();
            break;
        }

        case 2: {
            ifstream file(fname);
            ofstream output(fout, ios::app);
            string find;

            cin.ignore();
            cout << "Введите название для поиска: ";
            getline(cin, find);

            output << "=== Поиск слова: " << find << " ===" << endl;

            string line;
            bool found = false;

            while (getline(file, line)) {
                size_t pos1 = line.find(",");
                string name = line.substr(0, pos1);

                if (name == find) {
                    size_t pos2 = line.find(",", pos1 + 1);
                    string type = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    string price = line.substr(pos2 + 1);

                    cout << "Найдено: " << name << " | " << type << " | " << price << " руб" << endl;
                    output << "Найдено: " << name << " | " << type << " | " << price << " руб" << endl;
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Блюдо не найдено!" << endl;
                output << "Блюдо не найдено!" << endl;
            }

            file.close();
            output.close();
            break;
        }

        case 3: {
            ifstream file(fname);
            ofstream output(fout, ios::app);
            vector<string> words;
            string line;

            while (getline(file, line)) {
                if (!line.empty()) {
                    words.push_back(line);
                }
            }
            file.close();

            cout << "Сортировать по:" << endl;
            cout << "1 - Типу" << endl;
            cout << "2 - Цене" << endl;
            int num2;
            cin >> num2;
            cin.ignore();

            output << "=== Сортировка ===" << endl;

            if (num2 == 1) {
                sort(words.begin(), words.end(), [](const string& a, const string& b) {
                    size_t pos1a = a.find(",");
                    size_t pos2a = a.find(",", pos1a + 1);
                    string typea = a.substr(pos1a + 1, pos2a - pos1a - 1);

                    size_t pos1b = b.find(",");
                    size_t pos2b = b.find(",", pos1b + 1);
                    string typeb = b.substr(pos1b + 1, pos2b - pos1b - 1);

                    return typea < typeb;
                    });
                cout << "Отсортировано по типу:" << endl;
            }
            else {
                sort(words.begin(), words.end(), [](const string& a, const string& b) {
                    size_t pos1a = a.find(",");
                    size_t pos2a = a.find(",", pos1a + 1);
                    double pricea = stod(a.substr(pos2a + 1));

                    size_t pos1b = b.find(",");
                    size_t pos2b = b.find(",", pos1b + 1);
                    double priceb = stod(b.substr(pos2b + 1));

                    return pricea < priceb;
                    });
                cout << "Отсортировано по цене:" << endl;
            }

            ofstream newfile(fname);
            for (const string& dish : words) {
                cout << dish << endl;      
                output << dish << endl;    
                newfile << dish << endl;   
            }

            newfile.close();
            output.close();
            break;
        }

        case 4: {
            ifstream file(fname);
            ofstream output(fout, ios::app);
            double maprice;

            cout << "Введите максимальную цену: ";
            cin >> maprice;
            cin.ignore();

            output << "=== Блюда до " << maprice << " руб ===" << endl;

            string line;
            bool found = false;

            while (getline(file, line)) {
                size_t pos1 = line.find(",");
                size_t pos2 = line.find(",", pos1 + 1);
                string name = line.substr(0, pos1);
                string type = line.substr(pos1 + 1, pos2 - pos1 - 1);
                double price = stod(line.substr(pos2 + 1));

                if (price <= maprice) {
                    cout << name << " | " << type << " | " << price << " руб." << endl;
                    output << name << " | " << type << " | " << price << " руб." << endl;
                    found = true;
                }
            }

            if (!found) {
                cout << "Блюд не найдено!" << endl;
                output << "Блюд не найдено!" << endl;
            }

            file.close();
            output.close();
            break;
        }

        case 5: {
       
            ifstream file(fname);
            string line;

            cout << "=== Все блюда ===" << endl;

            while (getline(file, line)) {
                size_t pos1 = line.find(",");
                size_t pos2 = line.find(",", pos1 + 1);
                string name = line.substr(0, pos1);
                string type = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string price = line.substr(pos2 + 1);

                cout << name << " | " << type << " | " << price << " руб." << endl;
            }

            file.close();
            break;
        }

        case 0:
            cout << "Выход из программы!" << endl;
            break;
        }

    } while (num != 0);

    return 0;
}
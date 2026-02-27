// RK2_OAiP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// ВАРИАНТ 5

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <string>
#include <cctype>
#include <iomanip>
#include "StringInstrument.h"
#include "BrassInstrument.h"

using namespace std;

vector<shared_ptr<MusicalInstrument>> instruments;

void loadFromFile(const string& filename);
void saveToFile(const string& filename);
void displayAll();
void searchByName();
void filterByMaterial();
void filterByPriceRange();
void sortByPrice();
void sortByName();
void addInstrument();
void editInstrument();
void deleteInstrument();
void countStringInstruments();
void findMostExpensive();
void findLongestTubeBrass();
void filterWoodenExpensive();
void findMaxStringCount();
void sortByTuningAndPrice();
void showMenu();


void loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string type, name, material, param, ownersStr;
        double price;

        getline(ss, type, ';');
        getline(ss, name, ';');
        getline(ss, material, ';');
        ss >> price;
        ss.ignore();
        getline(ss, param, ';');
        getline(ss, ownersStr);

        vector<string> owners;
        istringstream ownersStream(ownersStr);
        string owner;
        while (getline(ownersStream, owner, '|')) {
            owners.push_back(owner);
        }

        if (type == "String") {
            size_t dashPos = param.find('-');
            int stringCount = stoi(param.substr(0, dashPos));
            string tuningType = param.substr(dashPos + 1);
            auto instr = make_shared<StringInstrument>(name, material, price, stringCount, tuningType);
            for (const auto& o : owners) instr->addOwner(o);
            instruments.push_back(instr);
        }
        else if (type == "Brass") {
            size_t dashPos = param.find('-');
            string brassType = param.substr(0, dashPos);
            int tubeLength = stoi(param.substr(dashPos + 1));
            auto instr = make_shared<BrassInstrument>(name, material, price, brassType, tubeLength);
            for (const auto& o : owners) instr->addOwner(o);
            instruments.push_back(instr);
        }
    }
    cout << "Данные загружены из " << filename << endl;
    file.close();
}

void saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка создания файла " << filename << endl;
        return;
    }

    for (const auto& instr : instruments) {
        file << (dynamic_cast<StringInstrument*>(instr.get()) ? "String" : "Brass") << ";";
        file << instr->getName() << ";";
        file << instr->getMaterial() << ";";
        file << instr->getPrice() << ";";

        if (auto s = dynamic_cast<StringInstrument*>(instr.get())) {
            file << s->getStringCount() << "-" << s->getTuningType();
        }
        else if (auto b = dynamic_cast<BrassInstrument*>(instr.get())) {
            file << b->getBrassType() << "-" << b->getTubeLength();
        }

        file << ";";
        const auto& owners = instr->getOwners();
        for (size_t i = 0; i < owners.size(); ++i) {
            file << owners[i];
            if (i < owners.size() - 1) file << "|";
        }
        file << endl;
    }
    cout << "Данные сохранены в " << filename << endl;
    file.close();
}

void displayAll() {
    if (instruments.empty()) {
        cout << "Список инструментов пуст." << endl;
        return;
    }
    for (const auto& instr : instruments) {
        instr->display();
    }
}

void searchByName() {
    cout << "Введите название инструмента: ";
    string name;
    getline(cin, name);
    bool found = false;
    for (const auto& instr : instruments) {
        if (instr->getName().find(name) != string::npos) {
            instr->display();
            found = true;
        }
    }
    if (!found) cout << "Инструменты не найдены." << endl;
}

void filterByMaterial() {
    cout << "Введите материал: ";
    string material;
    getline(cin, material);
    vector<shared_ptr<MusicalInstrument>> filtered;
    copy_if(instruments.begin(), instruments.end(), back_inserter(filtered),
        [&material](const shared_ptr<MusicalInstrument>& instr) {
            return instr->getMaterial() == material;
        });
    if (filtered.empty()) cout << "Инструменты не найдены." ;
    else for (const auto& instr : filtered) instr->display();
}

void filterByPriceRange() {
    double minPrice, maxPrice;
    cout << "Введите минимальную цену: ";
    cin >> minPrice;
    cout << "Введите максимальную цену: ";
    cin >> maxPrice;
    cin.ignore();
    vector<shared_ptr<MusicalInstrument>> filtered;
    copy_if(instruments.begin(), instruments.end(), back_inserter(filtered),
        [minPrice, maxPrice](const shared_ptr<MusicalInstrument>& instr) {
            double price = instr->getPrice();
            return price >= minPrice && price <= maxPrice;
        });
    if (filtered.empty()) cout << "Инструменты не найдены." << endl;
    else for (const auto& instr : filtered) instr->display();
}

void sortByPrice() {
    sort(instruments.begin(), instruments.end(),
        [](const shared_ptr<MusicalInstrument>& a, const shared_ptr<MusicalInstrument>& b) {
            return a->getPrice() < b->getPrice();
        });
    cout << "Сортировка по цене завершена." << endl;
}

void sortByName() {
    sort(instruments.begin(), instruments.end(),
        [](const shared_ptr<MusicalInstrument>& a, const shared_ptr<MusicalInstrument>& b) {
            return a->getName() < b->getName();
        });
    cout << "Сортировка по названию завершена." << endl;
}

void addInstrument() {
    cout << "Выберите тип (1 - Струнный, 2 - Духовой): ";
    int type;
    cin >> type;
    cin.ignore();

    string name, material;
    double price;
    cout << "Название: ";
    getline(cin, name);
    cout << "Материал: ";
    getline(cin, material);
    cout << "Цена: ";
    cin >> price;
    cin.ignore();

    if (type == 1) {
        int stringCount;
        string tuningType;
        cout << "Количество струн: ";
        cin >> stringCount;
        cin.ignore();
        cout << "Тип строя: ";
        getline(cin, tuningType);
        auto instr = make_shared<StringInstrument>(name, material, price, stringCount, tuningType);
        instruments.push_back(instr);
    }
    else if (type == 2) {
        string brassType;
        int tubeLength;
        cout << "Тип латуни: ";
        getline(cin, brassType);
        cout << "Длина трубы (см): ";
        cin >> tubeLength;
        cin.ignore();
        auto instr = make_shared<BrassInstrument>(name, material, price, brassType, tubeLength);
        instruments.push_back(instr);
    }
    cout << "Инструмент добавлен." << endl;
}

void editInstrument() {
    cout << "Введите номер инструмента для редактирования (начиная с 1): ";
    int index;
    cin >> index;
    cin.ignore();
    if (index < 1 || index > instruments.size()) {
        cout << "Неверный номер." << endl;
        return;
    }

    auto& instr = instruments[index - 1];
    cout << "Текущие данные:" << endl;
    instr->display();

    string name, material;
    double price;
    cout << "Новое название (оставьте пустым, если не хотите менять): ";
    getline(cin, name);
    
    cout << "Новый материал (оставьте пустым, если не хотите менять): ";
    getline(cin, material);
    cout << "Новая цена (введите 0, есои не хотите менять): ";
    cin >> price;
    cin.ignore();

    cout << "Добавить владельца? (1 - да, 0 - нет): ";
    int addOwner;
    cin >> addOwner;
    cin.ignore();
    if (addOwner == 1) {
        cout << "Имя владельца: ";
        string owner;
        getline(cin, owner);
        instr->addOwner(owner);
    }
    cout << "Изменения сохранены." << endl;
}

void deleteInstrument() {
    cout << "Введите номер инструмента для удаления (начиная с 1): ";
    int index;
    cin >> index;
    cin.ignore();
    if (index < 1 || index > instruments.size()) {
        cout << "Неверный номер." << endl;
        return;
    }
    instruments.erase(instruments.begin() + index - 1);
    cout << "Инструмент удалён." << endl;
}

void countStringInstruments() {
    int count = count_if(instruments.begin(), instruments.end(),
        [](const shared_ptr<MusicalInstrument>& instr) {
            auto s = dynamic_cast<StringInstrument*>(instr.get());
            return s && s->getStringCount() > 6;
        });
    cout << "Количество струнных инструментов с более чем 6 струнами: " << count << endl;
}

void findMostExpensive() {
    if (instruments.empty()) {
        cout << "Список пуст." << endl;
        return;
    }
    auto it = max_element(instruments.begin(), instruments.end(),
        [](const shared_ptr<MusicalInstrument>& a, const shared_ptr<MusicalInstrument>& b) {
            return a->getPrice() < b->getPrice();
        });
    cout << "Самый дорогой инструмент:" << endl;
    (*it)->display();
}

void findLongestTubeBrass() {
    vector<shared_ptr<MusicalInstrument>> brass;
    copy_if(instruments.begin(), instruments.end(), back_inserter(brass),
        [](const shared_ptr<MusicalInstrument>& instr) {
            return dynamic_cast<BrassInstrument*>(instr.get()) != nullptr;
        });
    if (brass.empty()) {
        cout << "Духовых инструментов нет." << endl;
        return;
    }
    auto it = max_element(brass.begin(), brass.end(),
        [](const shared_ptr<MusicalInstrument>& a, const shared_ptr<MusicalInstrument>& b) {
            return dynamic_cast<BrassInstrument*>(a.get())->getTubeLength() <
                dynamic_cast<BrassInstrument*>(b.get())->getTubeLength();
        });
    cout << "Духовой инструмент с самой длинной трубой:" << endl;
    (*it)->display();
}

void filterWoodenExpensive() {
    cout << "Инструменты из дерева дороже 600:" << endl;
    vector<shared_ptr<MusicalInstrument>> filtered;
    copy_if(instruments.begin(), instruments.end(), back_inserter(filtered),
        [](const shared_ptr<MusicalInstrument>& instr) {
            return instr->getMaterial() == "Wood" && instr->getPrice() > 600;
        });
    if (filtered.empty()) cout << "Не найдено." << endl;
    else for (const auto& instr : filtered) instr->display();
}

void findMaxStringCount() {
    vector<shared_ptr<MusicalInstrument>> stringInstr;
    copy_if(instruments.begin(), instruments.end(), back_inserter(stringInstr),
        [](const shared_ptr<MusicalInstrument>& instr) {
            return dynamic_cast<StringInstrument*>(instr.get()) != nullptr;
        });
    if (stringInstr.empty()) {
        cout << "Струнных инструментов нет." << endl;
        return;
    }
    auto it = max_element(stringInstr.begin(), stringInstr.end(),
        [](const shared_ptr<MusicalInstrument>& a, const shared_ptr<MusicalInstrument>& b) {
            return dynamic_cast<StringInstrument*>(a.get())->getStringCount() <
                dynamic_cast<StringInstrument*>(b.get())->getStringCount();
        });
    cout << "Струнный инструмент с максимальным количеством струн:" << endl;
    (*it)->display();
}

void sortByTuningAndPrice() {
    vector<shared_ptr<MusicalInstrument>> stringInstr;
    copy_if(instruments.begin(), instruments.end(), back_inserter(stringInstr),
        [](const shared_ptr<MusicalInstrument>& instr) {
            return dynamic_cast<StringInstrument*>(instr.get()) != nullptr;
        });
    sort(stringInstr.begin(), stringInstr.end(),
        [](const shared_ptr<MusicalInstrument>& a, const shared_ptr<MusicalInstrument>& b) {
            auto s1 = dynamic_cast<StringInstrument*>(a.get());
            auto s2 = dynamic_cast<StringInstrument*>(b.get());
            if (s1->getTuningType() != s2->getTuningType())
                return s1->getTuningType() < s2->getTuningType();
            return s1->getPrice() < s2->getPrice();
        });
    cout << "Струнные инструменты отсортированы по типу строя и цене:" << endl;
    for (const auto& instr : stringInstr) instr->display();
}

void showMenu() {
    cout << "\n=== Меню управления каталогом музыкальных инструментов ===" << endl;
    cout << "1. Загрузить данные из файла" << endl;
    cout << "2. Вывести все инструменты" << endl;
    cout << "3. Поиск по названию" << endl;
    cout << "4. Фильтр по материалу" << endl;
    cout << "5. Фильтр по диапазону цен" << endl;
    cout << "6. Сортировка по цене" << endl;
    cout << "7. Сортировка по названию" << endl;
    cout << "8. Добавить новый инструмент" << endl;
    cout << "9. Редактировать инструмент" << endl;
    cout << "10. Удалить инструмент" << endl;
    cout << "11. Сохранить в файл" << endl;
    cout << "12. Количество струнных с >6 струнами" << endl;
    cout << "13. Самый дорогой инструмент" << endl;
    cout << "14. Духовой с самой длинной трубой" << endl;
    cout << "15. Инструменты из дерева дороже 600" << endl;
    cout << "16. Струнный с макс. количеством струн" << endl;
    cout << "17. Сортировка струнных по строю и цене" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

int main() {
    

    int choice;
    do {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            cout << "Введите имя файла (по умолчанию catalog.txt): ";
            string filename;
            getline(cin, filename);
            if (filename.empty()) filename = "catalog.txt";
            loadFromFile(filename);
            break;
        }
        case 2:
            displayAll();
            break;
        case 3:
            searchByName();
            break;
        case 4:
            filterByMaterial();
            break;
        case 5:
            filterByPriceRange();
            break;
        case 6:
            sortByPrice();
            break;
        case 7:
            sortByName();
            break;
        case 8:
            addInstrument();
            break;
        case 9:
            editInstrument();
            break;
        case 10:
            deleteInstrument();
            break;
        case 11: {
            cout << "Введите имя файла (по умолчанию catalog_saved.txt): ";
            string filename;
            getline(cin, filename);
            if (filename.empty()) filename = "catalog_saved.txt";
            saveToFile(filename);
            break;
        }
        case 12:
            countStringInstruments();
            break;
        case 13:
            findMostExpensive();
            break;
        case 14:
            findLongestTubeBrass();
            break;
        case 15:
            filterWoodenExpensive();
            break;
        case 16:
            findMaxStringCount();
            break;
        case 17:
            sortByTuningAndPrice();
            break;
        case 0:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    } while (choice != 0);

    return 0;
}
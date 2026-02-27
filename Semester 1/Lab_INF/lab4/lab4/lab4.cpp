// lab4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include "Database.h"
#include "Pair.h"

using namespace std;

void mainMenu();
void CatMenu(Database& db);
void DishMenu(Database& db);
void OrderMenu(Database& db);
void AnaliticaMenu(Database& db);

int main() {
    Database db;
    if (db.connect()) {
        cout << "Приятного использования!" << endl;
    }
    else {
        cout << "Попробуйте еще раз!" << endl;
        return 1;
    }

    while (true) {
        mainMenu();
        int num;
        cin >> num;

        switch (num) {
        case 1:
            CatMenu(db);
            break;
        case 2:
            DishMenu(db);
            break;
        case 3:
            OrderMenu(db);
            break;
        case 4:
            AnaliticaMenu(db);
            break;
        case 5:
            cout << endl;
            cout << "Выход из программы...." << endl;
            return 0;
        default:
            cout << "Неверный выбор! Попробуйте снова" << endl;
        }
        cout << endl;
        cout << "Нажмите Enter для продолжения...";
        cin.ignore();
        cin.get();
    }

    return 0;
}

void mainMenu() {
    cout << endl;
    cout << "=== ГЛАВНОЕ МЕНЮ ===" << endl;
    cout << "1. Управление категориями" << endl;
    cout << "2. Управление блюдами" << endl;
    cout << "3. Управление заказами" << endl;
    cout << "4. Аналитические отчеты" << endl;
    cout << "5. Выход" << endl;
    cout << "Выберите действие: ";
}

void CatMenu(Database& db) {
    cout << endl;
    cout << "=== КАТЕГОРИИ ===" << endl;
    cout << "1. Добавить категорию" << endl;
    cout << "2. Показать все категории" << endl;
    cout << "3. Назад" << endl;
    cout << "Выберите: ";

    int num;
    cin >> num;

    if (num == 1) {
        cout << "Введите название категории: ";
        string name;
        cin.ignore();
        getline(cin, name);

        if (db.addCategory(name)) {
            cout << "Категория добавлена!" << endl;
        }
        else {
            cout << "Ошибка при добавлении категории" << endl;
        }
    }
    else if (num == 2) {
        cout << endl;
        cout << "Список категорий:" << endl;

        auto categories = db.getAllCategories();
        if (categories.empty()) {
            cout << "Категорий нет" << endl;
        }
        else {
            for (const auto& cat : categories) {
                cout << cat.first << ". " << cat.second << endl;
            }
        }
    }
}

void DishMenu(Database& db) {
    cout << endl;
    cout << "=== БЛЮДА ===" << endl;
    cout << "1. Добавить блюдо" << endl;
    cout << "2. Показать все блюда" << endl;
    cout << "3. Назад" << endl;
    cout << "Выберите: ";

    int num;
    cin >> num;

    if (num == 1) {
        cout << "Существующие категории:" << endl;
        auto categories = db.getAllCategories();
        for (const auto& cat : categories) {
            cout << cat.first << ". " << cat.second << endl;
        }

        cout << "Название блюда: ";
        string name;
        cin.ignore();
        getline(cin, name);

        cout << "ID категории: ";
        int catId;
        cin >> catId;

        cout << "Цена: ";
        double price;
        cin >> price;

        if (db.addDish(name, catId, price, "MainCourse")) {
            cout << "Блюдо добавлено!" << endl;
        }
        else {
            cout << "Ошибка при добавлении блюда" << endl;
        }
    }
    else if (num == 2) {
        cout << endl;
        cout << "Меню ресторана:" << endl;

        auto dishes = db.getAllDishes();
        if (dishes.empty()) {
            cout << "Блюд нет" << endl;
        }
        else {
            for (const auto& dish : dishes) {
                cout << get<0>(dish) << ". " << get<1>(dish)
                    << " (Категория: " << get<2>(dish)
                    << ") - " << get<3>(dish) << " руб" << endl;
            }
        }
    }
}

void OrderMenu(Database& db) {
    cout << endl;
    cout << "=== ЗАКАЗЫ ===" << endl;
    cout << "1. Создать заказ" << endl;
    cout << "2. Показать все заказы" << endl;
    cout << "3. Назад" << endl;
    cout << "Выберите: ";

    int num;
    cin >> num;

    if (num == 1) {
        
        cout << "Доступные блюда:" << endl;
        auto dishes = db.getAllDishes();
        if (dishes.empty()) {
            cout << "Нет доступных блюд" << endl;
            return;
        }

        for (const auto& dish : dishes) {
            cout << get<0>(dish) << ". " << get<1>(dish)
                << " (" << get<3>(dish) << " руб)" << endl;
        }

        cout << "Выберите ID блюда: ";
        int dishId;
        cin >> dishId;

        cout << "Количество: ";
        int quantity;
        cin >> quantity;

        
        double price = 0.0;
        for (const auto& dish : dishes) {
            if (get<0>(dish) == dishId) {
                price = get<3>(dish);
                break;
            }
        }

        if (price > 0) {
            double totalPrice = price * quantity;
            if (db.addOrder(dishId, quantity, totalPrice)) {
                cout << "Заказ создан на сумму: " << totalPrice << " руб!" << endl;
            }
            else {
                cout << "Ошибка при создании заказа" << endl;
            }
        }
        else {
            cout << "Блюдо с таким ID не найдено" << endl;
        }
    }
    else if (num == 2) {
        cout << endl;
        cout << "Все заказы:" << endl;

        auto orders = db.getAllOrders();  
        if (orders.empty()) {
            cout << "Заказов нет" << endl;
        }
        else {
            for (const auto& order : orders) {
                int orderId = get<0>(order);       
                int dishId = get<1>(order);        
                string date = get<2>(order);       
                int quantity = get<3>(order);      
                double total = get<4>(order);      

                cout << "Заказ #" << orderId
                    << " | Блюдо ID: " << dishId
                    << " | Дата: " << date
                    << " | Кол-во: " << quantity
                    << " | Сумма: " << total << " руб." << endl;
            }
        }
    }
}

void AnaliticaMenu(Database& db) {
    cout << endl;
    cout << "=== АНАЛИТИЧЕСКИЕ ОТЧЕТЫ ===" << endl;
    cout << "1. Выручка по категориям" << endl;
    cout << "2. Топ-3 самых продаваемых блюда" << endl;
    cout << "3. Средняя стоимость заказа" << endl;
    cout << "4. Количество заказов по блюдам" << endl;
    cout << "5. Демонстрация шаблонного класса Pair" << endl;
    cout << "6. Назад" << endl;
    cout << "Выберите: ";

    int num;
    cin >> num;

    switch (num) {
    case 1: {
        cout << endl;
        cout << "--- Выручка по категориям ---" << endl;

        auto revenue = db.getRevenueByCategory();
        if (revenue.empty()) {
            cout << "Нет данных о заказах" << endl;
        }
        else {
            for (const auto& item : revenue) {
                cout << item.first << ": " << item.second << " руб" << endl;
            }
        }
        break;
    }

    case 2: {
        cout << endl;
        cout << "--- Топ-3 самых продаваемых блюда ---" << endl;

        auto topDishes = db.getTopSellingDishes(3);
        if (topDishes.empty()) {
            cout << "Нет данных о заказах" << endl;
        }
        else {
            for (size_t i = 0; i < topDishes.size(); i++) {
                cout << i + 1 << ". " << topDishes[i].first
                    << " - " << topDishes[i].second << " порций" << endl;
            }
        }
        break;
    }

    case 3: {
        cout << endl;
        cout << "--- Средняя стоимость заказа ---" << endl;

        double average = db.getAverageOrderValue();
        if (average > 0) {
            cout << "Средняя стоимость заказа: " << average << " руб" << endl;
        }
        else {
            cout << "Нет данных о заказах" << endl;
        }
        break;
    }

    case 4: {
        cout << endl;
        cout << "--- Количество заказов по блюдам ---" << endl;

        auto dishCounts = db.getDishOrderCount();
        if (dishCounts.empty()) {
            cout << "Нет данных" << endl;
        }
        else {
            vector<Pair<string, int>> dishPairs;
            for (const auto& item : dishCounts) {
                dishPairs.push_back(Pair<string, int>(item.first, item.second));
            }

            sort(dishPairs.begin(), dishPairs.end(),
                [](const Pair<string, int>& a, const Pair<string, int>& b) {
                    return b.getSecond() < a.getSecond();
                });

            for (const auto& pair : dishPairs) {
                cout << pair.getFirst() << ": " << pair.getSecond() << " порций" << endl;
            }
        }
        break;
    }

    case 5: {
        cout << endl;
        cout << "=== Демонстрация шаблонного класса Pair ===" << endl;
        cout << endl;

        auto dishCounts = db.getDishOrderCount();

        if (!dishCounts.empty()) {
            Pair<string, int> Pair1(dishCounts[0].first, dishCounts[0].second);
            cout << "Пример 1: ";
            Pair1.display();
        }

        Pair<string, int> Pair2("Стейк", 5);
        cout << "Пример 2: ";
        Pair2.display();


        break;
    }

    case 6:
        return;
    default:
        cout << "Неверный выбор!" << endl;
    }
}
// lab2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Вариант 4

// strcpy заменяем на strcpy_s, из за предупреждений майкрософта, как безопасность

#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

int main() {
    char text[100]; 
    char words[10][50];
    int wordc = 0;

    cout << "Введите строку: ";
    cin.getline(text, 100); 

    
    char* ptr = text;
    int ind = 0;

    while (*ptr != '\0') { // символ NULL
        if (*ptr != ' ') {
            words[wordc][ind] = *ptr;
            ind++;
        }
        else {
            words[wordc][ind] = '\0'; // добавляем NULL
            wordc++;
            ind = 0;
        }
        ptr++; // перемещаем указатель
    }

    // последнее слово
    if (ind > 0) {
        words[wordc][ind] = '\0';
        wordc++;
    }

    cout << "Слова, оканчивающиеся на b: ";
    for (int i = 0; i < wordc; i++) {
        int len = strlen(words[i]); // strlen() из теории
        if (words[i][len - 1] == 'b') {
            cout << words[i] << " ";
        }
    }
    cout << endl;
    // Самое длинное слово и его длина
    char maword[50]; 
    strcpy_s(maword, words[0]); // копируем
    size_t madlina = strlen(maword); 

    for (int i = 0; i < wordc; i++) { 
        if (strlen(words[i]) > madlina) { 
            madlina = strlen(words[i]);
            strcpy_s(maword, words[i]); // копируем
        }
    }

    cout << "Самое длинное слово: " << maword
        << ", его длина: " << madlina << endl;

    // Подсчет количества букв d в последнем слове
    if (wordc > 0) {
        char* lword = words[wordc - 1];
        int count_d = 0;

        for (int i = 0; lword[i] != '\0'; i++) {
            if (lword[i] == 'd') {
                count_d++;
            }
        }
        cout << "Количество букв d в последнем слове " << lword << ": " << count_d << endl;
    }
    
    
    // Замена всех строчных букв на заглавные
    cout << "Строка в верхнем регистре: ";

    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') { 
            cout << (char)(text[i] - 32); // 
        }
        else {
            cout << text[i]; // остальные символы без изменений
        }
    }
    cout << endl;

    
    cout << "Слова, у которых совпадает второй и предпоследний символ: ";
    int coun = 0;
    for (int i = 0; i < wordc; i++) {
        int len = strlen(words[i]);
        if (len >= 4) { // нужно минимум 4 символа чтобы был второй и предпоследний
            if (words[i][1] == words[i][len - 2]) { // сравниваем второй и предпоследний символ
                cout << words[i] << " ";
                coun++;
            }
        }
    }
    cout << endl << "Количество таких слов: " << coun << endl;
    
    // Найти самую длинную общую подстроку 
    if (wordc >= 2) {
        char longpodstr[50] = "";
        int mapoddlin = 0;
        char w1[50], w2[50];

        for (int i = 0; i < wordc - 1; i++) {
            for (int j = i + 1; j < wordc; j++) {
                char *word1 = words[i];
                char *word2 = words[j];
                int len1 = strlen(word1);
                int len2 = strlen(word2);

                // общие подстроки
                for (int len = len1; len > 0; len--) { 
                    if (len <= mapoddlin) break;

                    for (int pos1 = 0; pos1 <= len1 - len; pos1++) {
                        for (int pos2 = 0; pos2 <= len2 - len; pos2++) {
                            // сравниваем
                            bool iscool = true;
                            for (int k = 0; k < len; k++) {
                                if (word1[pos1 + k] != word2[pos2 + k]) {
                                    iscool = false;
                                    break;
                                }
                            }

                            if (iscool && len > mapoddlin) {
                                mapoddlin = len;
                                strncpy_s(longpodstr, word1 + pos1, len);
                                strcpy_s(w1, word1);
                                strcpy_s(w2, word2);
                            }
                        }
                    }
                }
            }
        }

        if (mapoddlin > 0) {
            cout << "Самая длинная общая подстрока: " << longpodstr << " , её длина: " << mapoddlin << endl;
            cout << "Найдена между словами: " << w1 << " и " << w2 << endl;
        }
        else {
            cout << "Общих подстрок не найдено" << endl;
        }
    }


}
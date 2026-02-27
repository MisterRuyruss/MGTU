// Подсчет гласных

#include <iostream>
#include <string>
using namespace std;

int main()
{
    
    char gl[20] = {'а','о','у','ы','э','я','ю','ё','и','е','А','О','У','Ы','Э','Я','Ю','Ё','И','Е'};
    int count = 0;
    string n;
    cout << "Ваше слово: ";
    getline(cin, n);
    for (int j = 0; j < 20; j++) {
        for (int i = 0; i < n.length(); i++) {
            if (n[i] == gl[j]) {
                count++;
            }
        }
    }
    cout << count << " гласных";
    
}


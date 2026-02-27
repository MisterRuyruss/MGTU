#include "StringInstrument.h"
#include <fstream>
#include <sstream>

using namespace std;

StringInstrument::StringInstrument(const string& name, const string& material, double price, int stringCount, const string& tuningType)
    : MusicalInstrument(name, material, price), stringCount(stringCount), tuningType(tuningType) {
}

int StringInstrument::getStringCount() const {
    return stringCount;
}

string StringInstrument::getTuningType() const {
    return tuningType;
}

void StringInstrument::display() const {
    cout << "(Струнный инструмент) ";
    MusicalInstrument::display();
    cout << "  Количество струн: " << stringCount << ", Тип строя: " << tuningType << endl;
}

void StringInstrument::saveToFile(ofstream& out) const {
    MusicalInstrument::saveToFile(out);
    out << ";" << stringCount << "-" << tuningType;
}

void StringInstrument::loadFromFile(ifstream& in) {
    MusicalInstrument::loadFromFile(in);
    string param;
    if (getline(in, param, ';')) {
        size_t dashPos = param.find('-');
        if (dashPos != string::npos) {
            stringCount = stoi(param.substr(0, dashPos));
            tuningType = param.substr(dashPos + 1);
        }
    }
}
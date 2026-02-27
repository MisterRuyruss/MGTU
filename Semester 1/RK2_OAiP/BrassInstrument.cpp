#include "BrassInstrument.h"
#include <fstream>
#include <sstream>

using namespace std;

BrassInstrument::BrassInstrument(const string& name, const string& material, double price, const string& brassType, int tubeLength)
    : MusicalInstrument(name, material, price), brassType(brassType), tubeLength(tubeLength) {
}

string BrassInstrument::getBrassType() const {
    return brassType;
}

int BrassInstrument::getTubeLength() const {
    return tubeLength;
}

void BrassInstrument::display() const {
    cout << "(Духовой инструмент) ";
    MusicalInstrument::display();
    cout << "  Тип латуни: " << brassType << ", Длина трубы: " << tubeLength << " см" << endl;
}

void BrassInstrument::saveToFile(ofstream& out) const {
    MusicalInstrument::saveToFile(out);
    out << ";" << brassType << "-" << tubeLength;
}

void BrassInstrument::loadFromFile(ifstream& in) {
    MusicalInstrument::loadFromFile(in);
    string param;
    if (getline(in, param, ';')) {
        size_t dashPos = param.find('-');
        if (dashPos != string::npos) {
            brassType = param.substr(0, dashPos);
            tubeLength = stoi(param.substr(dashPos + 1));
        }
    }
}
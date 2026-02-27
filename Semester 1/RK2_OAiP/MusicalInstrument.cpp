#include "MusicalInstrument.h"
#include <fstream>
#include <sstream>

using namespace std;

MusicalInstrument::MusicalInstrument(const string& name, const string& material, double price)
    : name(name), material(material), price(price) {
}

string MusicalInstrument::getName() const {
    return name;
}

string MusicalInstrument::getMaterial() const {
    return material;
}

double MusicalInstrument::getPrice() const {
    return price;
}

const vector<string>& MusicalInstrument::getOwners() const {
    return owners;
}

void MusicalInstrument::addOwner(const string& owner) {
    owners.push_back(owner);
}

void MusicalInstrument::display() const {
    cout << "Название: " << name << ", Материал: " << material
        << ", Цена: " << price << ", Владельцы: ";
    for (const auto& owner : owners) {
        cout << owner << " ";
    }
    cout << endl;
}

void MusicalInstrument::saveToFile(ofstream& out) const {
    out << name << ";" << material << ";" << price;
    for (const auto& owner : owners) {
        out << "|" << owner;
    }
}

void MusicalInstrument::loadFromFile(ifstream& in) {
    string line;
    if (getline(in, line)) {
        istringstream ss(line);
        getline(ss, name, ';');
        getline(ss, material, ';');
        ss >> price;
        ss.ignore();
        string owner;
        owners.clear();
        while (getline(ss, owner, '|')) {
            owners.push_back(owner);
        }
    }
}
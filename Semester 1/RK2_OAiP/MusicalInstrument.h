#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class MusicalInstrument {
protected:
    string name;
    string material;
    double price;
    vector<string> owners; 

public:
    MusicalInstrument(const string& name, const string& material, double price);
    virtual ~MusicalInstrument() = default;

    string getName() const;
    string getMaterial() const;
    double getPrice() const;
    const vector<string>& getOwners() const;

    void addOwner(const string& owner);

    virtual void display() const;

    virtual void saveToFile(ofstream& out) const;
    virtual void loadFromFile(ifstream& in);
};
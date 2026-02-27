#pragma once
#include "MusicalInstrument.h"
#include <string>

using namespace std;

class BrassInstrument : public MusicalInstrument {
private:
    string brassType;   
    int tubeLength;         

public:
    BrassInstrument(const string& name, const string& material, double price,
        const string& brassType, int tubeLength);

    string getBrassType() const;
    int getTubeLength() const;

    void display() const override;
    void saveToFile(ofstream& out) const override;
    void loadFromFile(ifstream& in) override;
};
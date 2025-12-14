#pragma once
#include "MusicalInstrument.h"
#include <string>

using namespace std;

class StringInstrument : public MusicalInstrument {
private:
    int stringCount;    
    string tuningType; 

public:
    StringInstrument(const string& name, const string& material, double price,
        int stringCount, const string& tuningType);

    int getStringCount() const;
    string getTuningType() const;

    void display() const override; 
    void saveToFile(ofstream& out) const override;
    void loadFromFile(ifstream& in) override;
};
#ifndef PAIR_H
#define PAIR_H

#include <iostream>
#include <string>

template<typename T1, typename T2>
class Pair {
private:
    T1 first;   
    T2 second;  

public:
    
    Pair(const T1& f, const T2& s) : first(f), second(s) {}

    
    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }

    
    void display() const {
        std::cout << first << " -> " << second << " порций" << std::endl;
    }
};

#endif
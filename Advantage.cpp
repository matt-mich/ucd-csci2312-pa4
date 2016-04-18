//
// Created by Matthew Michaelis on 3/31/2016.
//

#include <sstream>
#include "Advantage.h"

namespace Gaming{
    const char Advantage::ADVANTAGE_ID = 'D';
    const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;

Advantage::Advantage(const Game &g, const Position &p, double capacity) : Resource(g,p,capacity){

}
Advantage::~Advantage(){

}

void Advantage::print(std::ostream &os) const{
    std::stringstream temp;
    std::string tempS;
    temp << ADVANTAGE_ID;
    temp << __id;
    temp >> tempS;
    os << tempS;
}

double Advantage::getCapacity() const{
    return __capacity * ADVANTAGE_MULT_FACTOR;
}

double Advantage::consume(){
    double temp = __capacity;
    __capacity = 0;
    return temp;
}
}

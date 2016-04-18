//
// Created by The on 3/31/2016.
//

#include <sstream>
#include "Food.h"


namespace Gaming {
    const char Food::FOOD_ID = 'F';
    Food::Food(const Game &g, const Position &p, double capacity) : Resource(g,p,capacity) {

    }

    Food::~Food(){

    }

    void Food::print(std::ostream &os) const{
        std::stringstream temp;
        std::string tempS;
        temp << FOOD_ID;
        temp << __id;
        temp >> tempS;
        os << tempS;
    }
}


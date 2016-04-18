//
// Created by Matthew Michaelis on 3/31/2016.
//

#include <sstream>
#include "Strategic.h"

namespace Gaming {

    const char Strategic::STRATEGIC_ID = 'T';

    Strategic::Strategic(const Game &g, const Position &p, double energy, Strategy *s) : Agent(g, p, energy) {
        __strategy = s;
    }

    Strategic::~Strategic() {

    }

    void Strategic::print(std::ostream &os) const {
        std::stringstream temp;
        std::string tempS;
        temp << STRATEGIC_ID;
        temp << __id;
        temp >> tempS;
        os << tempS;
    }

    ActionType Strategic::takeTurn(const Surroundings &s) const {
        return (*__strategy)(s);
    }
}
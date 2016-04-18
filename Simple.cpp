//
// Created by Matthew Michaelis on 3/31/2016.
//

#include <vector>
#include <sstream>
#include "Simple.h"


namespace Gaming{

    const char Simple::SIMPLE_ID = 'S';

    Simple::Simple(const Game &g, const Position &p, double energy) : Agent(g,p,energy){

    }
    Simple::~Simple(){

    }



    void Simple::print(std::ostream &os) const{
        std::stringstream temp;
        std::string tempS;
        temp << SIMPLE_ID;
        temp << __id;
        temp >> tempS;
        os << tempS;
    }

    ActionType Simple::takeTurn(const Surroundings &s) const{
        ActionType arr[9] = {NW,N,NE,W,STAY,E,SE,S,SW};
        std::vector<int> rec;
        std::vector<int> empty;
        PositionRandomizer rand;
        int j = 0;
        for(int i = 0; i < 9; ++i){
            if(s.array[i] == FOOD || s.array[i] == ADVANTAGE){
                rec.push_back(i);
            }
        }
        if(rec.size() > 0){
            Position i = rand(rec);
            int j = ((i.x*3)+(i.y));
            return arr[j];
        }

        for(int i = 0; i < 9; ++i) {
            if(s.array[i] == EMPTY){
                empty.push_back(i);
            }
        }
        if(empty.size() > 0){
            Position i = rand(empty);
            int j = ((i.x*3)+(i.y));
            return arr[j];
        }else{
            return STAY;
        }
    }
}
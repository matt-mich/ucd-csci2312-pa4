//
// Created by Matthew Michaelis on 3/31/2016.
//

#include "Resource.h"
#include "Agent.h"
namespace Gaming {
    const double Resource::RESOURCE_SPOIL_FACTOR = 1.2;

    Resource::Resource(const Game &g, const Position &p, double capacity) : Piece(g,p){
        __capacity = capacity;
    }

    Resource::~Resource(){

    }

    double Resource::consume(){
        double temp = __capacity;
        __capacity = 0;
        return temp;
    }

    void Resource::age(){
        if(__capacity < 0.01){
            __capacity = 0;
        }
        __capacity  = __capacity / RESOURCE_SPOIL_FACTOR;
    }

    ActionType Resource::takeTurn(const Surroundings &s) const{
        return STAY;
    }

// note: these won't be called while resources don't move
    Piece &Resource::operator*(Piece &other){
        return other.interact(this);
    }
    Piece &Resource::interact(Agent *other){
        double energy = consume();
        other->addEnergy(energy);
        return *this;
    }
    Piece &Resource::interact(Resource *other){ // note: no interaction between resources
        return *this;
    }
}

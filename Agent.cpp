//
// Created by The on 3/31/2016.
//
#include "Agent.h"
#include "Resource.h"
namespace Gaming{

    const double Agent::AGENT_FATIGUE_RATE = 0.3;

    Agent::Agent(const Game &g, const Position &p, double energy) : Piece(g,p){
    __energy = energy;
    }

    Agent::~Agent(){

    }

    void Agent::age(){
        __energy -= AGENT_FATIGUE_RATE;
    }
    Piece &Agent::operator*(Piece &other){

    }
    Piece &Agent::interact(Agent *other){

        return *other;
    }
    Piece &Agent::interact(Resource *other){
        __energy += other->consume();

    }
}

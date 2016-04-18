//
// Created by Matthew Michaelis on 3/31/2016.
//

#include "AggressiveAgentStrategy.h"
#include "Game.h"

namespace Gaming{

    const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;

    AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy){
        __agentEnergy = agentEnergy;
    }
    AggressiveAgentStrategy::~AggressiveAgentStrategy(){

    }
    ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const{
        ActionType arr[9] = {NW, N, NE, W, STAY, E, SE, S, SW};

        for (int i = 0; i < 9; ++i) {
            if (s.array[i] == SIMPLE) {
                if(DEFAULT_AGGRESSION_THRESHOLD <= __agentEnergy){
                    return arr[i];
                }
            }
        }
        for (int i = 0; i < 9; ++i) {
            if (s.array[i] == STRATEGIC) {
                if(DEFAULT_AGGRESSION_THRESHOLD <= __agentEnergy){
                    return arr[i];
                }
            }
        }
        for (int i = 0; i < 9; ++i) {
            if (s.array[i] == ADVANTAGE) {
                return arr[i];
            }
        }
        for (int i = 0; i < 9; ++i) {
            if (s.array[i] == FOOD) {
                return arr[i];
            }
        }
        for (int i = 0; i < 9; ++i) {
            if (s.array[i] == EMPTY) {
                return arr[i];
            }
        }
        return STAY;
    }
}

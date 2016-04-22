//
// Created by The on 3/31/2016.
//

#include "DefaultAgentStrategy.h"

namespace Gaming{
    DefaultAgentStrategy::DefaultAgentStrategy(){

    }
    DefaultAgentStrategy::~DefaultAgentStrategy(){

    }

    ActionType DefaultAgentStrategy::operator()(const Surroundings &s) const{
        ActionType arr[9] = {NW, N, NE, W, STAY, E, SW, S, SE};

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
        for (int i = 0; i < 9; ++i) {
            if (s.array[i] == SIMPLE) {
                return arr[i];
            }
        }
        return STAY;
    }
}
//
// Created by The on 3/31/2016.
//

#include "Piece.h"

namespace Gaming {

    unsigned int Piece::__idGen = 0;

    Piece::Piece(const Game &g, const Position &p) : __game(g), __position(p){
        __id = __idGen++;
    }
    Piece::~Piece() {

    }
    std::ostream &operator<<(std::ostream &os, const Piece &piece){
        piece.print(os);
        return os;
    }
}

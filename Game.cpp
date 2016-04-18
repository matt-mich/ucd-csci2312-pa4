//
// Created by The on 3/31/2016.
//

#include "Game.h"
#include "Piece.h"
#include "Simple.h"
#include "Food.h"
#include "Strategic.h"
#include "Advantage.h"
#include <iomanip>

namespace Gaming {

    PositionRandomizer Game::__posRandomizer;

    void Game::populate() {

        __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;
        unsigned int numStrategic = __numInitAgents / 2;
        unsigned int numSimple = __numInitAgents - numStrategic;
        unsigned int numAdvantages = __numInitResources / 4;
        unsigned int numFoods = __numInitResources - numAdvantages;

        // populate the grid (used in automatic random initialization of a Game)
        // simple pseudo-random number generator
        // sufficient for our casual purposes
        std::default_random_engine gen;
        std::uniform_int_distribution<int> d(0, __width * __height);

        // populate Strategic agents
        while (numStrategic > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Strategic(*this, pos, Game::STARTING_AGENT_ENERGY);
                numStrategic--;
            }
        }
        while (numSimple > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Simple(*this, pos, Game::STARTING_AGENT_ENERGY);
                numSimple--;
            }
        }
        while (numFoods > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Food(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
                numFoods--;
            }
        }
        while (numAdvantages > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Advantage(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
                numAdvantages--;
                }
            }
        }

    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned Game::MIN_WIDTH = 3;
    const unsigned Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;


    Game::Game() {
        __width = MIN_WIDTH;
        __height = MIN_HEIGHT;
        __numInitAgents = 0;
        __numInitResources = 0;
        __round = 0;
        __verbose = false;
        __status = NOT_STARTED;
        for (int i = 0; i < __height; ++i) {
            for (int j = 0; j < __width; ++j) {
                __grid.push_back(nullptr);
            }
        }
    }

    Game::Game(unsigned width, unsigned height, bool manual) { // note: manual population by default
        __round = 0;
        __numInitAgents = 0;
        __numInitResources = 0;
        __round = 0;
        __verbose = false;
        if (width >= MIN_WIDTH) {
            __width = width;
        } else {
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
        }

        if (height >= MIN_HEIGHT) {
            __height = height;
        } else {
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
        }
        for (int i = 0; i < __height; ++i) {
            for (int j = 0; j < __width; ++j) {
                __grid.push_back(nullptr);
            }
        }
        if (!manual) {
            populate();
        }

    }

    Game::~Game() {
        for(int i = 0; i < __height*__width; i++){
            delete __grid[i];
        }
    }

    // getters
    unsigned int Game::getNumPieces() const {
        unsigned int total = 0;
        for (int i = 0; i < __height; ++i) {
            for (int j = 0; j < __width; ++j) {
                if (__grid[(i * __width) + j] != nullptr) {
                    ++total;
                }
            }
        }
        return total;
    }


    // Currently biased towards the top right pieces, but I couldn't get std::set working.
    void Game::round() {   // play a single round
        for(int i = 0; i < __height*__width; ++i){
            if(__grid[i] != nullptr){
                if(!__grid[i]->getTurned()){
                    __grid[i]->age();
                    __grid[i]->setTurned(true);
                    if(!__grid[i]->isViable()){
                        delete __grid[i];
                        __grid[i] = nullptr;
                    }else{
                        Position pos = __grid[i]->getPosition();
                        Surroundings tempS = getSurroundings(pos);
                        ActionType ac = __grid[i]->takeTurn(tempS);

                        if(isLegal(ac,pos)){
                            Position posNew = move(pos,ac);
                            int posNewI = (pos.x*__width) + pos.y;
                            if(__grid[posNewI] == nullptr){
                                __grid[i]->setPosition(posNew);
                            }
                            else{
                                (*__grid[i])*(*__grid[posNewI]);
                            }
                        }
                    }
                }
            }
        }
        __round++;
        for(int i = 0; i < 0; i++){
            if(__grid[i] != nullptr){
                __grid[i]->setTurned(false);
        }
    }
    }

    std::ostream &operator<<(std::ostream &os, const Game &game) {
        os << "Round " << game.__round << ":" << std::endl;

        for (int i = 0; i < game.__height; ++i) {
            for (int j = 0; j < game.__width; ++j) {
                if((game.__grid[(i * game.__width) + j]) == nullptr){
                    os << "[     ]";
                }else{
                    os << "[" << std::setw(5) << std::left << *(game.__grid[(i * game.__width) + j]) << "]";
                }
            }
            os << std::endl;
        }
        os << "Status: " << game.__status << std::endl;
        return os;
    }

    const Position Game::move(const Position &pos, const ActionType &ac) const { // note: assumes legal, use with isLegal()
        ActionType arr[9] = {NW,N,NE,W,STAY,E,SE,S,SW};
        int j = 4;
        for(int i = 0; i < 9; i++){
            if(ac == arr[i]){
                j = i;
            }
        }
        int x,y;
        x = (j/3) - 1;
        y = (j%3) - 1;
        Position temp((pos.x+x)*__width,pos.y+y);
        return temp;
    }

    unsigned int Game::getNumAgents() const {
        unsigned int total = 0;
        for (int i = 0; i < __height; ++i) {
            for (int j = 0; j < __width; ++j) {
                if (__grid[(i * __width) + j] != nullptr) {
                    if(((__grid[(i * __width) + j]->getType()) == PieceType:: SIMPLE) || ((__grid[(i * __width) + j]->getType()) == PieceType::STRATEGIC)){
                        ++total;
                    }

                }
            }
        }
        return total;
    }

    unsigned int Game::getNumSimple() const {
        unsigned int total = 0;
        for (int i = 0; i < __height; ++i) {
            for (int j = 0; j < __width; ++j) {
                if (__grid[(i * __width) + j] != nullptr) {
                    if(((__grid[(i * __width) + j]->getType()) == PieceType:: SIMPLE)){
                        ++total;
                    }
                }
            }
        }
        return total;
    }

    unsigned int Game::getNumStrategic() const {
        unsigned int total = 0;
        for (int i = 0; i < __height; ++i) {
            for (int j = 0; j < __width; ++j) {
                if (__grid[(i * __width) + j] != nullptr) {
                    if(((__grid[(i * __width) + j]->getType()) == PieceType::STRATEGIC)){
                        ++total;
                    }
                }
            }
        }
        return total;
    }

    unsigned int Game::getNumResources() const {
        unsigned int total = 0;
        for (int i = 0; i < __height; ++i) {
            for (int j = 0; j < __width; ++j) {
                if (__grid[(i * __width) + j] != nullptr) {
                    if(((__grid[(i * __width) + j]->getType()) == PieceType:: FOOD) || ((__grid[(i * __width) + j]->getType()) == PieceType::ADVANTAGE)){
                        ++total;
                    }
                }
            }
        }
        return total;
    }
    const Piece *Game::getPiece(unsigned int x, unsigned int y) const {
        if (__grid[(x * __width) + y] != nullptr) {
            return __grid[(x * __width) + y];
        }else{
            throw PositionEmptyEx(__width,__height);
        }
    }

        // grid population methods
        void Game::addSimple(const Position &position){
            if(position.x >= __height  || position.y >= __width){
                throw OutOfBoundsEx(__height, __width, position.x,position.y);
            }
            if(position.x < 0 || position.y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(position.x * __width) + position.y] == nullptr){
            Simple *temp = new Simple(*this,position,STARTING_AGENT_ENERGY);
            __grid[(position.x * __width) + position.y] = temp;
            }else{
                throw PositionNonemptyEx(position.x, position.y);
            }
        }

        void Game::addSimple(const Position &position, double energy){
            if(position.x >= __height  || position.y >= __width){
                throw OutOfBoundsEx(__height, __width, position.x,position.y);
            }
            if(position.x < 0 || position.y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(position.x * __width) + position.y] == nullptr){
                Simple *temp = new Simple(*this,position,energy);
                __grid[(position.x * __width) + position.y] = temp;
            }else{
                throw PositionNonemptyEx(position.x, position.y);
            }
        }
        void Game::addSimple(unsigned x, unsigned y){
            if(x >= __height  || y >= __width){
                throw OutOfBoundsEx(__height, __width, x,y);
            }
            if(x < 0 || y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(x * __width) + y] == nullptr){
                Position pos(x,y);
                Simple *temp = new Simple(*this, pos ,STARTING_AGENT_ENERGY);
                __grid[(x * __width) + y] = temp;
            }else{
                throw PositionNonemptyEx(x,y);
            }
        }
        void Game::addSimple(unsigned x, unsigned y, double energy){
            if(x >= __height  || y >= __width){
                throw OutOfBoundsEx(__height, __width, x,y);
            }
            if(x < 0 || y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(x * __width) + y] == nullptr){
                Position pos(x,y);
                Simple *temp = new Simple(*this, pos ,energy);
                __grid[(x * __width) + y] = temp;
            }else{
                throw PositionNonemptyEx(x,y);
            }
        }
        void Game::addStrategic(const Position &position, Strategy *s){
            if(position.x >= __height  || position.y >= __width){
                throw OutOfBoundsEx(__height, __width, position.x,position.y);
            }
            if(position.x < 0 || position.y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(position.x * __width) + position.y] == nullptr){
                Strategic *temp = new Strategic(*this,position,STARTING_AGENT_ENERGY,s);
                __grid[(position.x * __width) + position.y] = temp;
            }else{
                throw PositionNonemptyEx(position.x, position.y);
            }
        }
        void Game::addStrategic(unsigned x, unsigned y, Strategy *s){
            if(x >= __height  || y >= __width){
                throw OutOfBoundsEx(__height, __width, x,y);
            }
            if(x < 0 || y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(x * __width) + y] == nullptr){
                Position pos(x,y);
                Strategic *temp = new Strategic(*this,pos,STARTING_AGENT_ENERGY,s);
                __grid[(x * __width) + y] = temp;
            }else{
                throw PositionNonemptyEx(x,y);
            }
        }
        void Game::addFood(const Position &position){
            if(position.x >= __height  || position.y >= __width){
                throw OutOfBoundsEx(__height, __width, position.x,position.y);
            }
            if(position.x < 0 || position.y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(position.x * __width) + position.y] == nullptr){
                Food *temp = new Food(*this,position,STARTING_RESOURCE_CAPACITY);
                __grid[(position.x * __width) + position.y] = temp;
            }else{
                throw PositionNonemptyEx(position.x, position.y);
            }
        }
        void Game::addFood(unsigned x, unsigned y){
            if(x >= __height  || y >= __width){
                throw OutOfBoundsEx(__height, __width, x,y);
            }
            if(x < 0 || y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(x * __width) + y] == nullptr){
                Position pos(x,y);
                Food *temp = new Food(*this,pos,STARTING_RESOURCE_CAPACITY);
                __grid[(x * __width) + y] = temp;
            }else{
                throw PositionNonemptyEx(x,y);
            }
        }
        void Game::addAdvantage(const Position &position){
            if(position.x >= __height  || position.y >= __width){
                throw OutOfBoundsEx(__height, __width, position.x,position.y);
            }
            if(position.x < 0 || position.y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(position.x * __width) + position.y] == nullptr){
                Advantage *temp = new Advantage(*this,position,STARTING_RESOURCE_CAPACITY);
                __grid[(position.x * __width) + position.y] = temp;
            }else{
                throw PositionNonemptyEx(position.x, position.y);
            }
        }
        void Game::addAdvantage(unsigned x, unsigned y){
            if(x >= __height  || y >= __width){
                throw OutOfBoundsEx(__height, __width, x,y);
            }
            if(x < 0 || y <0){
                throw (InsufficientDimensionsEx(0,0,__height,__width));
            }
            if(__grid[(x * __width) + y] == nullptr){
                Position pos(x,y);
                Advantage *temp = new Advantage(*this,pos,STARTING_RESOURCE_CAPACITY);
                __grid[(x * __width) + y] = temp;
            }else{
                throw PositionNonemptyEx(x,y);
            }
        }

        const Surroundings Game::getSurroundings(const Position &pos) const {
            Surroundings temp;
            int counter = 0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++){
                    if ((pos.x+i >= __height) || (pos.x+i < 0) ||( pos.y+j >= __width) || (pos.y+j < 0)) {
                        temp.array[counter] = INACCESSIBLE;
                        counter++;
                    }
                    else if ((__grid[((pos.x + i) * __width) + (pos.y + j)]) != nullptr) {
                        temp.array[counter] = (__grid[((pos.x + i) * __width) + (pos.y + j)])->getType();
                        counter++;
                    } else {
                        temp.array[counter] = EMPTY;
                        counter++;
                    }
                }
            }

            temp.array[4] = SELF;
            return temp;

        }

    const ActionType Game::reachSurroundings(const Position &from, const Position &to) { // note: STAY by default

        return STAY;
    }
    bool Game::isLegal(const ActionType &ac, const Position &pos) const{
        ActionType arr[9] = {NW,N,NE,W,STAY,E,SE,S,SW};
        Surroundings temp;
        temp = this->getSurroundings(pos);
        int i = 4;
        for(int j; j < 9; j++){
            if(ac == arr[j]){
                i = j;
            }
        }
        if (temp.array[i] == INACCESSIBLE){
            return false;
        }
        return true;
    }

        void Game::play(bool verbose) {    // play game until over

        }
}
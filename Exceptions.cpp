//
// Created by The on 3/31/2016.
//

#include <cstdlib>
#include "Exceptions.h"

namespace Gaming {

        void GamingException::setName(std::string name){
            __name = name;
        }

        std::ostream &operator<<(std::ostream &os, const GamingException &ex){
            os << ex.getName() << ": ";
            ex.__print_args(os);
            return os;
        }

//        unsigned __exp_width, __exp_height, __width, __height;

        DimensionEx::DimensionEx(unsigned expWidth, unsigned expHeight, unsigned width, unsigned height) {
            __exp_width = expWidth;
            __exp_height = expHeight;
            __width = width;
            __height = height;
            setName("DimensionEx");
        }
        unsigned DimensionEx::getExpWidth() const{
            return __exp_width;
        }
        unsigned DimensionEx::getExpHeight() const{
            return __exp_height;
        }
        unsigned DimensionEx::getWidth() const{
            return __width;
        }
        unsigned DimensionEx::getHeight() const{
            return __height;
        }


        InsufficientDimensionsEx::InsufficientDimensionsEx(unsigned minWidth, unsigned minHeight, unsigned width, unsigned height) : DimensionEx::DimensionEx(minWidth, minHeight, width, height){
            __exp_width = minWidth;
            __exp_height = minHeight;
            __width = width;
            __height = height;
            setName("InsufficientDimensionsEx");
        }

        void InsufficientDimensionsEx::__print_args(std::ostream &os) const{
            os << __width << ", " << __height;
        }

        OutOfBoundsEx::OutOfBoundsEx(unsigned maxWidth, unsigned maxHeight, unsigned width, unsigned height) : DimensionEx::DimensionEx(maxWidth, maxHeight, width, height){
            __exp_width = maxWidth;
            __exp_height = maxHeight;
            __width = width;
            __height = height;
            setName("OutOfBoundsEx");
        }

        void OutOfBoundsEx::__print_args(std::ostream &os) const{
            os << __name << ": " <<  __height << ", " << __width;
        }

        void PositionEx::__print_args(std::ostream &os) const{
            os << __name << ": " << __x << "," << __y;
        }

        PositionEx::PositionEx(unsigned x, unsigned y){
            __x = x;
            __y = y;
        }

    // to use in population methods

        PositionNonemptyEx::PositionNonemptyEx(unsigned x, unsigned y) : PositionEx(x,y){
            setName("PositionNonemptyEx");
        }

    // to use in Game Piece getter

        PositionEmptyEx::PositionEmptyEx(unsigned x, unsigned y) : PositionEx(x,y){
            setName("PositionEmptyEx");
        }

    // to use with position randomizer

        PosVectorEmptyEx::PosVectorEmptyEx(){
            setName("PosVectorEmptyEx");
        }
        void PosVectorEmptyEx::__print_args(std::ostream &os) const {
            os << __name << ": "<< "EMPTY";
        }
}
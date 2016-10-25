//
// Created by zaldi on 17/10/16.
//

#ifndef GEO_PROCC_BLOCK_H
#define GEO_PROCC_BLOCK_H

#include "cell.h"

#include <cstddef>
#include <iostream>

class block {

    int x_off, y_off;
    int x_size, y_size;
    bool loaded;
    size_t data_size;
    char * data;

public:

    block(int _x_off, int _y_off, int _x_size, int _y_size, size_t _data_size){
        x_off = _x_off;
        y_off = _y_off;
        x_size = _x_size;
        y_size = _y_size;
        data_size = _data_size;
        loaded = false;
    }

    int get_x_off(){ return x_off; }
    int get_y_off(){ return y_off; }
    int get_x_size(){ return x_size; }
    int get_y_size(){ return  y_size; }

    void allocate(){
        data = new char[data_size * x_size * y_size];
        loaded = true;
    }

    void deallocate(){
        delete[] data;
        loaded = false;
    }

    bool is_loaded(){
        return loaded;
    }

    char * get_ptr(){
        return data;
    }

    char * get_ptr(const cell& c){
        return data + data_size * (c.get_row() - y_off) * x_size + (c.get_col() - x_off);
    }

    friend std::ostream& operator << (std::ostream& os, block& b){
        os << "(x_off, y_off) = " << "(" << b.x_off << ", " << b.y_off << ")" << std::endl;
        os << "(x_size, y_size) = " << "(" << b.x_size << ", " << b.y_size << ")" << std::endl;
        os << "data size = " << b.data_size << std::endl;
    }

};


#endif //GEO_PROCC_BLOCK_H

//
// Created by zaldi on 25/10/16.
//

#ifndef GEO_PROCC_TEST_RASTER_H
#define GEO_PROCC_TEST_RASTER_H

#include <iostream>
#include "raster.h"
#include "cell.h"

template <typename t>
class test_raster: raster<t> {

    t * * data;

public:

    test_raster(int _rows, int _cols, t _noval){
        this->rows = _rows;
        this->cols = _cols;
        this->noval = _noval;
        data = new t * [this->rows];
        for(int r = 0; r < this->rows; r++)
            data[r] = new t[this->cols];
        for(int r = 0; r < this->rows; r++)
            for(int c = 0; c < this->cols; c++)
                data[r][c] = 0;
    }

    ~test_raster(){
        for(int r = 0; r < this->rows; r++) delete[] data[r];
        delete[] data;
    }

    char * get_ptr(const cell& c){
        return (char *) &(data[c.get_row()][c.get_col()]);
    }

    friend std::ostream& operator << (std::ostream& os, test_raster& tr){
        for(int r = 0; r < tr.rows; r++)
            for(int c = 0; c < tr.cols; c++){
                os << ((c == 0)? "(": "") << tr.data[r][c] << ((c < tr.cols - 1)? ", ":")\n");
            }
    }
};


#endif //GEO_PROCC_TEST_RASTER_H

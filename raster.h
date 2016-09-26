//
// Created by zaldi on 26/09/16.
//

#ifndef GEO_PROCC_RASTER_H
#define GEO_PROCC_RASTER_H


#include "cell.h"
#include <vector>

template <typename t>
class raster {

protected:

    int rows, cols;
    t noval;
    t * data;

public:

    /**
     * Default constructor
     * @return
     */
    raster(){};

    /**
     * Constructs a raster instance and alocates size(t) * rows * cols memory
     * @param _rows number of rows
     * @param _cols number of cols
     * @return void
     */
    raster(int _rows, int _cols, t _noval){
        rows = _rows;
        cols = _cols;
        noval = _noval;
        data = new t[rows * cols];
    };

    /**
     * Destructor, dealocate the data;
     */
    ~raster(){
        delete[] data;
    }

    /**
     * Returns the number of cols
     * @return
     */
    int get_rows(){
        return rows;
    }

    /**
     * Returns the number of rows
     * @return
     */
    int get_cols(){
        return cols;
    }

    /**
     * Checks if a cell is inside the raster
     * @param c cell
     * @return true if is inside otherwise false
     */
    bool is_inside(const cell& c){
        return c.get_row() >= 0 && c.get_row() < rows && c.get_col() > 0 && c.get_col() < cols;
    }


    /**
     * Checks whether a cell is contour. A cell is considered contour if it's inside, its value is different
     * from noval and is neighbor of an outside or noval cell
     * @param c cell
     * @return true if is contour, otherwise false
     */
    bool is_contour(const cell& c) {
        if (!is_inside(c)) return false;
        if ((*this)[c] == noval) return false;
        for (int i = 0; i < 8; i++) {
            cell n = c.get_neighbor(i);
            if (!is_inside(n)) return true;
            if ((*this)[n] == noval) return true;
        }
        return false;
    }

    /**
     * Checks whether a cell has data, c'est a dire if its value is different from noval
     * @param c cell
     * @return true if has data
     */
    bool has_data(const cell& c){
        return (*this)[c] != noval;
    }

    /**
     * Sets c
     * @param c cell
     * @return void
     */
    bool set_empty(const cell& c){
        (*this)[c] = noval;
    }

    /**
     * Returns a reference to the value at cell c
     * @param c cell
     * @return reference to data
     */
    const t& operator [](const cell& c) const{
        return data[c.get_row() * cols + c.get_col()];
    };

    /**
    * Returns a reference to the value at cell c
    * @param c cell
    * @return reference to data
    */
    t& operator [](const cell& c){
        return data[c.get_row() * cols + c.get_col()];
    };

};


#endif //GEO_PROCC_RASTER_H

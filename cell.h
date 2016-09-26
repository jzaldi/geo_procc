//
// Created by zaldi on 26/09/16.
//

#ifndef GEO_PROCC_CELL_H
#define GEO_PROCC_CELL_H


#include <ostream>

class cell {
    static int dr[], dc[];
    int row, col;
public:

    cell();
    cell(int, int);

    int get_row() const {return row;}
    int get_col() const {return col;}
    cell get_neighbor(int i) const;

    friend std::ostream& operator << (std::ostream&, const cell&);
};


#endif //GEO_PROCC_CELL_H

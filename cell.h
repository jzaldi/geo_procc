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

int cell::dr[] = { 0, 1, 1, 1, 0, -1, -1, -1};
int cell::dc[] = { 1, 1, 0, -1, -1, -1, 0, 1};

/**
 * Cell constructor by default
 * @return instance of cell
 */
cell::cell(){
    row = col = 0;
}

/**
 * Cell constructor from row, col
 * @param _row
 * @param _col
 * @return instance of cell
 */
cell::cell(int _row, int _col){
    row = _row;
    col = _col;
}

/**
 * Return neighbor i, as specified by dr, dc
 * @param i index between (0, 7)
 * @return instance of cell
 */
cell cell::get_neighbor(int i) const {
    return cell(row + dr[i], col + dc[i]);
}

/**
 * Print cell (r, c) to a stream
 * @param os reference to a stream
 * @param c cell
 * @return stream
 */
std::ostream& operator << (std::ostream& os, const cell& c){
    os << "cell(" << c.row << ", " << c.col << ")";
    return os;
}

#endif //GEO_PROCC_CELL_H

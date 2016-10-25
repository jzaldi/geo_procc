//
// Created by zaldi on 25/10/16.
//

#ifndef GEO_PROCC_NODE_H
#define GEO_PROCC_NODE_H

#include <iostream>
#include "cell.h"

/**
 * Class representing a node on a raster grid.
 * Node(0, 0) is the upper left corner of raster's cell(0,0)
 * Therefore there are n_cols + 1, n_rows + 1 nodes
 */
class node {

    int row; // node's row index
    int col; // node's column index

    static int dr[]; // Represents de order in which
    static int dc[]; // cell neighbors are taken

public:

    /**
     * Default constructor
     * @param _row node's row index
     * @param _col node's col index
     */
    node(int _row, int _col){
        row = _row;
        col = _col;
    }

    /**
     * Gets node's ith neighbor with the order
     * @param i neighbor index
     * @return ith node's cell neighbor
     */
    cell cell_neighbor(int i){
        i %= 4;
        return cell(row + dr[i], row + dc[i]);
    }

    /**
     * Dumps data to out stream
     * @param os ref to stream
     * @param n ref to node
     * @return ref to stream
     */
    friend std::ostream& operator << (std::ostream& os, node& n){
        return os << "node(" << n.row << ", " << n.col << ")";
    }

};

int node::dr[] = { -1, -1, 0, 0};
int node::dc[] = { -1, 0, 0, -1};

#endif //GEO_PROCC_NODE_H

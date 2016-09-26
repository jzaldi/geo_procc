//
// Created by zaldi on 26/09/16.
//

#ifndef GEO_PROCC_HYDRO_ALGORITHMS_H
#define GEO_PROCC_HYDRO_ALGORITHMS_H

#include "raster.h"
#include "cell.h"

#include <climits>
#include <stack>
#include <queue>
#include <vector>

/**
 *  Calculates the flow direction raster of a particular digital elevation model using a flooding algorithm.
 *  Input raster must be of short type.
 * @param dem
 * @param flow_dir
 */
void rw_flood_flow_direction(raster<short>& dem, raster<char>& flow_dir){

    const int m = USHRT_MAX + 1; // Possible data values present in the digital elevation model

    /*
     *      32   64   128
     *      16   0   1
     *      8   4   2
     */

    unsigned char inv_dir[] = {16, 32, 64, 128, 1, 2, 4, 8 };

    std::stack<cell> Q[m]; // Creates an array of stacks, one for each possible value

    for(int i = 0; i < dem.get_rows(); i++){
        for(int j = 0; j < dem.get_cols(); j++){ // Loops through the entire raster
            cell c = cell(i, j); // Cell creation
            if(dem.is_contour(c)){  // If the cell is contour, push it to the stack with index z
                int z = (int) dem[c];
                Q[z - SHRT_MIN].push(c);
            }
        }
    }

    for(int i = 0; i < m; i++){ // For each stack in the array
        int z = i + SHRT_MIN;
        while(!Q[i].empty()){   // while the stack is not empty
            cell c = Q[i].top();  // gets the peek of the stack and pop it
            Q[i].pop();
            for(int j = 0; j < 8; j ++){ // for each cell neighbor
                cell n = c.get_neighbor(j);
                if(dem.is_inside(n)){   // if the neighbor has data in dem, but we haven't processed it yet...
                    if(dem.has_data(n) && !flow_dir.has_data(n)) {
                        if (dem[n] < z) dem[n] = z;
                        flow_dir[n] = inv_dir[j];
                        Q[dem[n] - SHRT_MIN].push(n);
                    }
                }
            }
        }
    }
}

/**
 * Calculates the flow direction raster of a particular digital elevation model using a flooding algorithm.
 * @param dem
 * @param flow_dir
 */
template <typename t>
void priority_queue_flow_direction(raster<t>& dem, raster<char>& flow_dir){

    unsigned char inv_dir[] = {16, 32, 64, 128, 1, 2, 4, 8 };

    auto comp = [&dem](cell c0, cell c1){ return dem[c0] > dem[c1];};

    std::priority_queue<cell, std::vector<cell>, decltype(comp)> pq(comp);

    for(int i = 0; i < dem.get_rows(); i++){
        for(int j = 0; j < dem.get_cols(); j++){
            cell c = cell(i, j);
            if (dem.is_contour(c)){
                pq.push(c);
            }
        }
    }

    while(!pq.empty()){
        cell c = pq.top();
        pq.pop();
        for(int i = 0; i < 8; i++){
            cell n = c.get_neighbor(i);
            if(dem.is_inside(n)){
                if(dem.has_data(n) && !flow_dir.has_data(n)){
                    if(dem[n] < dem[c]) dem[n] = dem[c];
                    flow_dir[n] = inv_dir[i];
                    pq.push(n);
                }
            }
        }
    }

}

/**
 * Calculates the flow accumulation raster of a particular flow direction raster
 * @param flow_dir
 * @param flow_acc
 */
void stack_flow_accumulation(raster<char>& flow_dir, raster<unsigned>& flow_acc){

    unsigned char inv_dir[] = {16, 32, 64, 128, 1, 2, 4, 8 };

    for(int i = 0; i < flow_dir.get_rows(); i++){
        for(int j = 0; j < flow_dir.get_cols(); j++){
            cell c0 = cell(i, j); // Loops through every cell
            if(flow_dir.has_data(c0) && !flow_acc.has_data(c0)){ // If the flowdir has value but the flowacc don't
                std::stack< std::pair <cell, char> > s;
                s.push(std::pair<cell, char>(c0, 0));
                flow_acc[c0] = 0;
                while(!s.empty()){
                    cell c = s.top().first;
                    if(s.top().second < 8){
                        char d = s.top().second++;
                        cell n = c.get_neighbor(d);
                        if(flow_dir.is_inside(n)) if(flow_dir[n] == inv_dir[d]){
                                if(flow_acc.has_data(n)) {
                                    flow_acc[c] = flow_acc[c] + flow_acc[n];
                                } else {
                                    flow_acc[n] = 0;
                                    s.push(std::pair<cell, char>(n, 0));
                                }
                            }
                    } else {
                            s.pop();
                            if(!s.empty())flow_acc[s.top().first] =
                                    flow_acc[s.top().first] + 1 + flow_acc[c];
                    }
                }
            }
        }
    }
}

#endif //GEO_PROCC_HYDRO_ALGORITHMS_H

//
// Created by zaldi on 17/10/16.
//

#ifndef GEO_PROCC_BIG_RASTER_H
#define GEO_PROCC_BIG_RASTER_H

#include "raster.h"
#include "block.h"
#include "cell.h"

#include <iostream>
#include <vector>
#include <queue>

template <typename t>
class big_raster: public raster<t> {

protected:

    int block_x_size, block_y_size; // (cols, rows) of each memory block
    int block_x_count, block_y_count; // number of cols and rows of blocks
    size_t max_memory; // Max cache memory used
    bool read_only; // is it readonly?

    std::vector<block> blocks; // vector of memory blocks representing the raster
    std::queue<block*> loaded_blocks; // queue of blocks cache

    /**
     * Initialize the vector of blocks and the params of the big raster
     * Must be called by children
     */
    void initiallize(){

        block_x_count = this->cols / block_x_size;
        int last_block_x_size = this->cols % block_x_size;
        if(last_block_x_size == 0) last_block_x_size = block_x_size;
        else block_x_count++;

        block_y_count = this->rows / block_y_size;
        int last_block_y_size = this->rows % block_y_size;
        if(last_block_y_size == 0) last_block_y_size = block_y_size;
        else block_y_count++;

        for(int i = 0; i < block_y_count; i++){
            int y_off = i * block_x_size;
            int y_size = (i < block_y_count - 1) ? block_y_size: last_block_y_size;
            for(int j = 0; j < block_x_count; j++){
                int x_off = j * block_y_size;
                int x_size = (j < block_x_count - 1) ? block_x_size: last_block_x_size;
                blocks.push_back(block(x_off, y_off, x_size, y_size, sizeof(t)));
            }
        }

    }

    /**
     * Returns a pointer to the data of c
     * @param c
     * @return pointer to data at c
     */
    virtual char * get_ptr(const cell& c) {
        //  Find the block containing the data
        int block_x_index = c.get_col() / block_x_size;
        int block_y_index = c.get_row() / block_y_size;
        int block_index = block_y_index * block_x_count + block_x_index;

        if(!blocks[block_index].is_loaded()){

            /*
            std::cout << "Block loaded: " << std::endl;
            std::cout << blocks[block_index] << std::endl;
            std::cout << loaded_blocks.size() << std::endl;*/

            size_t required_size =
                    (1 + loaded_blocks.size()) * sizeof(t) * block_x_size * block_y_size;
            if(required_size > max_memory){ // We need to unload the last used block
                if(!read_only) store_block(loaded_blocks.front());
                //std::cout << "Block unloaded" << std::endl;
                loaded_blocks.front()->deallocate();
                loaded_blocks.pop();
            }
            blocks[block_index].allocate();
            fill_block(&blocks[block_index]);
            loaded_blocks.push(&blocks[block_index]);
        }

        return blocks[block_index].get_ptr(c);
    }

    /**
     * Fills a data block with data from an external source. Must be overloaded
     * by children
     * @param b reference to data block
     */
    virtual void fill_block(block* b){}

    /**
     * Saves data to an external source. If only called if not readonly. Must be overloaded
     * by children
     * @param b reference to data block
     */
    virtual void store_block(block* b){}

};


#endif //GEO_PROCC_BIG_RASTER_H

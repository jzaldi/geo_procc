//
// Created by zaldi on 17/10/16.
//

#ifndef GEO_PROCC_BIG_GDAL_RASTER_H
#define GEO_PROCC_BIG_GDAL_RASTER_H

#include "big_raster.h"
#include "cell.h"
#include "block.h"

#include <gdal/gdal_priv.h>

template <typename t>
class big_gdal_raster: public big_raster<t> {

    GDALDataset * dataset;
    GDALRasterBand * band;

public:
    big_gdal_raster(const char * file_name, int block_x_size, int block_y_size,
    size_t max_memory, bool read_only){

        this->block_x_size = block_x_size;
        this->block_y_size = block_y_size;
        this->max_memory = max_memory;
        this->read_only = read_only;

        GDALAllRegister();

        dataset = (GDALDataset *) GDALOpen(file_name, GA_Update);
        band = (GDALRasterBand *) dataset->GetRasterBand(1);

        this->rows = band->GetYSize();
        this->cols = band->GetXSize();
        this->noval = (t) band->GetNoDataValue();

        this->initiallize();

    }

    /**
     * Fills a data block with data from an external source. Must be overloaded
     * by children
     * @param b reference to data block
     */
    virtual void fill_block(block* b){
        band->RasterIO(GF_Read, b->get_x_off(), b->get_y_off(),
                       b->get_x_size(), b->get_y_size(),
                       b->get_ptr(), b->get_x_size(), b->get_y_size(),
                       band->GetRasterDataType(), 0, 0);
    }

    /**
     * Saves data to an external source. If only called if not readonly. Must be overloaded
     * by children
     * @param b reference to data block
     */
    virtual void store_block(block* b){
        band->RasterIO(GF_Write, b->get_x_off(), b->get_y_off(),
                       b->get_x_size(), b->get_y_size(),
                       b->get_ptr(), b->get_x_size(), b->get_y_size(),
                       band->GetRasterDataType(), 0, 0);
    }
};


#endif //GEO_PROCC_BIG_GDAL_RASTER_H

//
// Created by zaldi on 26/09/16.
//

#ifndef GEO_PROCC_GDAL_RASTER_H
#define GEO_PROCC_GDAL_RASTER_H

#include "raster.h"
#include <gdal/gdal_priv.h>

#include <iostream>

template<typename t>
class gdal_raster : public raster<t>{

    GDALDataset * dataset;
    GDALRasterBand * band;

public:

    /**
     * Reads a raster using gdal library
     * @param file_name file path to the raster
     * @return
     */
    gdal_raster(const char * file_name){

        GDALAllRegister();

        dataset = (GDALDataset *) GDALOpen(file_name, GA_Update);
        band = (GDALRasterBand *) dataset->GetRasterBand(1);

        this->rows = band->GetYSize();
        this->cols = band->GetXSize();
        std::cout << band->GetRasterDataType() << std::endl;
        this->noval = (t) band->GetNoDataValue();
        this->data = new t[band->GetXSize() * band->GetYSize()];
        band->RasterIO(GF_Read, 0, 0, band->GetXSize(), band->GetYSize(),
                       this->data, band->GetXSize(), band->GetYSize(),
                       band->GetRasterDataType(), 0, 0, NULL);
    }

    /**
     * Destructs the object and close the dataset
     */
    ~gdal_raster(){
        GDALClose(dataset);
    }


    /**
     *
     * @param source
     * @param noval
     * @return
     */
    template <typename t1>
    gdal_raster(gdal_raster<t1>& source, double noval, const char * file_name){

        GDALDataset * source_dataset = source.get_dataset();

        dataset = (GDALDataset *) source_dataset->GetDriver()->Create(
                file_name,
                source_dataset->GetRasterXSize(),
                source_dataset->GetRasterYSize(),
                1,
                GDT_Byte,
                NULL
        );

        double geo_transform[6];
        source_dataset->GetGeoTransform(geo_transform);
        dataset->SetGeoTransform(geo_transform);
        dataset->SetProjection(source_dataset->GetProjectionRef());
        band = dataset->GetRasterBand(1);
        band->SetNoDataValue(noval);

        this->rows = dataset->GetRasterYSize();
        this->cols = dataset->GetRasterXSize();

        this->noval = (t) band->GetNoDataValue();

        this->data = new t[this->rows * this->cols];
        t * p0 = this->data, * pf = this->data + this->rows * this->cols;
        do{
            * p0 = this->noval;
        } while(++p0 != pf);
    }

    /**
     * @return Returns a pointer to the gdal_dataset
     */
    GDALDataset * get_dataset(){
        return dataset;
    }

    void save(){
        band->RasterIO(GF_Write, 0, 0, band->GetXSize(), band->GetYSize(),
                       this->data, band->GetXSize(), band->GetYSize(),
                       band->GetRasterDataType(), 0, 0, NULL);
    }

};


#endif //GEO_PROCC_GDAL_RASTER_H

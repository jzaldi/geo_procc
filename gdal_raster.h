//
// Created by zaldi on 26/09/16.
//

#ifndef GEO_PROCC_GDAL_RASTER_H
#define GEO_PROCC_GDAL_RASTER_H

#include "raster.h"

#include <gdal/gdal_priv.h>

#include <iostream>
#include <typeinfo>

template<typename t>
class gdal_raster : public raster<t>{

    GDALDataset * dataset;
    GDALRasterBand * band;
    t * data;
public:

    GDALDataType get_gdal_dtype(){

        if(typeid(t) == typeid(unsigned char)) return GDT_Byte;
        if(typeid(t) == typeid(short)) return GDT_Int16;
        if(typeid(t) == typeid(unsigned)) return GDT_UInt32;
        if(typeid(t) == typeid(float)) return GDT_Float32;
        if(typeid(t) == typeid(double)) return GDT_Float64;

        return GDT_Unknown;
    }

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
        this->data = new t[this->rows * this->cols];
        std::cout << band->GetRasterDataType() << std::endl;
        this->noval = (t) band->GetNoDataValue();
        this->data = new t[band->GetXSize() * band->GetYSize()];
        band->RasterIO(GF_Read, 0, 0, band->GetXSize(), band->GetYSize(),
                       this->data, band->GetXSize(), band->GetYSize(),
                       band->GetRasterDataType(), 0, 0);
    }

    /**
     * Destructs the object and close the dataset
     */
    ~gdal_raster(){
        delete[](this->data);
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
                get_gdal_dtype(),
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
     * Returns a pointer to the data of c
     * @param c
     * @return pointer to data at c
    */
    virtual char * get_ptr(const cell& c){
        return (char*) (data + c.get_row() * this->cols + c.get_col());
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
                       band->GetRasterDataType(), 0, 0);
    }

};


#endif //GEO_PROCC_GDAL_RASTER_H

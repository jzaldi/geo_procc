//
// Created by zaldi on 17/10/16.
//

#ifndef GEO_PROCC_BIG_TEMP_RASTER_H
#define GEO_PROCC_BIG_TEMP_RASTER_H

#include "big_raster.h"

template <typename t>
class big_temp_raster: big_raster<t> {



public:
    big_temp_raster(big_raster& source, t noval){
        this->initiallize();
    }


};


#endif //GEO_PROCC_BIG_TEMP_RASTER_H

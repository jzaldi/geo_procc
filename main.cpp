#include <iostream>

#include "cell.h"

#include "gdal_raster.h"
#include "hydro_algorithms.h"
#include <climits>

int main() {

    cell c = cell(0, 1);
    std::cout << c << std::endl;

    const char * input_path = "test_data//output_srtm_flowdir.tif";
    const char * output_path = "test_data//output_srtm_flowacc.tif";

    gdal_raster<char> flow_dir = gdal_raster<char>(input_path);
    gdal_raster<unsigned> flow_acc = gdal_raster<unsigned>(flow_dir, UINT32_MAX, output_path);

    stack_flow_accumulation(flow_dir, flow_acc);

    flow_acc.save();

    return 0;
}
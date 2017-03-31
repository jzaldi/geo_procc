cmake_minimum_required(VERSION 3.5)
project(geo_procc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(SOURCE_FILES main.cpp cell.cpp cell.h raster.cpp raster.h gdal_raster.cpp gdal_raster.h hydro_algorithms.h block.h big_raster.h big_gdal_raster.h big_temp_raster.h test_raster.h node.h)
add_executable(geo_procc ${SOURCE_FILES})

target_link_libraries(geo_procc gdal)

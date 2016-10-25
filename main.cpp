#include <iostream>

#include "cell.h"
#include <iostream>
#include "gdal_raster.h"
#include "big_gdal_raster.h"
#include "hydro_algorithms.h"
#include "test_raster.h"
#include <climits>

#include "node.h"

using namespace std;

/**
 *
 * @param raster1
 * @param belongs_to_region
 * @param region
 */
template <typename t>
void vectorize_region(raster<t>& raster1, bool (*belongs_to_region)(t), vector<node>& region){
    
}

int main() {

    node n(1, 1);

    cout << n << endl;

    cout << n.cell_neighbor(3) << endl;

    return 0;
}
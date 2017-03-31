#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE geo_proc_tests
#include <boost/test/unit_test.hpp>

#include "../cell.h"

BOOST_AUTO_TEST_CASE(cell_test){
	cell c(10, 12);
	BOOST_CHECK(c.get_row() == 10);
	BOOST_CHECK(c.get_col() == 12);
}



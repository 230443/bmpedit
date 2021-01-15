//
// Created by daniel on 15.01.2021.
//

#include <boost/test/unit_test.hpp>
#include <cmath>
#include <string>
#include <complex>
#include "include/Task4.h"
#include "src/Task4.cpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#define IMAGE "../../Images/misc/pentagon.bmp"


inline boost::posix_time::ptime now()
{
	return boost::posix_time::microsec_clock::local_time();
}
//std::string name("lena.bmp");

BOOST_AUTO_TEST_SUITE(Task4Suite)

	BOOST_AUTO_TEST_CASE(erosion)
	{
		Task4 img(IMAGE);
		img.DFT();
	}


BOOST_AUTO_TEST_SUITE_END()



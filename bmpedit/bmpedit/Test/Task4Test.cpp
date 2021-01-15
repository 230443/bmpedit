//
// Created by daniel on 15.01.2021.
//


//
// Created by daniel on 09.12.2020.
//
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <string>
#include "include/Task4.h"
#include "src/Task4.cpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#define IMAGE "../../Images/mono/girlbw.bmp"


inline boost::posix_time::ptime now()
{
	return boost::posix_time::microsec_clock::local_time();
}
//std::string name("lena.bmp");

BOOST_AUTO_TEST_SUITE(Task4Suite)



BOOST_AUTO_TEST_SUITE_END()



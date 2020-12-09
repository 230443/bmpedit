//
// Created by daniel on 09.12.2020.
//
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <string>
#include "include/Bitmap.h"
#include "src/Task3.cpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#define IMAGE "/home/daniel/media/Shared/Images/mono/lenabw.bmp"
inline boost::posix_time::ptime now()
{
	return boost::posix_time::microsec_clock::local_time();
}
//std::string name("lena.bmp");

BOOST_AUTO_TEST_SUITE(Task3Suite)

	BOOST_AUTO_TEST_CASE(erosion)
	{
		Bitmap img(IMAGE);
		img.image.display("before",false,0,true);
		img.basic_morph_operation(3, 'e');
		img.image.display("dilation",false,0,true);
	}
	BOOST_AUTO_TEST_CASE(dilation)
	{
		Bitmap img(IMAGE);
		img.image.display("before",false,0,true);
		img.basic_morph_operation(3, 'd');
		img.image.display("dilation",false,0,true);
	}
	BOOST_AUTO_TEST_CASE(opening)
	{
		Bitmap img(IMAGE);
		img.image.display("before",false,0,true);
		img.opening(3);
		img.image.display("opening",false,0,true);
	}
	BOOST_AUTO_TEST_CASE(closing)
	{
		Bitmap img(IMAGE);
		img.image.display("before",false,0,true);
		img.closing(3);
		img.image.display("closing",false,0,true);
	}

BOOST_AUTO_TEST_SUITE_END()

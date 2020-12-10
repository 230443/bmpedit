//
// Created by daniel on 09.12.2020.
//
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <string>
#include "include/Bitmap.h"
#include "src/Task3.cpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#define IMAGE "/run/media/daniel/Shared/Images/mono/lenabw.bmp"
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
		img.image.display("erosion",false,0,true);
	}
	BOOST_AUTO_TEST_CASE(dilation)
	{
		Bitmap img(IMAGE);
		img.image.display("before",false,0,true);
		img.basic_morph_operation(3, 'd');
		img.image.display("dilation",false,0,true);
		img.save("/home/daniel/Desktop/dilation_new.bmp");
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
		//img.save("/home/daniel/Desktop/closing.bmp");
	}


BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(M3Suite)
	BOOST_AUTO_TEST_CASE(M3_3)
	{
		Bitmap img(IMAGE);
		//img.image.display("before",false,0,true);
		img.M3(333,222,3).save("/home/daniel/Desktop/M3_3.bmp");
		//img.save("/home/daniel/Desktop/closing.bmp");
	}
	BOOST_AUTO_TEST_CASE(M3_2)
	{
		Bitmap img(IMAGE);
		//img.image.display("before",false,0,true);
		img.M3(333,222,2).save("/home/daniel/Desktop/M3_2.bmp");
		//img.save("/home/daniel/Desktop/closing.bmp");
	}
BOOST_AUTO_TEST_SUITE_END()
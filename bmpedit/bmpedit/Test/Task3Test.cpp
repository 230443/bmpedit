//
// Created by daniel on 09.12.2020.
//
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <string>
#include "include/Bitmap.h"
#include "src/Task3.cpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#define IMAGE "../../Images/mono/girlbw.bmp"
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
		img.operation_3x3(3, 'e');
		img.image.display("erosion",false,0,true);
	}
	BOOST_AUTO_TEST_CASE(dilation)
	{
		Bitmap img(IMAGE);
		img.image.display("before",false,0,true);
		img.operation_3x3(3, 'd');
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
	BOOST_AUTO_TEST_CASE(HMT)
	{
		Bitmap img(IMAGE);
		img.image.display("before",false,0,true);
		img.operation_3x3(14, 'h');
		img.image.display("HMT",false,0,true);
		//img.save("/home/daniel/Desktop/closing.bmp");
	}


BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(M3Suite)


	BOOST_AUTO_TEST_CASE(M3_3)
	{
		Bitmap img(IMAGE);
		//img.image.display("before",false,0,true);
		img.M3(3);
		//img.save("/home/daniel/Desktop/closing.bmp");
	}
	BOOST_AUTO_TEST_CASE(M3_2)
	{
		Bitmap img(IMAGE);
		//img.image.display("before",false,0,true);
		img.M3(4);
		//img.save("/home/daniel/Desktop/closing.bmp");
	}
	BOOST_AUTO_TEST_CASE(R1)
	{
		{
			Bitmap img(IMAGE);
			img.image.display("original",false,0,true);
			img.operation_3x3(14, 'h');
			img.save("HMT.bmp");
		}
		Bitmap img(IMAGE);
		CImg<unsigned char> seeds("HMT.bmp");
		//seeds.display("seeds",false,0,true);
		//img.image.display("before",false,0,true);
		auto points = img.select_seeds();
		points.display("seeds",false,0,true);
		img.R1(4, points, 20);
		img.image.display("R1",false,0,true);



		//img.save("/home/daniel/Desktop/closing.bmp");
	}
BOOST_AUTO_TEST_SUITE_END()
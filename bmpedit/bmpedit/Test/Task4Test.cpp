//
// Created by daniel on 15.01.2021.
//

#include <boost/test/unit_test.hpp>
#include <cmath>
#include <string>
#include <complex>
#include "include/Task4.h"
#include "include/Bitmap.h"
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
		Bitmap img(IMAGE);
		img.DFT();
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Task4TimeSuite)

	int n = 1;
	BOOST_AUTO_TEST_CASE(DFT)
	{

		boost::posix_time::time_duration sum;
		for (int i = 0; i < n; ++i)
		{
			Bitmap img(IMAGE);
			boost::posix_time::ptime startTime = now();

			img.DFT();

			boost::posix_time::time_duration elapsed = now() - startTime;
			sum += elapsed;
		}
		std::cout << "DFT	"<< IMAGE << "	" << sum / n << std::endl;
	}

	BOOST_AUTO_TEST_CASE(IDFT)
	{

		boost::posix_time::time_duration sum;
		for (int i = 0; i < n; ++i)
		{
			Bitmap img(IMAGE);
			img.DFT();
			boost::posix_time::ptime startTime = now();

			img.IDFT();

			boost::posix_time::time_duration elapsed = now() - startTime;
			sum += elapsed;
		}
		std::cout << "IDFT	"<< IMAGE << "	" << sum / n << std::endl;
	}

BOOST_AUTO_TEST_SUITE_END()



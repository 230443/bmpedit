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

	BOOST_AUTO_TEST_CASE(DFT)
	{
		Bitmap img(IMAGE);
		img.DFT_2D();
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Task4TimeSuite)

	int n = 2;
	BOOST_AUTO_TEST_CASE(DFT)
	{

		boost::posix_time::time_duration sum;
		for (int i = 0; i < n; ++i)
		{
			Bitmap img(IMAGE);
			boost::posix_time::ptime startTime = now();

			img.DFT_2D();

			boost::posix_time::time_duration elapsed = now() - startTime;
			sum += elapsed;
		}
		std::cout << "DFT	"<< IMAGE << "	" << sum / n << std::endl;
	}

	BOOST_AUTO_TEST_CASE(IDFT)
	{

		boost::posix_time::time_duration sum;
		boost::posix_time::time_duration sum1;
		for (int i = 0; i < n; ++i)
		{
			Bitmap img(IMAGE);


			boost::posix_time::ptime startTime1 = now();
			img.DFT_2D();
			boost::posix_time::time_duration elapsed1 = now() - startTime1;
			sum1 += elapsed1;

			boost::posix_time::ptime startTime = now();

			img.IDFT_2D();

			boost::posix_time::time_duration elapsed = now() - startTime;
			sum += elapsed;
		}
		std::cout << "DFT	"<< IMAGE << "	" << sum1 / n << std::endl;
		std::cout << "IDFT	"<< IMAGE << "	" << sum / n << std::endl;
	}
	BOOST_AUTO_TEST_CASE(bit_reversal)
	{
		Bitmap img(IMAGE);
		BOOST_REQUIRE_EQUAL(img.reverse_bits(128+256,9),3);
	}
	BOOST_AUTO_TEST_CASE(test_dft_idft)
	{
		Bitmap img(IMAGE);

		img.DFT_2D();
		img.print_abs();
		img.IDFT_2D();
	}

BOOST_AUTO_TEST_SUITE_END()


///home/daniel/Desktop/Repos/Bmp/bmpedit/bmpedit/cmake-build-release/Test --run_test=Task4TimeSuite --logger=HRF,all --color_output=false --report_format=HRF --show_progress=no
//Testing started at 21:48 ...
//Running 2 test cases...
//		stack
//	DFT	../../Images/misc/pentagon.bmp	00:00:06.645755
//	IDFT	../../Images/misc/pentagon.bmp	00:00:07.681930
//		vector
//	DFT	../../Images/misc/pentagon.bmp	00:00:06.702352
//	IDFT	../../Images/misc/pentagon.bmp	00:00:07.642679
//DFT	../../Images/misc/pentagon.bmp	00:00:06.705549
//IDFT	../../Images/misc/pentagon.bmp	00:00:07.623555

//DFT	../../Images/misc/pentagon.bmp	00:00:05.976103
//IDFT	../../Images/misc/pentagon.bmp	00:00:07.606136
//	swap_ranges
//DFT	../../Images/misc/pentagon.bmp	00:00:06.281634
//IDFT	../../Images/misc/pentagon.bmp	00:00:07.771092
//	swap_ranges , inverse not swapping
//DFT	../../Images/misc/pentagon.bmp	00:00:06.075908
//IDFT	../../Images/misc/pentagon.bmp	00:00:05.907310
//DFT	../../Images/misc/pentagon.bmp	00:00:06.566673
//IDFT	../../Images/misc/pentagon.bmp	00:00:06.346205
//	Full look up table
//DFT	../../Images/misc/pentagon.bmp	00:00:00.767949
//IDFT	../../Images/misc/pentagon.bmp	00:00:00.740966
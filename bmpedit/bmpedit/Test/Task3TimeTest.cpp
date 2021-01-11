//
// Created by daniel on 15.12.2020.
//
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <string>
#include "include/Bitmap.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#define IMAGE "../../Images/mono/girlbw.bmp"
inline boost::posix_time::ptime now()
{
	return boost::posix_time::microsec_clock::local_time();
}

BOOST_AUTO_TEST_SUITE(Task3TimeSuite)

int n = 100;
BOOST_AUTO_TEST_CASE(dilation)
	{

		for (int j = 2; j <= 4; j++)
		{
			boost::posix_time::time_duration sum;
			for (int i = 0; i < n; ++i)
			{
				Bitmap img(IMAGE);
				boost::posix_time::ptime startTime = now();

				img.operation_3x3(j, 'd');

				boost::posix_time::time_duration elapsed = now() - startTime;
				sum += elapsed;
			}
			std::cout << "dilation	" << j << "	" << IMAGE << "	" << sum / n << std::endl;
		}
	}

	BOOST_AUTO_TEST_CASE(erosion)
	{

		for (int j = 2; j <= 4; j++)
		{
			boost::posix_time::time_duration sum;
			for (int i = 0; i < n; ++i)
			{
				Bitmap img(IMAGE);
				boost::posix_time::ptime startTime = now();

				img.operation_3x3(j, 'e');

				boost::posix_time::time_duration elapsed = now() - startTime;
				sum += elapsed;
			}
			std::cout << "erosion	" << j << "	" << IMAGE << "	" << sum / n << std::endl;
		}
	}

	BOOST_AUTO_TEST_CASE(HMT)
	{

		for (int j = 2; j <= 4; j++)
		{
			boost::posix_time::time_duration sum;
			for (int i = 0; i < n; ++i)
			{
				Bitmap img(IMAGE);
				boost::posix_time::ptime startTime = now();

				img.operation_3x3(j, 'h');

				boost::posix_time::time_duration elapsed = now() - startTime;
				sum += elapsed;
			}
			std::cout << "HMT	" << j << "	" << IMAGE << "	" << sum / n << std::endl;
		}
	}
	BOOST_AUTO_TEST_CASE(opening)
	{

		for (int j = 2; j <= 4; j++)
		{
			boost::posix_time::time_duration sum;
			for (int i = 0; i < n; ++i)
			{
				Bitmap img(IMAGE);
				boost::posix_time::ptime startTime = now();

				img.opening(j);

				boost::posix_time::time_duration elapsed = now() - startTime;
				sum += elapsed;
			}
			std::cout << "opening	" << j << "	" << IMAGE << "	" << sum / n << std::endl;
		}
	}
	BOOST_AUTO_TEST_CASE(closing)
	{

		for (int j = 2; j <= 4; j++)
		{
			boost::posix_time::time_duration sum;
			for (int i = 0; i < n; ++i)
			{
				Bitmap img(IMAGE);
				boost::posix_time::ptime startTime = now();

				img.closing(j);

				boost::posix_time::time_duration elapsed = now() - startTime;
				sum += elapsed;
			}
			std::cout << "closing	" << j << "	" << IMAGE << "	" << sum / n << std::endl;
		}
	}
	BOOST_AUTO_TEST_CASE(M3)
	{

		for (int j = 2; j <= 4; j++)
		{
			boost::posix_time::time_duration sum;
			for (int i = 0; i < n; ++i)
			{
				Bitmap img(IMAGE);
				boost::posix_time::ptime startTime = now();

				img.M3(j);

				boost::posix_time::time_duration elapsed = now() - startTime;
				sum += elapsed;
			}
			std::cout << "M3	" << j << "	" << IMAGE << "	" << sum / n << std::endl;
		}
	}
	BOOST_AUTO_TEST_CASE(R1)
	{

		for (int j = 2; j <= 4; j++)
		{
			boost::posix_time::time_duration sum;
			for (int i = 0; i < n; ++i)
			{
				Bitmap img(IMAGE);
				cimg_library::CImg<unsigned char> seeds("seeds.bmp");
				boost::posix_time::ptime startTime = now();

				img.R1(j, seeds, 20);

				boost::posix_time::time_duration elapsed = now() - startTime;
				sum += elapsed;
			}
			std::cout << "R1	" << j << "	" << IMAGE << "	" << sum / n << std::endl;
		}
	}


BOOST_AUTO_TEST_SUITE_END()

//dilation	2	boatbw.bmp	00:00:00.002136
//dilation	3	boatbw.bmp	00:00:00.001044
//dilation	4	boatbw.bmp	00:00:00.001301
//erosion		2	boatbw.bmp	00:00:00.001274
//erosion		3	boatbw.bmp	00:00:00.001295
//erosion		4	boatbw.bmp	00:00:00.001410
//HMT			2	boatbw.bmp	00:00:00.001820
//HMT			3	boatbw.bmp	00:00:00.001608
//HMT			4	boatbw.bmp	00:00:00.001635
//opening		2	boatbw.bmp	00:00:00.003268
//opening		3	boatbw.bmp	00:00:00.002317
//opening		4	boatbw.bmp	00:00:00.002808
//closing		2	boatbw.bmp	00:00:00.003227
//closing		3	boatbw.bmp	00:00:00.002127
//closing		4	boatbw.bmp	00:00:00.002447
//
//dilation		2	girlbw.bmp	00:00:00.001793	boatbw.bmp	00:00:00.002136
//dilation		3	girlbw.bmp	00:00:00.001178	boatbw.bmp	00:00:00.001044
//dilation		4	girlbw.bmp	00:00:00.001422	boatbw.bmp	00:00:00.001301
//erosion		2	girlbw.bmp	00:00:00.001013	boatbw.bmp	00:00:00.001274
//erosion		3	girlbw.bmp	00:00:00.000728	boatbw.bmp	00:00:00.001295
//erosion		4	girlbw.bmp	00:00:00.000930	boatbw.bmp	00:00:00.001410
//HMT			2	girlbw.bmp	00:00:00.001545	boatbw.bmp	00:00:00.001820
//HMT			3	girlbw.bmp	00:00:00.001099	boatbw.bmp	00:00:00.001608
//HMT			4	girlbw.bmp	00:00:00.001021	boatbw.bmp	00:00:00.001635
//opening		2	girlbw.bmp	00:00:00.002501	boatbw.bmp	00:00:00.003268
//opening		3	girlbw.bmp	00:00:00.001798	boatbw.bmp	00:00:00.002317
//opening		4	girlbw.bmp	00:00:00.002157	boatbw.bmp	00:00:00.002808
//closing		2	girlbw.bmp	00:00:00.002564	boatbw.bmp	00:00:00.003227
//closing		3	girlbw.bmp	00:00:00.001890	boatbw.bmp	00:00:00.002127
//closing		4	girlbw.bmp	00:00:00.002231	boatbw.bmp	00:00:00.002447
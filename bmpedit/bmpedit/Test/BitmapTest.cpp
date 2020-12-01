//
// Created by daniel on 14.11.2020.
//

#include <boost/test/unit_test.hpp>
#include <cmath>
#include <string>
#include "Bitmap.h"
#include "Task2.cpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#define IMAGE "/run/media/daniel/Shared/Images/lena/lena.bmp"
inline boost::posix_time::ptime now()
{
    return boost::posix_time::microsec_clock::local_time();
}
//std::string name("lena.bmp");

BOOST_AUTO_TEST_SUITE(BitmapSuite)

    BOOST_AUTO_TEST_CASE(FilterCmean_Time)
    {
        Bitmap img(IMAGE);

        boost::posix_time::ptime startTime = now();

        img.filter(5,2, Bitmap::contra);

        boost::posix_time::time_duration elapsed = now() - startTime;

        std::cout << "filter_contra " << elapsed << std::endl;

    }
    BOOST_AUTO_TEST_CASE(FilterAlpha_Time)
    {
        Bitmap img(IMAGE);

        boost::posix_time::ptime startTime = now();

        img.filter(8,2, Bitmap::alpha);

        boost::posix_time::time_duration elapsed = now() - startTime;

        std::cout << "filter_alpha " << elapsed << std::endl;

    }

    BOOST_AUTO_TEST_CASE(exponential)
    {
        Bitmap img(IMAGE);
        img.contrast(0.5);
        img.save("contrast.bmp");
        img.image.display("contrast",false,0,true);

        img.hexponent(10, 255);

        img.image.display("After",false,0,true);
        img.save("test.bmp");

    }

    BOOST_AUTO_TEST_CASE(histogram)
    {
        Bitmap img(IMAGE);
        img.histogram();
    }
    BOOST_AUTO_TEST_CASE(mean)
    {
        Bitmap img(IMAGE);
        std::cout<<img.mean()<<std::endl;
        std::cout<<img.casyco();
    }
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(laplaceSuite)

	int n = 1000;
	BOOST_AUTO_TEST_CASE(slaplace0)
	{
		boost::posix_time::time_duration sum;
		for (int i = 0; i < n; ++i)
		{
			Bitmap img(IMAGE);
			boost::posix_time::ptime startTime = now();

			img.slaplace(nullptr);

			boost::posix_time::time_duration elapsed = now() - startTime;
			sum += elapsed;
		}

		std::cout << "slaplace0 " << sum/n << std::endl;

	}
	BOOST_AUTO_TEST_CASE(slaplace1)
	{
		boost::posix_time::time_duration sum;
		for (int i = 0; i < n; ++i)
		{
			Bitmap img(IMAGE);
			boost::posix_time::ptime startTime = now();

			int mask[9] = { -1, -1, -1,
					-1, +8, -1,
					-1, -1, -1 };
			img.slaplace(mask);

			boost::posix_time::time_duration elapsed = now() - startTime;
			sum += elapsed;
		}

		std::cout << "slaplace1 " << sum/n << std::endl;

	}
	BOOST_AUTO_TEST_CASE(slaplace2)
	{
		boost::posix_time::time_duration sum;
		for (int i = 0; i < n; ++i)
		{
			Bitmap img(IMAGE);
			boost::posix_time::ptime startTime = now();

			int mask[9] = { +1, -2, +1,
					-2, +4, -2,
					+1, -2, +1 };
			img.slaplace(mask);

			boost::posix_time::time_duration elapsed = now() - startTime;
			sum += elapsed;
		}
		std::cout << "slaplace2 " << sum/n << std::endl;
	} /*
	BOOST_AUTO_TEST_CASE(slaplaceMix)
	{
		boost::posix_time::time_duration sum;
		for (int i = 0; i < n; ++i)
		{
			int mask_nr = rand() %3;
			Bitmap img(IMAGE);
			boost::posix_time::ptime startTime = now();
			if (mask_nr == 0)
				img.slaplace(nullptr);
			else if (mask_nr == 1)
			{
				int mask[9] = { -1, -1, -1,
						-1, +8, -1,
						-1, -1, -1 };
				img.slaplace(mask);
			}
			else if (mask_nr == 2)
			{
				int mask[9] = { +1, -2, +1,
						-2, +4, -2,
						+1, -2, +1 };
				img.slaplace(mask);
			}
			boost::posix_time::time_duration elapsed = now() - startTime;
			sum += elapsed;
		}
		std::cout << "slaplace2 " << sum/n << std::endl;
	}*/


BOOST_AUTO_TEST_SUITE_END()
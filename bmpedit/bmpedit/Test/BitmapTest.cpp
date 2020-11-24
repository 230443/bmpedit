//
// Created by daniel on 14.11.2020.
//

#include <boost/test/unit_test.hpp>
#include <cmath>
#include "Bitmap.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#define IMAGE "lena_impulse2.bmp"
inline boost::posix_time::ptime now()
{
    return boost::posix_time::microsec_clock::local_time();
}


BOOST_AUTO_TEST_SUITE(BitmapSuite)

    BOOST_AUTO_TEST_CASE(cmeanTime)
    {
        Bitmap img(IMAGE);

        boost::posix_time::ptime startTime = now();

        img.cmean(5,2);

        boost::posix_time::time_duration elapsed = now() - startTime;

        std::cout << "cmean " << elapsed << std::endl;

    }

    BOOST_AUTO_TEST_CASE(FilterCmean_Time)
    {
        Bitmap img(IMAGE);

        boost::posix_time::ptime startTime = now();

        img.filter(5,2, Bitmap::contra);

        boost::posix_time::time_duration elapsed = now() - startTime;

        std::cout << "filter_contra " << elapsed << std::endl;

    }
    BOOST_AUTO_TEST_CASE(Alpha_Time)
    {
        Bitmap img(IMAGE);

        boost::posix_time::ptime startTime = now();

        img.alpha(16,2);

        boost::posix_time::time_duration elapsed = now() - startTime;

        std::cout << "alpha " << elapsed << std::endl;

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
        Bitmap img("lena.bmp");
        img.contrast(0.5);
        img.save("contrast.bmp");
        img.image.display("contrast",false,0,true);

        img.hexponent(10, 255);

        img.image.display("After",false,0,true);
        img.save("test.bmp");

    }

    */
    BOOST_AUTO_TEST_CASE(histogram)
    {
        Bitmap img("lena.bmp");
        img.histogram();
    }
    BOOST_AUTO_TEST_CASE(mean)
    {
        Bitmap img("lena.bmp");
        std::cout<<img.mean()<<std::endl;
        std::cout<<img.casyco();
    }

BOOST_AUTO_TEST_SUITE_END()
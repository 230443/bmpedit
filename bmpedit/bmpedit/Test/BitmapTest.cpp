//
// Created by daniel on 14.11.2020.
//

#include <boost/test/unit_test.hpp>
#include <cmath>
#include "Bitmap.h"
#include <boost/date_time/posix_time/posix_time.hpp>

inline boost::posix_time::ptime now()
{
    return boost::posix_time::microsec_clock::local_time();
}


BOOST_AUTO_TEST_SUITE(BitmapSuite)

    BOOST_AUTO_TEST_CASE(cmeanTime)
    {
        double x = 0.0;

        Bitmap img("lena_impulse2.bmp");

        boost::posix_time::ptime startTime = now();

        img.cmean(3,2);

        boost::posix_time::time_duration elapsed = now() - startTime;

        std::cout << elapsed << " ";

    }
BOOST_AUTO_TEST_SUITE_END()
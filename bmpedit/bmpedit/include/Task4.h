//
// Created by daniel on 15.01.2021.
//

#ifndef BMPEDIT_TASK4_H
#define BMPEDIT_TASK4_H


#include <complex>
#include "CImg.h"
//#include <cmath>

	//const double PI = 3.141592653589793238463;
class Bitmap;

	class Task4
	{
	private:
		cimg_library::CImg<unsigned char> img;
		cimg_library::CImg<std::complex<double>> img_transformed;

		void DFT_1D();

		void transform_row(int row_number);

	public:
		Task4(cimg_library::CImg<unsigned char>& img);

		void DFT();


	};

#endif //BMPEDIT_TASK4_H

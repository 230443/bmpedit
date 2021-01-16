//
// Created by daniel on 15.01.2021.
//

#ifndef BMPEDIT_TASK4_H
#define BMPEDIT_TASK4_H


#include <complex>
#include "CImg.h"
#include <vector>


	//const double PI = 3.141592653589793238463;
class Bitmap;

	class Task4
	{
	private:
		cimg_library::CImg<unsigned char>* img;

		size_t HEIGHT;
		size_t WIDTH;




		std::vector<std::vector<std::complex<double>>> img_transformed;
		//std::array<std::array<std::complex<double>,WIDTH>,HEIGHT> img_transformed_shifted;
		//std::vector<std::vector<std::complex<double>>> img_transformed;
		//std::vector<std::vector<std::complex<double>>> img_transformed_shifted;

		//template <typename T>
		void transform_row(int row_number,
				unsigned char* first_pixel,
				std::vector<std::vector<std::complex<double>>>& img_output);
		void transform_row_and_shift(int row_number, std::complex<double>* first_pixel);
		void i_transform_row_and_shift(int row_number,
				std::complex<double>* first_pixel,
				std::vector<std::vector<std::complex<double>>>& img_output);
		void i_transform_row(int row_number,
				std::complex<double>* first_pixel,
				cimg_library::CImg<double>& img_output);

	protected:

		void constructor();

	public:
		Task4(cimg_library::CImg<unsigned char>* img);
		void DFT();
		void IDFT();

		void print_abs();
		void print_arg();
		void print_real();
		void print_imag();



	};

#endif //BMPEDIT_TASK4_H

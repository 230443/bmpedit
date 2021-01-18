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
	std::vector<double> coefficients_for_k;

	template<typename T>
	void DFT_1D(const T* input_row, int output_column_nr,
			std::vector<std::vector<std::complex<double>>>& output_matrix);

	void initialize_coefficients(bool inverse);

protected:

	void constructor();

public:
	Task4(cimg_library::CImg<unsigned char>* img);

	//	DFT
	void DFT_2D();
	void IDFT_2D();

	//	Presenting DFT
	void print_abs();
	void print_arg();
	void print_real();
	void print_imag();

	void apply_mask(cimg_library::CImg<double>&);
	void LPF(int max_band);

	void FFT(std::vector<double>&);

	uint32_t reverse_bits(uint32_t, size_t size);
};

#endif //BMPEDIT_TASK4_H

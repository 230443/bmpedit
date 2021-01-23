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
	std::vector<std::complex<double>> coefficients;

	template<typename T>
	void DFT(const T* input_row, int output_column_nr,
			std::vector<std::vector<std::complex<double>>>& output_matrix);
	template<typename T>
	void FFT(const T* input_row, int stages, int output_column_nr,
			std::vector<std::vector<std::complex<double>>>& output_matrix);

	void set_DFT_coefficients(bool inverse);
	void set_FFT_coefficients(bool inverse, unsigned size);
	template<typename T>
	void apply_mask(T*);

protected:
	void constructor();
	static uint32_t reverse_bits(uint32_t, size_t size);

public:
	Task4(cimg_library::CImg<unsigned char>* img);

	//	DFT
	void DFT_2D();
	void IDFT_2D();
	//	FFT
	void FFT_2D();
	void IFFT_2D();

	//	Presenting DFT
	void print_abs(bool save=false);
	void print_arg(bool save=false);
	void print_real(bool save=false);
	void print_imag(bool save=false);

	//	Filters
	void LPF(int size);
	void HPF(int size);
	void PMF(int k,int l);
	void BPF(int min, int max);
	void BCF(int min, int max);
	void EDF(double angle, double approx, int size);
};

#endif //BMPEDIT_TASK4_H

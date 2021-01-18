//
// Created by daniel on 15.01.2021.
//

#include "include/Task4.h"
#include <iostream>


Task4::Task4(cimg_library::CImg<unsigned char>* img)
		:img(img), HEIGHT(0), WIDTH(0), img_transformed(WIDTH, std::vector<std::complex<double>>(HEIGHT))
{
	//img_transformed.resize(HEIGHT,std::vector<std::complex<double>>(WIDTH));
	//{
	//	using namespace std;
	//	cout<<img->height()<<"(t4)"<<img->width()<<endl;
	//	cout<<HEIGHT<<","<<WIDTH<<endl;
	//	cout<<img_transformed.capacity()<<","<<img_transformed[0].capacity()<<endl;
	//}
}

void Task4::constructor()
{
	WIDTH = img->width();
	HEIGHT = img->height();

	img_transformed = std::vector(HEIGHT, std::vector<std::complex<double>>(WIDTH));
}

uint32_t Task4::reverse_bits(uint32_t n, size_t size)
{
	uint32_t x = 0;
	for (; size > 0; size--)
	{
		x <<= 1;
		x |= n & 1;
		n >>= 1;
	}
	return x;
}

template<typename T>
void Task4::DFT_1D(const T* input_row, int output_column_nr,
		std::vector<std::vector<std::complex<double>>>& output_matrix)
{
	for (int k = 0; k < WIDTH; k++)
	{
		std::complex<double> transformed_pixel;
		auto element = input_row;
		for (int n = 0; n < WIDTH; n++)
		{
			transformed_pixel += (std::complex<double>)*element * std::polar(1.0, coefficients_for_k[k] * n);
			element++;
		}
		output_matrix[k][output_column_nr] = transformed_pixel;
	}
}

void Task4::DFT_2D()
{
	initialize_coefficients(false);
	std::vector<std::vector<std::complex<double>>> matrix_tmp(WIDTH, std::vector<std::complex<double>>(HEIGHT));

	for (unsigned y = 0; y < HEIGHT; y++)
		DFT_1D(img->data(0, y), y, matrix_tmp);

	std::swap_ranges(matrix_tmp.begin(), matrix_tmp.begin() + HEIGHT / 2, matrix_tmp.begin() + HEIGHT / 2);

	for (unsigned y = 0; y < HEIGHT; y++)
		DFT_1D(&matrix_tmp[y][0], y, this->img_transformed);

	std::swap_ranges(img_transformed.begin(), img_transformed.begin() + HEIGHT / 2,
			img_transformed.begin() + HEIGHT / 2);
}

void Task4::initialize_coefficients(bool inverse)
{
	double sign = inverse ? -2 : 2;
	coefficients_for_k = std::vector<double>(HEIGHT);
	for (unsigned k = 0; k < HEIGHT; k++)
	{
		coefficients_for_k[k] = sign * M_PI * k / WIDTH;
	}
}

void Task4::IDFT_2D()
{
	std::vector<std::vector<std::complex<double>>> matrix_tmp(WIDTH, std::vector<std::complex<double>>(HEIGHT));
	initialize_coefficients(true);

	for (unsigned y = 0; y < HEIGHT; y++)
		DFT_1D(&img_transformed[y][0], y, matrix_tmp);
	//std::swap_ranges(matrix_tmp.begin(),matrix_tmp.begin()+HEIGHT/2,matrix_tmp.begin()+HEIGHT/2);

	for (unsigned y = 0; y < HEIGHT; y++)
		DFT_1D(&matrix_tmp[y][0], y, this->img_transformed);
	//std::swap_ranges(img_transformed.begin(),img_transformed.begin()+HEIGHT/2,img_transformed.begin()+HEIGHT/2);

	auto* pixel = img->begin();
	for (auto row: img_transformed)
		for (auto value: row)
		{
			*pixel = lround(abs(value / double(HEIGHT * WIDTH)));
			pixel++;
		}
}


void Task4::print_abs()
{
	cimg_library::CImg<double> new_img(HEIGHT, WIDTH);
	auto pixel = new_img.begin();

	for (auto& row : img_transformed)
		for (auto& value : row)
			*pixel++ = log10(abs(value));

	new_img.display("DFT. Magnitude spectrum", false, nullptr, true);
}

void Task4::print_arg()
{
	cimg_library::CImg<double> new_img(HEIGHT, WIDTH);
	auto pixel = new_img.begin();

	for (auto& row : img_transformed)
		for (auto& value : row)
			*pixel++ = arg(value);

	new_img.display("DFT. Phase spectrum", false, nullptr, true);
}

void Task4::print_real()
{
	cimg_library::CImg<double> new_img(HEIGHT, WIDTH);
	auto pixel = new_img.begin();
	for (auto& row : img_transformed)
		for (auto& value : row)
			*pixel++ = log10(real(value));

	new_img.display("DFT. Real spectrum", false, nullptr, true);
}

void Task4::print_imag()
{
	cimg_library::CImg<double> new_img(HEIGHT, WIDTH);
	auto pixel = new_img.begin();
	for (auto& row : img_transformed)
		for (auto& value : row)
			*pixel++ = imag(value);

	new_img.display("DFT. Imaginary spectrum", false, nullptr, true);
}

void Task4::apply_mask(cimg_library::CImg<double>& mask)
{
	auto* pixel = mask.begin();
	for (auto& row : img_transformed)
		for (auto& value : row)
			value *= *pixel++;
}

void Task4::LPF(int size)
{
	cimg_library::CImg<double> mask(WIDTH,HEIGHT,1,1,0);

	const double color[] = {1};
	mask.draw_circle(HEIGHT/2,WIDTH/2,size,color);
	apply_mask(mask);
}
void Task4::HPF(int size)
{
	cimg_library::CImg<double> mask(WIDTH,HEIGHT,1,1,1);

	const double color[] = {0};
	mask.draw_circle(HEIGHT/2,WIDTH/2,size,color);
	apply_mask(mask);
}

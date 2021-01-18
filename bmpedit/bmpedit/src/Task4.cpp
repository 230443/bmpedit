//
// Created by daniel on 15.01.2021.
//

#include "include/Task4.h"
#include <iostream>


Task4::Task4(cimg_library::CImg<unsigned char>* img)
		:img(img), HEIGHT(0), WIDTH(0),img_transformed(WIDTH,std::vector<std::complex<double>>(HEIGHT))
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

	img_transformed = std::vector(HEIGHT,std::vector<std::complex<double>>(WIDTH));

	//{
	//	using namespace std;
	//	cout<<img->height()<<"(const)"<<img->width()<<endl;
	//	cout<<HEIGHT<<","<<WIDTH<<endl;
	//	cout<<img_transformed.capacity()<<","<<img_transformed[0].capacity()<<endl;
	//}
}


void Task4::transform_row(int row_number, unsigned char* first_pixel,
		std::vector<std::vector<std::complex<double>>>& img_output)
{
	for (int k = 0; k<WIDTH; k++)
	{
			//-j*2*pi*k/N
		double row_coefficient = -2 * M_PI * k / WIDTH;

		std::complex<double> transformed_pixel;
		auto* pixel = first_pixel;
		for (int n = 0; n < WIDTH; n++)
		{
			transformed_pixel += std::polar((double)*pixel, row_coefficient * n);
			pixel++;
		}

		img_output[k][row_number] = transformed_pixel;
	}
}
void Task4::transform_row_and_shift(int row_number, std::complex<double>* first_pixel)
// resulting values are inserted at shifted position
{
	unsigned column;
	if(row_number<WIDTH/2)
		column = row_number+WIDTH/2;
	else
		column = row_number-WIDTH/2;


	for (int k = 0; k<HEIGHT/2; k++)
	{
		double row_coefficient = -2 * M_PI * k / HEIGHT;

		std::complex<double> transformed_pixel;
		auto* pixel = first_pixel;

		for (int n = 0; n < WIDTH; n++)
		{
			std::complex<double> tmp(0,row_coefficient*n);
			transformed_pixel += *pixel * std::exp(tmp);
			pixel++;
		}
		img_transformed[k+HEIGHT/2][column] = transformed_pixel;
	}
	for (unsigned k = HEIGHT/2; k<HEIGHT; k++)
	{
		double row_coefficient = -2 * M_PI * k / HEIGHT;

		std::complex<double> transformed_pixel;
		auto* pixel = first_pixel;
		for (int n = 0; n < WIDTH; n++)
		{
			std::complex<double> tmp(0,row_coefficient*n);
			transformed_pixel += *pixel * std::exp(tmp);
			pixel++;
		}
		img_transformed[k-HEIGHT/2][column] = transformed_pixel;
	}

}


void Task4::DFT()
{
	std::vector<std::vector<std::complex<double>>> img_tmp(WIDTH,std::vector<std::complex<double>>(HEIGHT));
	for (int y = 0; y<HEIGHT; y++)
	{
		transform_row(y, img->data(0, y), img_tmp);
		//{
		//	using namespace std;
		//	cout<<img_tmp[0][y]<<";";
		//}
	}
	for (int y = 0; y<HEIGHT; y++)
	{
		transform_row_and_shift(y, &img_tmp[y][0]);
	}
}




void Task4::IDFT()
{
	std::vector<std::vector<std::complex<double>>> img_tmp(WIDTH,std::vector<std::complex<double>>(HEIGHT));
	for (int y = 0; y<HEIGHT; y++)
	{
		i_transform_row_and_shift(y, &img_transformed[y][0], img_tmp);
		//{
		//	using namespace std;
		//	cout<<img_tmp[0][y]<<";";
		//}
	}
	cimg_library::CImg<double> new_img(WIDTH,HEIGHT);
	for (int y = 0; y<WIDTH; y++)
	{
		i_transform_row(y, &img_tmp[y][0], new_img);
	}

	//new_img.display("After IDFT",false,0,true);

}

void Task4::i_transform_row_and_shift(int row_number, std::complex<double>* first_pixel,
		std::vector<std::vector<std::complex<double>>>& img_output)
{
	unsigned column;
	if(row_number<WIDTH/2)
		column = row_number+WIDTH/2;
	else
		column = row_number-WIDTH/2;

	for (int k = 0; k<HEIGHT; k++)
	{
		double row_coefficient = 2 * M_PI * k / HEIGHT;

		std::complex<double> transformed_pixel;
		auto* pixel = first_pixel;

		for (int n = 0; n < WIDTH; n++)
		{
			std::complex<double> tmp(0,row_coefficient*n);
			transformed_pixel += *pixel * std::exp(tmp);
			pixel++;
		}
		img_output[k][column] = ((double)1/HEIGHT) * transformed_pixel;
	}
}

void Task4::i_transform_row(int row_number, std::complex<double>* first_pixel, cimg_library::CImg<double>& img_output)
{
	for (int k = 0; k<WIDTH; k++)
	{
		//-j*2*pi*k/N
		double row_coefficient = 2 * M_PI * k / WIDTH;

		std::complex<double> transformed_pixel;
		auto* pixel = first_pixel;
		for (int n = 0; n < WIDTH; n++)
		{
			std::complex<double> tmp(0,row_coefficient*n);
			transformed_pixel += *pixel * std::exp(tmp);
			pixel++;
		}
		img_output(row_number,k) = abs(transformed_pixel);
	}
}

void Task4::print_abs()
{
	cimg_library::CImg<double> new_img(HEIGHT,WIDTH);
	auto pixel = new_img.begin();

	for(auto& row : img_transformed)
		for(auto& value : row)
			*pixel++ = log10(abs(value));

	new_img.display("DFT. Magnitude spectrum",false, nullptr,true);
}

void Task4::print_arg()
{
	cimg_library::CImg<double> new_img(HEIGHT,WIDTH);
	auto pixel = new_img.begin();

	for(auto& row : img_transformed)
		for(auto& value : row)
			*pixel++ = arg(value);

	new_img.display("DFT. Phase spectrum",false,nullptr,true);
}

void Task4::print_real()
{
	cimg_library::CImg<double> new_img(HEIGHT,WIDTH);
	auto pixel = new_img.begin();
	for(auto& row : img_transformed)
		for(auto& value : row)
			*pixel++ = log10(real(value));

	new_img.display("DFT. Real spectrum",false,nullptr,true);
}

void Task4::print_imag()
{
	cimg_library::CImg<double> new_img(HEIGHT,WIDTH);
	auto pixel = new_img.begin();
	for(auto& row : img_transformed)
		for(auto& value : row)
			*pixel++ = imag(value);

	new_img.display("DFT. Imaginary spectrum",false,nullptr,true);
}

uint32_t Task4::reverse_bits(uint32_t n, size_t size)
{
	uint32_t x=0;
	for(; size>0; size--)
	{
		x<<=1;
		x |= n&1;
		n>>=1;
	}
	return x;
}



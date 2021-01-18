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

	new_img.display("After IDFT",false,0,true);

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

template<typename T>
void Task4::DFT_1D(const T* input_row, int output_column_nr, std::vector<std::vector<std::complex<double>>>& output_matrix)
{
	for (int k = 0; k<WIDTH; k++)
	{
		//-j*2*pi*k/N
		//double row_coefficient = 2 * M_PI * k / WIDTH;

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
	initialize_coefficiens(false);
	std::vector<std::vector<std::complex<double>>> matrix_tmp(WIDTH,std::vector<std::complex<double>>(HEIGHT));

	for (unsigned y = 0; y<HEIGHT/2; y++)
		DFT_1D(img->data(0,y),y+HEIGHT/2,matrix_tmp);
	for (unsigned y = HEIGHT/2; y<HEIGHT; y++)
	{
		DFT_1D(img->data(0,y),y-HEIGHT/2,matrix_tmp);
		{
			using namespace std;
			cout<<matrix_tmp[y][y]<<";";
		}
	}

	for (unsigned y = 0; y<HEIGHT/2; y++)
		DFT_1D(&matrix_tmp[y][0],y+HEIGHT/2,this->img_transformed);

	for (unsigned y = HEIGHT/2; y<HEIGHT; y++)
	{
		DFT_1D(&matrix_tmp[y][0],y-HEIGHT/2,this->img_transformed);
		{
			using namespace std;
			cout<<matrix_tmp[0][y]<<",";
		}
	}
}

void Task4::initialize_coefficiens(bool inverse)
{
	double sign = inverse? -2 : 2;
	coefficients_for_k = std::vector<double>(HEIGHT);
	for (unsigned k = 0; k<HEIGHT; k++)
	{
		coefficients_for_k[k]=sign * M_PI * k / WIDTH;
	}

}





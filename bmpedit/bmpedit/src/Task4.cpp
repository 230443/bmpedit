//
// Created by daniel on 15.01.2021.
//

#include "include/Task4.h"
#include <iostream>


Task4::Task4(cimg_library::CImg<unsigned char>* img)
		:img(img),img_transformed(512)
{
	for (int y = 0; y < WIDTH; y++)
	{
		img_transformed[y].resize(HEIGHT);
	}
}



void Task4::transform_row(int row_number, unsigned char* first_pixel,
		std::array<std::array<std::complex<double>, WIDTH>, HEIGHT>& img_output)
{
	for (int k = 0; k<img->width(); k++)
	{
			//-j*2*pi*k/N
		double row_coefficient = -2 * M_PI * k / img->width();

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
		double row_coefficient = -2 * M_PI * k / img->width();

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
	for (int k = HEIGHT/2; k<HEIGHT; k++)
	{
		double row_coefficient = -2 * M_PI * k / img->width();

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
	std::array<std::array<std::complex<double>,WIDTH>,HEIGHT> img_tmp;
	for (int y = 0; y<img->height(); y++)
	{
		transform_row(y, img->data(0, y), img_tmp);
		//{
		//	using namespace std;
		//	cout<<img_tmp[0][y]<<";";
		//}
	}
	for (int y = 0; y<img->height(); y++)
	{
		transform_row_and_shift(y, &img_tmp[y][0]);
	}

	//print_magnitude();
}

void Task4::print_magnitude()
{
	cimg_library::CImg<double> new_img(HEIGHT,WIDTH);

	for(int y = 0 ; y<512 ; y++)
		for(int x = 0 ; x<512 ; x++)
		{
			new_img(x,y)=log10(abs(img_transformed[x][y]));
		}
	new_img.display("After DFT",false,0,true);
}

void Task4::IDFT()
{
	std::array<std::array<std::complex<double>,WIDTH>,HEIGHT> img_tmp;
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
		std::array<std::array<std::complex<double>, WIDTH>, HEIGHT>& img_output)
{
	//{
	//	using namespace std;
	//	cerr<<*first_pixel<<";";
	//	//if (!y%7)
	//	//cout<<"#";
	//}
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

	for (int k = 0; k<img->width(); k++)
	{
		//-j*2*pi*k/N
		double row_coefficient = 2 * M_PI * k / img->width();

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



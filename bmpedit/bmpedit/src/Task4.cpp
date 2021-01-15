//
// Created by daniel on 15.01.2021.
//

#include "include/Task4.h"


Task4::Task4(cimg_library::CImg<unsigned char>& img)
		:img(img), img_transformed(img.width(), img.height())
{
}

void Task4::DFT_1D()
{
	for (int y = 0; y<img.height(); y++)
		transform_row(y);
}

void Task4::transform_row(int row_number)
{
	unsigned char * first_pixel = img.data(0,row_number);
	//byte* last_pixel = img->data(img->width()-1,row_number);
	for (int k = 0; k<img.width(); k++)
	{
		double row_coefficient;	//-j*2*pi*k/N
		row_coefficient = -2 * M_PI * k / img.width();

		std::complex<double> transformed_pixel;

		unsigned char* pixel = first_pixel;
		for (int n=0; n<img.width(); n++)
			transformed_pixel += std::polar((double)*(pixel++),row_coefficient*n);
		
		img_transformed(k,row_number) = transformed_pixel;
	}
}

void Task4::DFT()
{
	DFT_1D();
	img_transformed.rotate(-90);
	DFT_1D();
	img_transformed.rotate(90);
}

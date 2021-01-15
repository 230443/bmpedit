//
// Created by daniel on 15.01.2021.
//

#include <complex.h>
#include "include/Task4.h"

Task4::Task4(std::string&& filename)
		:Bitmap("filename"), img_transformed(Bitmap::image.width(), Bitmap::image.height())
{
}

void Task4::DFT_1D()
{
	for (int y = 0; y<image.height(); y++)
		transform_row(y);
}

void Task4::transform_row(int row_number)
{
	byte* first_pixel = image.data(0,row_number);
	//byte* last_pixel = image.data(image.width()-1,row_number);
	for (int k = 0; k<image.width(); k++)
	{
		double row_coefficient;	//-j*2*pi*k/N
		row_coefficient = -2 * M_PI * k / image.width();

		std::complex<double> transformed_pixel;

		byte* pixel = first_pixel;
		for (int n=0; n<image.width(); n++)
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

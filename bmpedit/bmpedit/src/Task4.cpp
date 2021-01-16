//
// Created by daniel on 15.01.2021.
//

#include "include/Task4.h"
#include <iostream>


Task4::Task4(cimg_library::CImg<unsigned char>* img)
		:img(img),img_transformed(512),img_transformed_shifted(512)
{
	//for (int y = 0; y<img->height(); y++)
	//{
	//	img_transformed_shifted.emplace_back();
	//	for (int y = 0; y<img->height(); y++)
	//	{
	//		img_transformed_shifted[y].emplace_back();
	//	}
	//}

}

void Task4::DFT_1D()
{
	for (int y = 0; y<img->height(); y++)
	{
		transform_row(y);
		{
			using namespace std;
			cout<<y<<";";
		}
	}
}

void Task4::transform_row(int row_number)
{
	img_transformed.emplace_back();
	unsigned char* first_pixel = img->data(0,row_number);
	//byte* last_pixel = img->data(img->width()-1,row_number);
	for (int k = 0; k<img->width(); k++)
	{
			//-j*2*pi*k/N
		double row_coefficient = -2 * M_PI * k / img->width();
		std::complex<double> transformed_pixel;
		unsigned char* pixel = first_pixel;

		for (int n=0; n<img->width(); n++)
		{
			transformed_pixel += std::polar((double)*pixel, row_coefficient * n);
			pixel++;
		}

		//img_transformed.resize(512);
		//{
		//	using namespace std;
		//	cout<<"after transform_row"<<endl;
		//	cout<<"vector size:"<<img_transformed[0].capacity()<<endl;
		//}

		//img_transformed[row_number][k] = transformed_pixel;
		img_transformed[row_number].push_back(transformed_pixel);
	}
}

void Task4::DFT()
{
	for (int y = 0; y<img->height(); y++)
	{
		transform_row(y);
		{
			using namespace std;
			cout<<y<<",";
		}
	}
	//for (int y = 0; y<img->height(); y++)
	//{
	//	transform_col(y);
	//	{
	//		using namespace std;
	//		cout<<y<<";";
	//	}
	//}


	print_spatial_domain();
}

void Task4::print_spatial_domain()
{

	for(int y = 0 ; y<512 ; y++)
		for(int x = 0 ; x<512 ; x++)
		{
			*(img->begin()+y*512+x)=log10(abs(img_transformed[x][y]));
		}

}
void Task4::transform_col(int row_number)
{
	int new_row;

	if (row_number < 512 / 2)
		new_row = row_number + 512 / 2;
	else
		new_row = row_number - 512 / 2;

	for (int k = 0; k < 512 / 2; k++)
	{
		//-j*2*pi*k/N
		double row_coefficient = -2 * M_PI * k / img->width();
		std::complex<double> transformed_pixel = 0.0;

		int n = 0;

		for (auto& pixel : img_transformed[row_number])
		{
			transformed_pixel += pixel * std::polar((double)1, row_coefficient * n);
		}
		img_transformed_shifted[new_row][k + 512 / 2] = transformed_pixel;
	}
	for (int k = 512 / 2; k < 512; k++)
	{
		//-j*2*pi*k/N
		double row_coefficient = -2 * M_PI * k / img->width();
		std::complex<double> transformed_pixel = 0.0;

		int n = 0;

		for (auto& pixel : img_transformed[row_number])
		{
			transformed_pixel += pixel * std::polar((double)1, row_coefficient * n);
		}
		img_transformed_shifted[new_row][k - 512 / 2] = transformed_pixel;
	}
}

//
// Created by daniel on 24.11.2020.
//
#include "Bitmap.h"
#include <iostream>
using namespace cimg_library;

void Bitmap::make_hist(int&& color)
{
	byte* p = image.data(0, 0, color);
	byte* r = p + offset;
	while (p < r)
	{
		h[*p]++;
		p++;
	}
}
void Bitmap::hexponent(int gmin, int gmax)
{
	make_hist();

	byte g[256]={0};
	double sum=0;

	double rN = (double)1/(H*W);					// reciprocal N
	double eps= 0.01;								// epsilon
	double a = -(double)(gmax-gmin)/std::log(eps);	// reciprocal alpha

	for(int i = 0; i<256; i++)
	{
		sum+=h[i]*rN;
		g[i]=gmin - a*std::log(1-sum+eps);
	}
	for (auto& p : image)
		p = g[p];
}

void Bitmap::histogram()
{
	make_hist();
	uint32_t max=0;
	for(auto i: h)
		if (i > max) max = i;
	CImg<byte> hist(256,256,1,1,0);
	for(int i = 0; i<256; i++)
	{
		byte* p = hist.data(0,i,0);
		byte* r = p + (int)(h[i]*255/max);
		while (p < r)
			*(p++)=255;
	}
	hist.rotate(270);
	hist.save("histogram.bmp");
}

byte Bitmap::osobel(byte *tab, int size, int d)
{
	int X = tab[2] + 2*tab[5] + tab[8]-
			tab[0] - 2*tab[3] - tab[6];
	int Y = tab[0] + 2*tab[1] + tab[2]-
			tab[6] - 2*tab[7] - tab[8];
	int result =std::sqrt(X*X+Y*Y);
	if (result>255) return 255;
	return result;
	//return std::sqrt(X*X+Y*Y);
}

double Bitmap::mean()
{
	make_hist();
	uint64_t sum=0;
	for (int i=0; i<255; ++i)
		sum += i*h[i];
	return (double)sum/offset;
}

double Bitmap::casyco()
{
	double mean = Bitmap::mean();
	double sum2 = 0;
	double sum3 = 0;
	for (int i=0; i<255; ++i)
	{
		sum2 += (i - mean) * (i - mean) * h[i];
		sum3 += (i - mean) * (i - mean) * (i - mean) * h[i];
	}
	double sd = sqrt(sum2/(double)offset);
	return 1/(sd*sd*sd)*sum3/(double)offset;
}



void Bitmap::slaplace(int* kernel)
{

	byte(*mask)(const byte* i, int kernel[], int W);
	if(kernel == nullptr)
		mask = &mask0;
	else
		mask = &mask9;
	CImg<byte> tmp(W, H, 1, image.spectrum());			//temporary image

	int w = W - 2;								//new width

	byte* i = image.data(1, 1);					//iterator on the image
	byte* t = tmp.data(1, 1);					//iterator on the tmp
	byte* ir = i + w;									//last in a row
	byte* last = ir + W * (H - 1 * 2 - 1);			//last in a given color

	for (int s = 0; s != image.spectrum(); ++s)			//select pixel to apply filter to
	{
		while (i < last)
		{
			while (i < ir)
			{
				*t = mask(i, kernel, W);
				i++;
				t++;
			}
			i += 2;				//go to the next line
			t += 2;
			ir = i + w;
		}
		i += 2 * W;				//go to the next color
		t += 2 * W;
		ir = i + w;
		last += offset;
	}
	copy_frame(tmp, 1);
}

byte Bitmap::mask0(const byte* i, int* kernel, int W)
{
	int sum = 0;
	sum += *(i - W)*(-1);
	sum += *(i - 1)*(-1);
	sum += *(i  )*( 4);
	sum += *(i + 1)*(-1);
	sum += *(i + W)*(-1);
	return (byte)(sum*128/400+127);
}
byte Bitmap::mask9(const byte* i, int* kernel, int W)
{

	int sum = 0;
	sum += *(i - W - 1	) * (*(kernel++));
	sum += *(i - W		) * (*(kernel++));
	sum += *(i - W + 1	) * (*(kernel++));

	sum += *(i 		- 1	) * (*(kernel++));
	sum += *(i 			) * (*(kernel++));
	sum += *(i 		+ 1	) * (*(kernel++));

	sum += *(i + W - 1	) * (*(kernel++));
	sum += *(i + W 		) * (*(kernel++));
	sum += *(i + W - 1	) * (*(kernel));

	return (byte)(sum*128/400+127);
}


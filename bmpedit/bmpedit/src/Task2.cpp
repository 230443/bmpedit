//
// Created by daniel on 24.11.2020.
//
#include "../include/Bitmap.h"
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
	std::string histName = ifname.substr(0,ifname.size()-4) + "_hist.bmp";
	hist.save(histName.c_str());
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


byte Bitmap::mask0(const byte* i, const int8_t* kernel, int W)
{
	int sum = 0;
	sum += *(i - W)*(-1);
	sum += *(i - 1)*(-1);
	sum += *(i  )*( 4);
	sum += *(i + 1)*(-1);
	sum += *(i + W)*(-1);
	return (byte)(sum*128/400+127);
}
byte Bitmap::mask9(const byte* i, const int8_t* kernel, int W)
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

	return (byte)(sum*128/800+127);
}


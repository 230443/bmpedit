#pragma once
#include <string>
#include <algorithm>
#include <numeric>
#include "CImg.h"

typedef unsigned char byte;

class Bitmap 
{
public:
	cimg_library::CImg<byte> image;
private:
	int H, W;
	long offset;
	void optimize(cimg_library::CImg<byte>& image);
	void set_new_image(cimg_library::CImg<byte>& tmp);
	void make_arr(byte* p, int& win_s, byte* tab);
	byte contr(byte* tab, int size, int Q);
	void copy_frame(cimg_library::CImg<byte>& tmp, int win_s);
public:
	Bitmap(const char* const&& filename);
	~Bitmap();
	//modify brightness, contrast and RGB components
	void brightness(int val);
	void contrast(float a);
	void negative();
	//simple geometric operations.
	void hflip();
	void vflip();
	void dflip();
	//shrinking and enlargement
	void shrink(int k);
	void enlarge(int k);
		//filters
	//Alpha - trimmed mean filter(--alpha)
	//win_s - window size - distance from a center pixel
	//d - number of discarded pixels
	void alpha(int win_s = 1, int d = 4);									
	//contraharmonic mean filter(--cmean).
	void cmean(int win_s = 1, int Q=2);

	//	(E1)Mean square error(--mse).
	double mse(const char* const fname);
	//	(E2)Peak mean square error(--pmse).
	double pmse(const char* const fname);
	//	(E3)Signal to noise ratio(--snr).
	double snr(const char* const fname);
	//	(E4)Peak signal to noise ratio(--psnr).
	double psnr(const char* const fname);
	//	(E5)Maximum difference(--md)
	double md(const char* const fname);






	void save(std::string ofname);	//method overloaded, save(const char* const)
};


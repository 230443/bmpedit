#pragma once
#include <string>
#include <algorithm>
#include <cmath>
#include "CImg.h"

typedef unsigned char byte;

class Bitmap 
{
public:
	cimg_library::CImg<byte> image;
	void optimize(cimg_library::CImg<byte>& image);
private:
	int H, W;
	long offset;
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
	void alpha( int d = 4, int win_s = 1);									
	//contraharmonic mean filter(--cmean).
	void cmean(int Q=2, int win_s = 1);

	//	(E1)Mean square error(--mse).
	float mse(cimg_library::CImg<byte>& ref);
	//	(E2)Peak mean square error(--pmse).
	float pmse(cimg_library::CImg<byte>& ref);
	//	(E3)Signal to noise ratio(--snr).
	float snr(cimg_library::CImg<byte>& ref);
	//	(E4)Peak signal to noise ratio(--psnr).
	float psnr(cimg_library::CImg<byte>& ref);
	//	(E5)Maximum difference(--md)
	float md(cimg_library::CImg<byte>& ref);






	void save(std::string ofname);	//method overloaded, save(const char* const)
};


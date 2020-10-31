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
	bool is_mono;
	void set_new_image(cimg_library::CImg<byte>& tmp);
	void make_arr(byte* p, int win_s, byte tab[]);
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
	void alpha(int win_size = 1, int d = 2);	//win_size - distance from center pixel
												//d - number of discarded pixels
	//contraharmonic mean filter(--cmean).






	void save(std::string ofname);	//method overloaded, save(const char* const)
};


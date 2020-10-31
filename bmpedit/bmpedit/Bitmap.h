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
	void cmean(int win_s = 1);






	void save(std::string ofname);	//method overloaded, save(const char* const)
};


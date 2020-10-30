#pragma once
#include <string>
#include <algorithm>
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






	void save(std::string ofname);	//method overloaded, save(const char* const)
};


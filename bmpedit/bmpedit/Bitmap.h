#pragma once
#include <string>
#include <algorithm>
#include "CImg.h"

typedef unsigned char byte;

class Bitmap 
{
public:
	cimg_library::CImg<byte> image;
	int H, W;
public:
	Bitmap(const char* const&& filename);
	~Bitmap();
	void set_new_image(cimg_library::CImg<byte>& tmp);
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


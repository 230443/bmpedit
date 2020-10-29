#pragma once
#include <string>
#include <algorithm>
#include "CImg.h"

class Bitmap 
{
public:
	cimg_library::CImg<unsigned char> image;
	const unsigned int H, W, SP, S;
public:
	Bitmap(const char* const filename);
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
	void shrink();
	void enlarge();





	void save(std::string ofname);	//method overloaded, save(const char* const)
};


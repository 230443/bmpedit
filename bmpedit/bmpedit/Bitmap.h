#pragma once
#include <string>
#include "CImg.h"

using namespace cimg_library;

class Bitmap : public CImg<unsigned char>
{
public:
	Bitmap(const char* const filename);
	//modify brightness, contrast and RGB components
	void brightness(int val);
	void contrast(float a);
	void negative();
	//simple geometric operations.



	void save(std::string ofname);	//method overloaded, save(const char* const)
};


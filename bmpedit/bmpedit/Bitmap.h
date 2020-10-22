#pragma once
#include <string>
#include "Cimg.h"

using namespace cimg_library;

class Bitmap : public CImg<unsigned char>
{
public:
	Bitmap(const char* const filename);
	//modify brightness, contrast and RGB components 
	void brightness(int val);
	void contrast(float val);
	//simple geometric operations.



	void save(std::string ofname);	//method overloaded, save(const char* const)
};


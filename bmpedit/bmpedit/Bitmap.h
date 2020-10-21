#pragma once
#include <string>
#include "Cimg.h"

using namespace cimg_library;

class Bitmap
{
	CImg<unsigned char> image;
public:
	Bitmap(const char* const filename);

	
};


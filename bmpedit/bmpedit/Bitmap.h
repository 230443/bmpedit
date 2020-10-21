#pragma once
#include <string>
#include "Cimg.h"

using namespace cimg_library;

class Bitmap
{
	int m, n;
	
public:
	unsigned char** pix;
	Bitmap(const char* const filename);
	
};


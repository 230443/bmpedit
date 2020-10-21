#pragma once
#include <string>
#include "Cimg.h"

using namespace cimg_library;

class Bitmap : private CImg<unsigned char>
{
public:
	Bitmap(const char* const filename);
	int pixvalue(int x=0, int y=0);
};


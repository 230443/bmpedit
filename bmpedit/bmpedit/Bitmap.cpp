#include "Bitmap.h"

Bitmap::Bitmap(const char* const filename)
{
	CImg<unsigned char> image(filename);
}

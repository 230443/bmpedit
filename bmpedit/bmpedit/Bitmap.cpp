#include "Bitmap.h"

Bitmap::Bitmap(const char* const filename)
	:CImg<unsigned char>(filename)
{

}

int Bitmap::pixvalue(int x, int y)
{
	return (*data(x, y));
}

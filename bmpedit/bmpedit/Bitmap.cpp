#include "Bitmap.h"

using namespace cimg_library;

Bitmap::Bitmap(const char* const filename)
	:image(filename), H(image.height()), W(image.width()), SP(image.spectrum()), S(image.size()) 	//CImg creates one pixel array. First all red pixels then G and B.									//RRR..BBB...GGG not RGBRGBRGB
{
}
Bitmap::~Bitmap()
{
	~image;
}
void Bitmap::brightness(int val)
{
	unsigned char* ptr = image.begin();
	if (val < 0)
	{
		for (; ptr != image.end(); ++ptr)
		{
			if (*ptr > 0)
				*ptr += val;
		}
	}
	else 
	{
		for (; ptr != image.end(); ++ptr)
		{
			if (*ptr < 255)
				*ptr += val;
		}
	}

}
void Bitmap::contrast(float a) // y=ax+y_0-ax_0, where (x_0,y_0)is middle point of the spectrum
{
	float b = (1 - a) * 128;

	unsigned char tab[256];
	for (int i = 0; i < 256; i++)
	{
		int y = a * i + b;
		if (y >= 255)		tab[i] = 255;
		else if (y <= 0)	tab[i] = 0;
		else				tab[i] = y;
	}
	for (unsigned char* ptr = image.begin(); ptr != image.end(); ++ptr)
	{
		*ptr = tab[*ptr];
	}
}
void Bitmap::negative()
{
	for (unsigned char* ptr = image.begin(); ptr != image.end(); ++ptr)
	{
		*ptr = ~*ptr;
	}
}
void Bitmap::hflip()
{
	for (int s = 0; s != SP; ++s)
	{
		for (int x = 0; x != W; ++x)
		{
			for (int y = 0; y != H/2; ++y)
			{
				std::swap(image(x, y, s), image(x, (H - y), s));

			}
		}
	}
}
void Bitmap::vflip()
{
}
void Bitmap::dflip()
{
}
void Bitmap::shrink()
{
}
void Bitmap::enlarge()
{
}
void Bitmap::save(std::string ofname)
{
	image.save(ofname.c_str());
}


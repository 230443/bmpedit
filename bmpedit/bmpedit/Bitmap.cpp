#include "Bitmap.h"

Bitmap::Bitmap(const char* const filename)
	:CImg<unsigned char>(filename)
{
}
void Bitmap::brightness(int val)
{
	unsigned char* ptr = begin();
	if (val < 0)
	{
		for (int i = 0; i < height() * width() * 3; i++, ptr++)
		{
			if (*ptr > 0)
				*ptr += val;
		}
		display();
	}
	else 
	{
		for (int i = 0; i < height() * width() * 3; i++, ptr++)
		{
			if (*ptr < 255)
				*ptr += val;
		}
		display();
	}

}
void Bitmap::contrast(float a) // y=ax+y_0-ax_0, (x_0,y_0) middle of the spectrum
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
	unsigned char* ptr = begin();
	for (int i = 0; i < height() * width() * 3; i++, ptr++)
	{
		*ptr = tab[*ptr];
	}
	display();
}
void Bitmap::save(std::string ofname)
{
	CImg::save(ofname.c_str());
}


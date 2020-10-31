#include "Bitmap.h"

using namespace cimg_library;

Bitmap::Bitmap(const char* const&& filename)
	:image(filename), H(image.height()), W(image.width()) 	//CImg creates one pixel array. First all red pixels then G and B.									//RRR..BBB...GGG not RGBRGBRGB
{
	offset = W*H;
	is_mono = true;
	if (image.spectrum() == 3)
	{
		for (byte* ptr = image.begin(); ptr != (image.begin() + offset); ++ptr)
		{
			if (*ptr == *(ptr + offset) == *(ptr + offset * 2))
			{
				is_mono = false;
				break;
			}
		}
	}
	if (is_mono)
	{
		CImg<byte> tmp(image.data(), image.width(), image.height());
		image = tmp;
	}
}
Bitmap::~Bitmap()
{
	~image;
}
void Bitmap::set_new_image(cimg_library::CImg<byte> &tmp)
{
	image = tmp;
	H = image.height();
	W = image.width();
}

void Bitmap::make_arr(byte* p, int win_s, byte tab[])
{
	int i = 0;
	for (int xw = -win_s; xw <= win_s; ++xw)
	{
		for (int yw = -win_s; yw <= win_s; ++yw)
		{
			tab[i++] = *(p + xw + (yw * W));
		}
	}
}


void Bitmap::brightness(int val)
{
	byte* ptr = image.begin();
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

	byte tab[256];
	for (int i = 0; i < 256; i++)
	{
		int y = a * i + b;
		if (y >= 255)		tab[i] = 255;
		else if (y <= 0)	tab[i] = 0;
		else				tab[i] = y;
	}
	for (byte* ptr = image.begin(); ptr != image.end(); ++ptr)
	{
		*ptr = tab[*ptr];
	}
}
void Bitmap::negative()
{
	for (byte* ptr = image.begin(); ptr != image.end(); ++ptr)
	{
		*ptr = ~*ptr;
	}
}
void Bitmap::vflip()
{
	int h = H - 1;
	for (int x = 0; x != W; ++x)
	{
		for (int y = 0; y != H/2; ++y)
		{

				std::swap(image(x, y, 0), image(x, (h - y), 0));
				std::swap(image(x, y, 1), image(x, (h - y), 1));
				std::swap(image(x, y, 2), image(x, (h - y), 2));
		}
	}
}
void Bitmap::hflip()
{
	int w = W - 1;
	for (int x = 0; x != W/2; ++x)
	{
		for (int y = 0; y != H; ++y)
		{
			std::swap(image(x, y, 0), image((w - x), y, 0));
			std::swap(image(x, y, 1), image((w - x), y, 1));
			std::swap(image(x, y, 2), image((w - x), y, 2));
		}
	}
}
void Bitmap::dflip()
{

	for (int s = 0; s != image.spectrum(); ++s)
	{
		byte* ptr = image.begin() + s * W * H;
		byte* rptr = image.begin() + ((s + 1) * W * H) - 1;
		for (; ptr<rptr; ++ptr,--rptr)
		{
			std::swap(*ptr, *rptr);
		}
	}
}
void Bitmap::shrink(int k)
{
	if (!(W % k))
	{
		CImg<byte> tmp(W / k, H / k, 1, image.spectrum());
		byte* it = image.begin();
		byte* ptr = tmp.begin();
		while (it < image.end())
		{
			byte* endptr = it + W;
			while (it < endptr)
			{
				*ptr = *it;
				ptr++;
				it += k;
			}
			it += W * (k - 1);
		}
		set_new_image(tmp);
	}
	
}
void Bitmap::enlarge(int k)		//k=2,3,4...
{								//not optimal - 4 loops
	
	CImg<byte> tmp(W*k, H*k, 1, image.spectrum());
	byte* it = image.begin();
	byte* ptr = tmp.begin();
	while (it < image.end())
	{
		byte* endptr = ptr + tmp.width();
		while (ptr < endptr)
		{
			byte* endptr = ptr + k;
			while (ptr < endptr)
			{
				*ptr = *it;
				for (int i = 1; i != k; ++i)
						*(ptr + (tmp.width() * i)) = *it;
				ptr++;
			}
			it++;
		}
		ptr += tmp.width() * (k - 1);
		
	}
	set_new_image(tmp);
}

void Bitmap::alpha(int win_size, int d)
{
	d = d / 2;
	int size = (win_size * 2 + 1) * (win_size * 2 + 1);
	byte* tab = new byte[size];
	CImg<byte> tmp(W, H, 1, image.spectrum());
	//CImg<byte> tmp(image);
	for (int s = 0; s != image.spectrum(); ++s)
	{
		for (int x = win_size; x < (W - win_size); ++x)
		{
			for (int y = win_size; y < (H - win_size); ++y)
			{
				make_arr(image.data(x, y, s), win_size, tab);
				std::sort(tab, tab + size);

				tmp(x, y, s) = std::accumulate(tab + d, tab + size - d, 0) / (size - 2 * d);
				//*(tmp.data()+x+y*W) = image(x, y, 1, s);
				
			}
		}

	}
	image = tmp;
}
	
void Bitmap::save(std::string ofname)
{
	image.save(ofname.c_str());
}


#include "Bitmap.h"

using namespace cimg_library;

Bitmap::Bitmap(const char* const&& filename)
	:image(filename), H(image.height()), W(image.width()) 	//CImg creates one pixel array. First all red pixels then G and B.									//RRR..BBB...GGG not RGBRGBRGB
{
	offset = W*H;
	is_mono = true;
	if (image.spectrum() == 3)								//check bits per pixel
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

void Bitmap::make_arr(byte* p, int& win_s, byte* tab)
{
	byte* pr = p + win_s * W; //end off loop
	for (p -= win_s * W; p <= pr; p += W)
	{
		for (int xw = -win_s; xw <= win_s; ++xw)
		{
			*(tab++) = *(p + xw);
		}
	}
}

byte Bitmap::contr(byte* tab, int size, int Q)
{
	long double sum1 = 0;
	long double sum2 = 0;
	for (byte* end=tab+size; tab != end; tab++)
	{
		double xq = *tab;
		for (int i = 1; i < Q; i++)
			xq *= *tab;
		sum1 += xq;
		sum2 += (*tab) * xq;
	}
	return (byte)(sum2/sum1);
}

void Bitmap::copy_frame(cimg_library::CImg<byte>& tmp, int win_s)
{
	int w = W - 1;
	for (int s = 0; s != image.spectrum(); ++s)									
	{
		if(win_s==1)	//reduce nested loop
			for (int y = 1; y < H - 1; y++)
			{
				tmp(0, y, s) = tmp(1, y, s);
				tmp(w, y, s) = tmp(w-1, y, s);
			}
		else
			for (int y = win_s; y < H - win_s; y++)
			{
				for (int i = 0; i < win_s; i++)
				{
					tmp(0 + i, y, s) = tmp(win_s, y, s);
					tmp(w - i, y, s) = tmp(w - win_s, y, s);
				}
			}
		byte* i = tmp.begin() + W * win_s + W + offset * s; //last value in the first good row
		byte* t = i - W;									//value above i
		byte* tr = tmp.begin() + offset*s;					
		while (t != tr)
		{
			*(--t) = *(--i);
		}
		i += (H - 2 - win_s) * W;						//first value in the last good row
		t = i + W;
		tr += offset;
		while (t != tr)
		{
			*(t++) = *(i++);
		}
	}
	image = tmp;
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
		byte* ptr = image.begin() + s * offset;
		byte* rptr = image.begin() + ((s + 1) * offset) - 1;
		while (ptr<rptr)
			std::swap(*(ptr++), *(rptr--));
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

void Bitmap::alpha(int win_s, int d)
{
	d = d / 2;
	int size = (win_s * 2 + 1) * (win_s * 2 + 1);		//surface of the window
	byte* tab = new byte[size];
	CImg<byte> tmp(W, H, 1, image.spectrum());			//temporary image

	int w = W - 2 * win_s;

	byte* i = image.data(win_s, win_s);					//iterator on the image
	byte* t = tmp.data(win_s, win_s);					//iterator on the tmp
	byte* ir = i + w;									//last in a row
	byte* last = ir + W * (H - win_s * 2 - 1);			//last in a given color

	for (int s = 0; s != image.spectrum(); ++s)			//select pixel to apply filter to
	{
		while(i<last)
		{
			while (i < ir)
			{
				make_arr(i, win_s, tab);
				std::sort(tab, tab + size);

				*t = std::accumulate(tab + d, tab + size - d, 0) / (size - 2 * d);
				i++; t++;
			}
			i += win_s * 2;				//go to the next line
			t += win_s * 2;
			ir = i + w;
		}
		i += win_s * 2 * W;				//go to the next color
		t += win_s * 2 * W;
		ir = i + w;
		last += offset;
	}
	copy_frame(tmp, win_s);
}

void Bitmap::cmean(int win_s, int Q)
{
	int size = (win_s * 2 + 1) * (win_s * 2 + 1);		//surface of the window
	byte* tab = new byte[size];
	CImg<byte> tmp(W, H, 1, image.spectrum());			//temporary image

	int w = W - 2 * win_s;								//new width

	byte* i = image.data(win_s, win_s);					//iterator on the image
	byte* t = tmp.data(win_s, win_s);					//iterator on the tmp
	byte* ir = i + w;									//last in a row
	byte* last = ir + W * (H - win_s * 2 - 1);			//last in a given color

	for (int s = 0; s != image.spectrum(); ++s)			//select pixel to apply filter to
	{
		while (i < last)
		{
			while (i < ir)
			{
				make_arr(i, win_s, tab);
				*t = contr(tab, size, Q);
				i++; t++;
			}
			i += win_s * 2;				//go to the next line
			t += win_s * 2;
			ir = i + w;
		}
		i += win_s * 2 * W;				//go to the next color
		t += win_s * 2 * W;
		ir = i + w;
		last += offset;
	}
	copy_frame(tmp, win_s);
}
	
void Bitmap::save(std::string ofname)
{
	image.save(ofname.c_str());
}


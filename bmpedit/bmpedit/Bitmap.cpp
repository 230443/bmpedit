#include "Bitmap.h"
#include <iostream>
using namespace cimg_library;

Bitmap::Bitmap(const char* const&& filename)
	:image(filename), H(image.height()), W(image.width()) 	//CImg creates one pixel array. First all red pixels then G and B.									//RRR..BBB...GGG not RGBRGBRGB
{
	offset = W*H;
	optimize(image);
    for(int i = 0; i<256; i++)
    {
        h[i]=0;
    }
}
Bitmap::~Bitmap()
{
	~image;
}

void Bitmap::optimize(cimg_library::CImg<byte>& image)
{
	bool is_mono = true;
	unsigned int offset = image.height()* image.width();
	if (image.spectrum() == 3)								//check bits per pixel
	{
		for (byte* ptr = image.begin(); ptr < (image.begin() + offset); ++ptr)
		{
			if (*ptr != *(ptr + offset) || *ptr != *(ptr + offset * 2))
			{
				//std::cout << (int)(ptr - image.begin()) << std::endl;
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

void Bitmap::set_new_image(cimg_library::CImg<byte> &tmp)
{
	image = tmp;
	H = image.height();
	W = image.width();
}

void Bitmap::make_arr(byte* p, int& win_s, byte* tab) const
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
        byte* tr = tmp.begin() + offset * s;
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
void Bitmap::make_hist(int&& color)
{
	byte* p = image.data(0, 0, color);
	byte* r = p + offset;
	while (p < r)
    {
        h[*p]++;
        p++;
    }
}
byte Bitmap::alpha(byte* tab, int size, int d)
{
	std::sort(tab, tab + size);
	int sum = 0;
	for (int i = d; i < size - d; i++)
		sum += tab[i];
	return sum / (size - 2 * d);
}

byte Bitmap::contra(byte* tab, int size, int Q)
{
	long double sum1 = 0;
	if (Q == 0)
	{
		for (byte* end = tab + size; tab != end; tab++)
		{
			sum1 += (*tab);
		}
		return sum1 / size;
	}
	else
	{
		long double sum2 = 0;
		for (byte* end = tab + size; tab != end; tab++)
		{
			double xq = *tab;
			for (int i = 1; i < Q; i++)
				xq *= *tab;
			if (Q < 0) xq = 1 / xq;
			sum1 += xq;
			sum2 += (*tab) * xq;
			//sum1 += pow(*tab,Q);
			//sum2 += pow(*tab,Q+1);
		}
		return (byte)(sum2/sum1);
	}
	
}

void Bitmap::brightness(int val)
{
    byte *ptr = image.begin();
    if (val < 0)
    {
        for (; ptr != image.end(); ++ptr)
        {
            if (*ptr > -val)
                *ptr += val;
            else
                *ptr = 0;
        }
    } else
    {
        for (; ptr != image.end(); ++ptr)
        {
            if (*ptr < 255 - val)
                *ptr += val;
            else
                *ptr = 255;
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
	for (unsigned char & ptr : image)
	{
		ptr = tab[ptr];
	}
}
void Bitmap::negative()
{
	for (byte & ptr : image)
		ptr = ~ptr;
}
void Bitmap::vflip()
{
	int h = H - 1;

	for (int s = 0; s != image.spectrum(); ++s)
	{
		for (int x = 0; x != W; ++x)
		{
			for (int y = 0; y != H / 2; ++y)
			{
				std::swap(image(x, y, s), image(x, (h - y), s));
			}
		}
	}
}
void Bitmap::hflip()
{
	int w = W - 1;
	byte* p;
	byte* r;
	for (int s = 0; s != image.spectrum(); ++s)
	{
		for (int y = 0; y != H; ++y)
		{
			p = image.data(0, y, s);
			r = image.data(w, y, s);
			while (p < r)
			{
				std::swap(*p++, *r--);
			}
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
	CImg<byte> tmp(W / k, H / k, 1, image.spectrum());
	byte* i;
	byte* ir;
	byte* t = tmp.begin();

	for (int s = 0; s != image.spectrum(); ++s)
	{
		for (int y = 0; y < H-k; y = y + k)
		{
			i = image.data(0, y, s);
			//t = tmp.data(0, y, s);
			ir = i + W-k;
			while (i <= ir)
			{
				*(t++) = *i;
				i += k;
			}
		}
	}

	set_new_image(tmp);

}
void Bitmap::enlarge(int k)		//k=2,3,4...
{								

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


void Bitmap::alpha(int d, int win_s)
{
	d = d / 2;
	int size = (win_s * 2 + 1) * (win_s * 2 + 1);		//surface of the window
	byte* tab = new byte[size];
	CImg<byte> tmp(W, H, 1, image.spectrum());			//temporary image

	int dwin_s = 2 * win_s;
	int w = W - dwin_s;
	

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

				int sum = 0;
				for (int i = d; i < size - d; i++)
					sum += tab[i];
				*t = sum / (size - 2 * d);
				i++; t++;
			}
			i += dwin_s;				//go to the next line
			t += dwin_s;
			ir = i + w;
		}
		i += dwin_s * W;				//go to the next color
		t += dwin_s * W;
		ir = i + w;
		last += offset;
	}
	copy_frame(tmp, win_s);
    delete[] tab;
}

void Bitmap::cmean(int Q, int win_s)
{
	int size = (win_s * 2 + 1) * (win_s * 2 + 1);		//surface of the window
	byte* tab = new byte[size];
	CImg<byte> tmp(W, H, 1, image.spectrum());			//temporary image

	int dwin_s = 2 * win_s;
	int w = W - dwin_s;								//new width

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
				*t = contra(tab, size, Q);
				i++; t++;
			}
			i += dwin_s;				//go to the next line
			t += dwin_s;
			ir = i + w;
		}
		i += dwin_s * W;				//go to the next color
		t += dwin_s * W;
		ir = i + w;
		last += offset;
	}
	copy_frame(tmp, win_s);
	delete[] tab;
}

void Bitmap::filter(int n, int win_s, func method)
{
	int size = (win_s * 2 + 1) * (win_s * 2 + 1);		//surface of the window
	byte* tab = new byte[size];
	CImg<byte> tmp(W, H, 1, image.spectrum());			//temporary image

	int dwin_s = 2 * win_s;
	int w = W - dwin_s;								//new width

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
				*t = method(tab, size, n);
				i++; t++;
			}
			i += dwin_s;				//go to the next line
			t += dwin_s;
			ir = i + w;
		}
		i += dwin_s * W;				//go to the next color
		t += dwin_s * W;
		ir = i + w;
		last += offset;
	}
	copy_frame(tmp, win_s);
	delete[] tab;
}


//========================================
double Bitmap::mse(cimg_library::CImg<byte>& ref)
{
	double sum = 0;
	byte* i = image.begin();					//iterator on the image
	byte* r = ref.begin();						//iterator to reference image
	for (; r < ref.end(); ++i, ++r)
		sum += (*i - *r) * (*i - *r);
	return sum/ref.size();
}

double Bitmap::pmse(cimg_library::CImg<byte>& ref)
{
	double sum = 0;
	byte max = 0;
	byte* i = image.begin();					
	byte* r = ref.begin();						
	for (; r < ref.end(); ++i, ++r)
	{
		sum += (*i - *r) * (*i - *r);
		if (max < *r) max = *r;
	}
	return (sum / ref.size()) / (max * max);
}

double Bitmap::snr(cimg_library::CImg<byte>& ref)
{
	double sum = 0;
	double sumr = 0;
	byte* i = image.begin();					
	byte* r = ref.begin();						
	for (; r < ref.end(); ++i, ++r)
	{
		sum += (*i - *r) * (*i - *r);
		sumr += (*r) * (*r);
	}
	return 10 * std::log10(sumr / sum);
}

double Bitmap::psnr(cimg_library::CImg<byte>& ref)
{
	double sum = 0;
	byte max = 0;
	byte* i = image.begin();					
	byte* r = ref.begin();						
	for (; r < ref.end(); ++i, ++r)
	{
		sum += (*i - *r) * (*i - *r);
		if (max < *r) max = *r;
	}
	return 10 * std::log10((max * max) / (sum/ref.size()));
}

double Bitmap::md(cimg_library::CImg<byte>& ref)
{
	int diff = 0;

	byte* i = image.begin();					
	byte* r = ref.begin();						
	for (; r < ref.end(); ++i, ++r)
		if (diff < std::abs(*i - *r))	diff = std::abs(*i - *r);
	return diff;
}

	
void Bitmap::save(const std::string& ofname) const
{
	image.save(ofname.c_str());
}

void Bitmap::hexponent(int gmin, int gmax)
{
    make_hist();
    //for(int i = 0; i<256; i++)
    //{
    //    std::cout<< i<<": " <<h[i]<<std::endl;
    //}
    byte g[256]={0};
    double sum=0;
    double rN = (double)1/(H*W);
    double eps= 0.01;
    double a = -(double)(gmax-gmin)/std::log(eps);
    for(int i = 0; i<256; i++)
    {
        sum+=h[i]*rN;
        if(sum!=0)
        std::cout<<sum<<std::endl;
        g[i]=gmin - a*std::log(1-sum+eps);
    }
    for (auto& p : image)
        p = g[p];
}

void Bitmap::histogram()
{

}



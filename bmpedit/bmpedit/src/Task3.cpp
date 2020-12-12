//
// Created by daniel on 02.12.2020.
//
#include "../include/Bitmap.h"
#include <iostream>

using namespace cimg_library;

byte Bitmap::dilation(const byte* i, const int8_t* se, int W)
{
	if (*(se++)	&& 	*(i - W - 1	) )	return 255;
	if (*(se++)	&& 	*(i - W		) )	return 255;
	if (*(se++)	&& 	*(i - W + 1	) )	return 255;
	if (*(se++)	&& 	*(i 	- 1	) )	return 255;
	if (*(se++)	&& 	*(i 		) )	return 255;
	if (*(se++)	&& 	*(i 	+ 1	) )	return 255;
	if (*(se++)	&& 	*(i + W - 1	) )	return 255;
	if (*(se++)	&& 	*(i + W 	) )	return 255;
	if (*(se)	&& 	*(i + W + 1	) )	return 255;
	return 0;
	if (*(se++)) if (	*(i - W - 1	) )	return 255;
	if (*(se++)) if (	*(i - W		) )	return 255;
	if (*(se++)) if (	*(i - W + 1	) )	return 255;
	if (*(se++)) if (	*(i 	- 1	) )	return 255;
	if (*(se++)) if (	*(i 		) )	return 255;
	if (*(se++)) if (	*(i 	+ 1	) )	return 255;
	if (*(se++)) if (	*(i + W - 1	) )	return 255;
	if (*(se++)) if (	*(i + W 	) )	return 255;
	if (*(se))	 if (	*(i + W + 1	) )	return 255;
	return 0;
}

byte Bitmap::erosion(const byte* i, const int8_t* se, int W)
{
	if (*(se++)) if (!*(i - W - 1	) )	return 0;
	if (*(se++)) if (!*(i - W		) )	return 0;
	if (*(se++)) if (!*(i - W + 1	) )	return 0;
	if (*(se++)) if (!*(i 		- 1	) )	return 0;
	if (*(se++)) if (!*(i 			) )	return 0;
	if (*(se++)) if (!*(i 		+ 1	) )	return 0;
	if (*(se++)) if (!*(i + W - 1	) )	return 0;
	if (*(se++)) if (!*(i + W 		) )	return 0;
	if (*(se))	 if (!*(i + W + 1	) )	return 0;
	return 255;
}

byte Bitmap::HMT(const byte* i, const int8_t* se, int W)
{
	for (int y = -W; y <= W; y += W)
	{
		for (int x = -1; x <= 1; ++x)
		{
			if (*se)
			{
				if (*se > 0)
					{if (!*(i+x+y)) return 0;}
				else if (*(i+x+y))
					return 0;
			}
			se++;
		}
	}
	return 255;
}


void Bitmap::operation_3x3(unsigned SE_number, char type)
{
	byte(*operation)(const byte* i, const int8_t se[], int W);
	const int8_t * se = &SE[SE_number][0]; 		//choose structural element

	if (type == 'd')		//define operation [dilation/erosion/HMT]
		operation = dilation;
	else if (type == 'e')
		operation = erosion;
	else if (type == 'h')
		operation = HMT;

	CImg<byte> tmp(W, H, 1, image.spectrum());			//temporary image

	int w = W - 2;								//new width

	byte* i = image.data(1, 1);					//iterator on the image
	byte* t = tmp.data(1, 1);					//iterator on the tmp
	byte* ir = i + w;									//last in a row
	byte* last = ir + W * (H - 1 * 2 - 1);			//last in a given color

	for (int s = 0; s != image.spectrum(); ++s)			//select pixel to apply filter to
	{
		while (i < last)
		{
			while (i < ir)
			{
				*t = operation(i, se, W);
				i++;
				t++;
			}
			i += 2;				//go to the next line
			t += 2;
			ir = i + w;
		}
		i += 2 * W;				//go to the next color
		t += 2 * W;
		ir = i + w;
		last += offset;
	}
	copy_frame(tmp, 1);
}

void Bitmap::opening(unsigned int SE_number)
{
	operation_3x3(SE_number, 'e');
	operation_3x3(SE_number, 'd');
}

void Bitmap::closing(unsigned int SE_number)
{
	operation_3x3(SE_number, 'd');
	operation_3x3(SE_number, 'e');
}

//i - pixel from original image , t - pixel from new empty image, se - structural element B


cimg_library::CImg<byte> Bitmap::M3(int x, int y,unsigned SE_number)
{
	CImgDisplay disp(image,"select point",0, false, false);
	while (!disp.is_closed())  //	event loop
		if (disp.button()&1)
		{
			x = disp.mouse_x();
			y = disp.mouse_y();
			disp.close();
		}

	CImg<byte> tmp(W, H, 1, 1,0);
	const int8_t* se = &SE[SE_number][0];
	fill(image.data(x,y),tmp.data(x,y),se);
	tmp.display();

	return tmp;
}

void Bitmap::fill(byte* i, byte* t, const int8_t* se, byte color)
{
	if (!(*t))				//return if already filled
		if (*i)				//return if centre pixel not white (mask must include centre pixel)
		{
			*t = color;
			if(i>image.begin()+W+1 && i<image.end()-W-1 && (i-image.begin())%W != W-1 && (i-image.begin())%W != 0)	// W - image.width()
			{
				int index = 0;
				for (int y = -W; y <= W; y += W)
				{
					for (int x = -1; x <= 1; ++x)
					{
						if (se[index++])
							fill(i + x + y, t + x + y, se, color);
					}
				}
			}
		}
}

cimg_library::CImg<byte> Bitmap::R1(CImg<byte>& seeds, unsigned int SE_number)
{
	const int8_t* se = &SE[SE_number][0];

	CImg<byte> tmp(W, H, 1, 1,0);

	byte* i = image.begin();
	byte* t = tmp.begin();
	byte color = 255;
	for (auto& seed : seeds)
	{
		if (seed)
		{
			fill(i, t, se, color);
			color-=7;						//choose pseudorandom color
			if (color<30)	color=-31;
		}
		i++;
		t++;
	}

	return tmp;
}


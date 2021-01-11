//
// Created by daniel on 02.12.2020.
//
#include "../include/Bitmap.h"
#include <iostream>
#include <queue>
#include <memory>
#include <vector>

using namespace cimg_library;

byte Bitmap::dilation(const byte* i, const int8_t* se, int W)
{
	if ( 	*(i - W - 1	) && *(se++)	)	return 255;
	if ( 	*(i - W		) && *(se++)	)	return 255;
	if ( 	*(i - W + 1	) && *(se++)	)	return 255;
	if ( 	*(i 	- 1	) && *(se++)	)	return 255;
	if ( 	*(i 		) && *(se++)	)	return 255;
	if ( 	*(i 	+ 1	) && *(se++)	)	return 255;
	if ( 	*(i + W - 1	) && *(se++)	)	return 255;
	if ( 	*(i + W 	) && *(se++)	)	return 255;
	if ( 	*(i + W + 1	) && *(se)		)	return 255;
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
	operation = dilation;
	const int8_t * se = &SE[SE_number][0]; 		//choose structural element

	if (type == 'd')		//define operation [dilation/erosion/HMT]
		operation = dilation;
	else if (type == 'e')
		operation = erosion;
	else if (type == 'h')
		operation = HMT;
	else if (type == 'l')
	{
		if (SE_number == 0)
		{
			operation = mask0;
			se = nullptr;
		}
		else
		{
			se = &LAPLACE_MASK[SE_number-1][0];
			operation = mask9;
		}
	}


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

const int8_t* se;
byte color;
long int t_i_offset;

void Bitmap::M3(unsigned SE_number)
{
	CImgDisplay disp(image,"select point",0, false, false);
	unsigned x,y;
	while (!disp.is_closed())  //	event loop
		if (disp.button()&1)
		{
			x = disp.mouse_x();
			y = disp.mouse_y();
			disp.close();
		}

	CImg<byte> tmp(W, H, 1, 1,0);
	se = &SE[SE_number][0];
	color = 255;
	t_i_offset=tmp.begin()-image.begin();
	fill(image.data(x,y));
	//tmp.display();

	image = tmp;
}

void Bitmap::fill(byte* i)
{
	if (*(i+t_i_offset)) return;		//return if already filled
	if (!(*i)) return;	//return if centre pixel is background (mask must include centre pixel)
	*(i+t_i_offset) = color;
	if (
			i > image.begin() + W + 1 &&
			i < image.end() - W - 1 &&
			(i - image.begin()) % W != W - 1 &&
			(i - image.begin()) % W != 0
	)    // W - image.width()
	{
		if (*(se)) 		fill((i - W - 1));
		if (*(se+1)) 	fill((i - W));
		if (*(se+2)) 	fill((i - W + 1));
		if (*(se+3)) 	fill((i - 1));
		if (*(se+5)) 	fill((i + 1));
		if (*(se+6)) 	fill((i + W - 1));
		if (*(se+7)) 	fill((i + W));
		if (*(se+8)) 	fill((i + W + 1));
	}
}

cimg_library::CImg<byte> Bitmap::select_seeds() const
{
	CImg<byte> tmp(W, H, 1, 1,0);

	CImgDisplay disp(image,"select point",0, false, false);
	while (!disp.is_closed())//	event loop
	{
		if (disp.button() & 1)
		{
			tmp(disp.mouse_x(), disp.mouse_y()) = 255;
		}
		if (disp.is_keyQ())
		{
			disp.close();
		}
	}

	return tmp;
}

void Bitmap::R1(unsigned SE_number, cimg_library::CImg<byte>& seeds)
{
	se = &SE[SE_number][0];

	CImg<byte> tmp(W, H, 1, 1,0);

	t_i_offset=tmp.begin()-image.begin();
	byte* i=image.begin();
	color = 255;
	using namespace std;
	vector<unique_ptr<queue<unsigned char>>> regions;
	for (auto& seed : seeds)
	{
		if (seed)
		{
			queue<unsigned char> region;
			region.push(seed);
			regions.push_back(make_unique<queue<unsigned char>>(region));
		}
		i++;
	}
	for(auto region : regions)
	{

	}
	image = tmp;
}




std::stack<byte*> to_be_filled;

void Bitmap::grow(byte* i)
{
	if (*(i+t_i_offset)) return;		//return if already filled
	if (!(*i)) return;	//return if centre pixel is background (mask must include centre pixel)
	*(i+t_i_offset) = color;
	if (
			i > image.begin() + W + 1 &&
					i < image.end() - W - 1 &&
					(i - image.begin()) % W != W - 1 &&
					(i - image.begin()) % W != 0
			)    // W - image.width()
	{
		if (*(se)) 		to_be_filled.push(i - W - 1);
		if (*(se+1)) 	to_be_filled.push(i - W);
		if (*(se+2)) 	to_be_filled.push(i - W + 1);
		if (*(se+3)) 	to_be_filled.push(i - 1);
		if (*(se+5)) 	to_be_filled.push(i + 1);
		if (*(se+6)) 	to_be_filled.push(i + W - 1);
		if (*(se+7)) 	to_be_filled.push(i + W);
		if (*(se+8)) 	to_be_filled.push(i + W + 1);
	}
}

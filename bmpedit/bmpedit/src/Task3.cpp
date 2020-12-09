//
// Created by daniel on 02.12.2020.
//
#include "../include/Bitmap.h"

using namespace cimg_library;

byte Bitmap::dilation(const byte* i, const bool* se, int W)
{
	if (*(se++)) if (	*(i - W - 1	) )	return 255;
	if (*(se++)) if (	*(i - W		) )	return 255;
	if (*(se++)) if (	*(i - W + 1	) )	return 255;
	if (*(se++)) if (	*(i 	- 1	) )	return 255;
	if (*(se++)) if (	*(i 		) )	return 255;
	if (*(se++)) if (	*(i 	+ 1	) )	return 255;
	if (*(se++)) if (	*(i + W - 1	) )	return 255;
	if (*(se++)) if (	*(i + W 	) )	return 255;
	if (*(se))	 if (	*(i + W - 1	) )	return 255;
	return 0;
}

byte Bitmap::erosion(const byte* i, const bool* se, int W)
{
	//if (	*(i - W - 1	) && (*(se++))	)	return 255;
	//if (	*(i - W		) && (*(se++))	)	return 255;
	//if (	*(i - W + 1	) && (*(se++))	)	return 255;
	//if (	*(i 	- 1	) && (*(se++))	)	return 255;
	//if (	*(i 		) && (*(se++))	)	return 255;
	//if (	*(i 	+ 1	) && (*(se++))	)	return 255;
	//if (	*(i + W - 1	) && (*(se++))	)	return 255;
	//if (	*(i + W 	) && (*(se++))	)	return 255;
	//if (	*(i + W - 1	) && (*(se	))	)	return 255;
	if (*(se++)) if (!*(i - W - 1	) )	return 0;
	if (*(se++)) if (!*(i - W		) )	return 0;
	if (*(se++)) if (!*(i - W + 1	) )	return 0;
	if (*(se++)) if (!*(i 		- 1	) )	return 0;
	if (*(se++)) if (!*(i 			) )	return 0;
	if (*(se++)) if (!*(i 		+ 1	) )	return 0;
	if (*(se++)) if (!*(i + W - 1	) )	return 0;
	if (*(se++)) if (!*(i + W 		) )	return 0;
	if (*(se))	 if (!*(i + W - 1	) )	return 0;
	return 255;
}


void Bitmap::basic_morph_operation(unsigned SE_number, char type)
{
	byte(*operation)(const byte* i, const bool se[], int W);
	const bool* se = &SE[SE_number][0]; 		//choose structural element

	if (type == 'd')		//define operation [dilation/erosion]
		operation = dilation;
	else if (type == 'e')
		operation = erosion;



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


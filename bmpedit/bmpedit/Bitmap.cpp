#include "Bitmap.h"

Bitmap::Bitmap(const char* const filename)
{
	CImg<unsigned char> image(filename);
	m = image.height();
	n = image.height();
	unsigned char* first = image.data();
	unsigned char** a = new unsigned char* [m];
	for (int i = 0; i < m; ++i)
		a[i] = new unsigned char[n];
	pix = a;
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			pix[i][j] = *(first++);
		}
	}
}

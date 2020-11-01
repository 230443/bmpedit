#include <iostream>
#include <string>
#include "Bitmap.h"
#include "CImg.h"
using namespace std;

int main(int argc, char* argv[])
{
	string filename, fname;
	//cin >> filename;
	filename = "lena_impulse2.bmp";
	
	cout << filename <<endl;
	Bitmap img(filename.c_str());

	cimg_library::CImg<byte>ref("lena.bmp");
	img.optimize(ref);

	img.image.display();		//for debuging

	cout << img.md(ref) << endl;
	img.alpha(1, 6);
	cout << img.md(ref) << endl;
	img.image.display();

	//img.brightness(20);
	//img.contrast(4);
	//img.negative();
	//img.dflip();
	//img.enlarge(2);
	//img.alpha(2,2);
	//img.cmean(2,4);
	//img.save("lenanew.bmp");
}
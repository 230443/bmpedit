#include <iostream>
#include <string>
#include "Bitmap.h"
using namespace std;

int main(int argc, char* argv[])
{
	string filename;
	//cin >> filename;
	filename = "lenac_impulse2.bmp";
	cout << filename<<endl;
	Bitmap img(filename.c_str());
	img.image.display();		//for debuging
	//img.brightness(20);
	//img.contrast(4);
	//img.negative();
	//img.hflip();
	//img.enlarge(2);
	img.alpha(1, 4);
	img.image.display();
}
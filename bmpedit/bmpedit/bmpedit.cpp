#include <iostream>
#include <string>
#include "Bitmap.h"
using namespace std;

int main(int argc, char* argv[])
{
	string filename;
	//cin >> filename;
	filename = "lenac.bmp";
	cout << filename<<endl;
	Bitmap img(filename.c_str());
	img.image.display();		//for debuging
	//img.brightness(20);
	//img.contrast(4);
	//img.negative();
	//img.dflip();
	img.enlarge(2);
	img.image.display();
}
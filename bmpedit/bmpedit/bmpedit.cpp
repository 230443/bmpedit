#include <iostream>
#include <string>
#include "Bitmap.h"
using namespace std;

int main(int argc, char* argv[])
{
	string filename;
	//cin >> filename;
	filename = "lena.bmp";
	cout << filename<<endl;
	Bitmap img(filename.c_str());
	img.image.display();		//for debuging
	//img.brightness(20);
	//img.contrast(4);
	//img.negative();
	//img.dflip();
	//img.enlarge(2);
	//img.alpha(2,2);
	//img.cmean(2,4);
	//img.save("lenanew.bmp");
	img.image.display();
}
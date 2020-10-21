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
	cout << img.pixvalue(1) << endl
		<< img.pixvalue(50) << endl
		<< img.pixvalue(0, 511) << endl;
}
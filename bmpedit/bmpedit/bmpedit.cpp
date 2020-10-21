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

}
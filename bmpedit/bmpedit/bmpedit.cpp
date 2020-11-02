#include <iostream>
#include <string>
#include "Bitmap.h"
#include "CImg.h"
using namespace std;

void show_usage(string name)
{
	std::cerr << "Usage: " << name << " infile [-r ref] [options] [-o outfile]"
		<< "Options:\n"
		<< "--brightness [val], --contrast [k], --negative, --shrink [k], --enlarge [k], --alpha [d,filter_size], --cmean[Q,filter_size]\n"
		<< "--mse, --pmse, --snr, --psnr, --md\n"
		<< "ref \t reference image for similarity measurements"
		<< endl;
}




int main(int argc, char* argv[])
{
	if (argc < 3) {
		show_usage(argv[0]);
		return 1;
	}
	string arg = argv[1];
	Bitmap img(arg.c_str());
	cimg_library::CImg<byte>ref;
	

	for (int i = 2; i < argc; ++i)
	{
		string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
			show_usage(argv[0]);
			return 0;
		}
		else if (arg == "-r")
		{
			cimg_library::CImg<byte> tmp(argv[++i]);
			ref = tmp;
			img.optimize(ref);
			ref.display();
			img.image.display();
			continue;
		}
		else if (arg == "--brightness")
		{
			img.brightness(atoi(argv[++i]));
			continue;
		}
		else if (arg == "--constrast")
		{
			img.contrast(atof(argv[++i]));
			continue;
		}
		else if (arg == "--negative")
		{
			img.negative();
			continue;
		}
		else if (arg == "--hflip")
		{
			img.hflip();
			continue;
		}
		else if (arg == "--vflip")
		{
			img.vflip();
			continue;
		}
		else if (arg == "--dflip")
		{
			img.dflip();
			continue;
		}
		else if (arg == "--shrink")
		{
			img.shrink(atoi(argv[++i]));
			continue;
		}
		else if (arg == "--enlarge")
		{
			img.enlarge(atoi(argv[++i]));
			continue;
		}
		else if (arg == "--alpha")
		{
			img.alpha(atoi(argv[++i]),atoi(argv[++i]));
			continue;
		}
		else if (arg == "--cmean")
		{
			img.cmean(atoi(argv[++i]), atoi(argv[++i]));
			continue;
		}
		else if (arg == "--mse")
		{
			img.mse(ref);
			continue;
		}

	}/*
	






















	//===========================================================
	//===========================================================
	//===========================================================
	//===========================================================




	string filename, fname;
	//cin >> filename;
	filename = "lena_normal2.bmp";
	
	cout << filename <<endl;
	Bitmap img(filename.c_str());

	cimg_library::CImg<byte>ref("lena.bmp");
	img.optimize(ref);

	img.image.display();		//for debuging

	float mse(cimg_library::CImg<byte> & ref);
	//	(E2)Peak mean square error(--pmse).
	float pmse(cimg_library::CImg<byte> & ref);
	//	(E3)Signal to noise ratio(--snr).
	float snr(cimg_library::CImg<byte> & ref);
	//	(E4)Peak signal to noise ratio(--psnr).
	float psnr(cimg_library::CImg<byte> & ref);
	//	(E5)Maximum difference(--md)
	float md(cimg_library::CImg<byte> & ref);

	cout << img.mse(ref) << endl;
	cout << img.pmse(ref) << endl;
	cout << img.snr(ref) << endl;
	cout << img.psnr(ref) << endl;
	cout << img.md(ref) << endl;

	clock_t start = clock();
	img.alpha(1, 2);
	clock_t end = clock();
	double elapsed = double(end - start) / CLOCKS_PER_SEC;
	cout << endl << elapsed << " s " << endl << endl;


	cout << img.mse(ref) << endl;
	cout << img.pmse(ref) << endl;
	cout << img.snr(ref) << endl;
	cout << img.psnr(ref) << endl;
	cout << img.md(ref) << endl;

	img.image.display();

	//img.brightness(20);
	//img.contrast(4);
	//img.negative();
	//img.vflip();
	//img.enlarge(2);
	//img.alpha(2,2);
	//img.cmean(2,4);
	//img.save("lenanew.bmp");
	//img.image.display();*/
}
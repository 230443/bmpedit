#include <iostream>
#include <string>
#include "Bitmap.h"
#include "CImg.h"
using namespace std;

void show_usage(string name)
{
	std::cerr << "Usage: " << name << " infile [-r ref] [options] [-o outfile]"
		<< "Options:\n"
		<< "--brightness [val], --contrast [a], --negative, --shrink [k], --enlarge [k], --alpha [d,filter_size], --cmean[Q,filter_size]\n"
		<< "--mse, --pmse, --snr, --psnr, --md\n"
		<< "ref - reference image for similarity measurements\n"
		<< "a == (0,1) - decrease contrast\n a == (1,inf) - increase contrast\n"
		<< "filter_size - distance from center pixel"
		<< endl;
}
void wrong_value(string name)
{
	std::cerr << "Value of " << name << " not specified correctly\n";
	show_usage("bmpedit");
}



int main(int argc, char* argv[])
{
	if (argc < 3) {
		show_usage(argv[0]);
		return 1;
	}
	string arg = argv[1];
	Bitmap img(arg.c_str());
	img.image.display();

	cimg_library::CImg<byte>ref;
	bool ref_avaible = false;

	for (int i = 2; i < argc; ++i)
	{
		string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
			show_usage(argv[0]);
			return 0;
		}
		else if (arg == "-r")
		{
			ref.assign(argv[++i]);
			img.optimize(ref);
			ref_avaible = true;
			ref.display();
			continue;
		}
		else if (arg == "--brightness")
		{
			int k = atoi(argv[++i]);
			if (k)
			{
				img.brightness(k);
				continue;
			}
			else
			{
				wrong_value(arg);
				return 1;
			}
			continue;
		}
		else if (arg == "--constrast")
		{
			int k = atof(argv[++i]);
			if (k > 0)
			{
				img.contrast(k);
				continue;
			}
			else
			{
				wrong_value(arg);
				return 1;
			}
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
			int k = atoi(argv[++i]);
			if (k)
			{
				img.shrink(k);
				continue;
			}
			else
			{
				wrong_value(arg);
				return 1;
			}
		}
		else if (arg == "--enlarge")
		{
			int k = atoi(argv[++i]);
			if (k)
			{
				img.enlarge(k);
				continue;
			}
			else
			{
				wrong_value(arg);
				return 1;
			}
		}
		else if (arg == "--alpha")
		{
			if (argc == i + 1 || *argv[i + 1] == '-')
			{
				img.alpha();
				continue;
			}
			else
			{
				int d = atoi(argv[++i]);
				if (argc == i + 1 || *argv[i + 1] == '-')
				{
					img.alpha(d);
					continue;
				}
				else
				{
					int win_s = atoi(argv[++i]);
					img.alpha(d, win_s);
					continue;
				}
			}
		}
		else if (arg == "--cmean")
		{
			int Q = atoi(argv[++i]);
			if (argc == i + 1 || *argv[i + 1] == '-')
			{
				img.cmean(Q);
				continue;
			}
			else
			{
				int win_s = atoi(argv[++i]);
				img.alpha(Q, win_s);
				continue;
			}
		}
		else if (ref_avaible)
		{
			if (arg == "--mse")
			{
				img.mse(ref);
				continue;
			}
			else if (arg == "--pmse")
			{
				img.mse(ref);
				continue;
			}
			else if (arg == "--sne")
			{
				img.mse(ref);
				continue;
			}
			else if (arg == "--psnr")
			{
				img.mse(ref);
				continue;
			}
			else if (arg == "--md")
			{
				img.mse(ref);
				continue;
			}
		}
		else if (arg == "-o")
		{
			string ofname = argv[++i];
			if (!ofname.empty())
				img.save(ofname.c_str());
			else
			{
				wrong_value(arg);
				return 1;
			}
		}
		else
		{
			cout << "Command not found or reference image not specified" << endl;
			show_usage(argv[0]);
			return 1;
		}
	}
	img.image.display();/*






















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
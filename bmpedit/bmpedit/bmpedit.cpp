#include <iostream>
#include <string>
#include "Bitmap.h"
#include "CImg.h"

using namespace std;

void show_usage()
{
std::cerr << "Usage: bmpedit	infile [-r ref] [options|similarity measures] [-o outfile]" << endl
		<< "		infile [options] [-o outfile]" << endl
		<< "		infile [-r ref] [options|similarity measures]" << endl
		<< "	Options:\n"
		<< "	--brightness [n], --contrast [a], --negative," << endl
		<< "		a = (0,1) - decrease contrast\t a = (1,inf) - increase contrast" << endl
		<< "	--shrink [k], --enlarge [k]," << endl
		<< "	--alpha [d] [filter_size], --cmean [Q] [filter_size]" << endl
		<< "		filter_size - distance from center pixel" << endl
		<< "	-r ref" << endl
		<< "		ref - filename of the original image" << endl
		<< "	Similarity measures:\n"
		<< "	--mse, --pmse, --snr, --psnr, --md" << endl
		<< endl;
}
void wrong_value(string name)
{
	std::cerr << "Value of " << name << " not specified correctly\n";
	show_usage();
}



int main(int argc, char* argv[])
{
	if (argc < 3) {
		show_usage();
		return 1;
	}
	string arg = argv[1];
	Bitmap img(arg.c_str());
	img.image.display("Before",false,0,true);

	cimg_library::CImg<byte>ref;
	bool ref_avaible = false;

	for (int i = 2; i < argc; ++i)
	{
		string arg = argv[i];
		//cout << endl << arg << endl;
		if ((arg == "-h") || (arg == "--help")) {
			show_usage();
			return 0;
		}
		// must have an argument
		if (i + 1 < argc)
		{
			if (arg == "-r")
			{
				ref.assign(argv[++i]);
				img.optimize(ref);
				ref_avaible = true;
				//ref.display();
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
			else if (arg == "--contrast")
			{
				float k = atof(argv[++i]);
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
			else if ((i + 2) < argc)
			{
				if (arg == "--alpha")
				{
					if (*argv[i + 1] == '-')
					{
						img.alpha();
						continue;
					}
					else if ((i + 2) < argc);
					{
						int d = atoi(argv[++i]);
						arg = argv[i + 1];
						if (arg.find('-') != string::npos)
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

					if (*argv[i + 1] == '-')
					{
						img.cmean();
						continue;
					}
					else if (i + 2 < argc)
					{
						int d = atoi(argv[++i]);
						arg = argv[i + 1];
						if (*argv[i + 1] == '-')
						{
							img.cmean(d);
							continue;
						}
						else
						{
							int win_s = atoi(argv[++i]);
							img.cmean(d, win_s);
							continue;
						}
					}
				}
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
		if (ref_avaible)
		{
			if (arg == "--mse")
			{
				cout << "MSE\t" << img.mse(ref) << endl;
				continue;
			}
			else if (arg == "--pmse")
			{
				cout << "PMSE\t" << img.pmse(ref) << endl;
				continue;
			}
			else if (arg == "--snr")
			{
				cout << "SNR\t" << img.snr(ref) << " dB" << endl;
				continue;
			}
			else if (arg == "--psnr")
			{
				cout << "PSNR\t" << img.psnr(ref) << " dB" << endl;
				continue;
			}
			else if (arg == "--md")
			{
				cout << "MD\t" << img.md(ref) << endl;
				continue;
			}
		}
		else
		{
			cout << "Command not found or value not specified" << endl;
			show_usage();
			return 1;
		}
	}
    img.image.display("After",false,0,true);
}
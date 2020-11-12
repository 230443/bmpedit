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
void wrong_value(const string& name)
{
	std::cerr << "Value of \"" << name << "\" not specified correctly. ";
}
int is_not_int(const string& arg, string&& val)
{
    cout<< "log_try_catch: " << arg <<" " << val << endl;
	try {
		int k = stoi(val);
	}
	catch (std::invalid_argument& e) {
		wrong_value(arg);
		cerr << "Invalid argument: " << val;
		return 50;
	}
	catch (std::out_of_range& e) {
		wrong_value(arg);
		cerr << "Out of range: " << val;
		return 1;
	}
	return 0;
}
int is_not_float(const string& arg, string&& val)
{
    cout<< "log_try_catch: " << arg <<" " << val << endl;
	try {
		float k = stof(val);
	}
	catch (std::invalid_argument& e) {
		wrong_value(arg);
		cerr << "Invalid argument: " << val ;
		return 1;
	}
	catch (std::out_of_range& e) {
		wrong_value(arg);
        cerr << "Out of range: " << val;
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{

		if (argc > 1)
			if (((string)argv[1] == "-h") || ((string)argv[1] == "--help"))
			{
				show_usage();
				return 0;
			}
		std::cerr << "Not enough arguments. Check: bmpedit --help" << endl;
		return 1;
	}
	string arg = argv[1];
	Bitmap img(arg.c_str());
	img.image.display("Before",false,0,true);

	cimg_library::CImg<byte>ref;
	bool ref_available = false;

	for (int i = 2; i < argc; ++i)
	{

		string arg = argv[i];
        cout<<"log_argv[i]_loop: " <<arg<<" "<< i<<"\n";

		if ((arg == "-h") || (arg == "--help")) {
			show_usage();
			return 0;
		}

        if ((i + 2) < argc)        // must have 2 arguments
        {
            if (arg == "--alpha")
            {
                //cout << endl << "alpha" << endl;
                if (is_not_int(arg, argv[i + 1])) return 1;
                if (is_not_int(arg, argv[i + 2])) return 1;
                img.alpha(atoi(argv[i + 1]), atoi(argv[i + 2]));
                i += 2;
                continue;
            }
            else if (arg == "--cmean")
            {
                cout << endl << "cmean" << endl;
                if (is_not_int(arg, argv[i + 1])) return 1;
                if (is_not_int(arg, argv[i + 2])) return 1;
                img.cmean(atoi(argv[i + 1]), atoi(argv[i + 2]));
                i += 2;
                continue;
            }
        }
		if (i + 1 < argc)      // must have 1 argument
		{
			if (arg == "-r")
			{
				ref.assign(argv[++i]);
				img.optimize(ref);
				ref_available = true;
				//ref.display();
				continue;
			}
			else if (arg == "--brightness")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.brightness(atoi(argv[++i]));
				continue;
			}
			else if (arg == "--contrast")
			{
				if (is_not_float(arg, argv[i + 1])) return 1;
				img.contrast(atof(argv[++i]));
				continue;
			}
			else if (arg == "--shrink")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.brightness(atoi(argv[++i]));
				continue;
			}
			else if (arg == "--enlarge")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.brightness(atoi(argv[++i]));
				continue;
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
				continue;
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
		if (ref_available)
		{/*
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
			}*/
			//==================================================================
            if (arg == "--mse")
            {
                cout << "MSE\t"     << img.mse(ref)     << endl;
                continue;
            }
            else if (arg == "--pmse")
            {
                cout << "PMSE\t"    << img.pmse(ref)    << endl;
                continue;
            }
            else if (arg == "--snr")
            {
                cout << "SNR\t"     << img.snr(ref)     << endl;
                continue;
            }
            else if (arg == "--psnr")
            {
                cout << "PSNR\t"    << img.psnr(ref)    << endl;
                continue;
            }
            else if (arg == "--md")
            {
                cout << "MD\t"      << img.md(ref)      << endl;
                continue;
            }
		}
		else
		{
			cout <<"i:" << i << " arg:"<<arg <<" Command not found or value not specified" << endl;
			return 1;
		}
	}
    img.image.display("After",false,0,true);
}
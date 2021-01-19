#include <iostream>
#include <string>
#include "../include/Bitmap.h"
#include "../include/CImg.h"

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
        << "    --histogram" << endl
        << "    --hexponent [g_min] [g_max]" << endl
        << "    --casyco" << endl
        << "    --slaplace [n]" << endl
        << "        n - mask number" << endl
        << "    --osobel" << endl
        << "    --dilation [n]" << endl
        << "    --erosion [n]" << endl
        << "    --HMT [n]" << endl
        << "    --opening [n]" << endl
        << "    --closing [n]" << endl
        << "    --M3 [n]" << endl
        << "    --R1 [n] [filename|-select|-self]" << endl
		<< "	-r ref" << endl
		<< "		ref - filename of the original image" << endl
		<< "	Similarity measures:\n"
		<< "	--mse, --pmse, --snr, --psnr, --md, --all" << endl
		<< endl;
}
void wrong_value(const string& name)
{
	std::cerr << "Value of \"" << name << "\" not specified correctly. ";
}
int is_not_int(const string& arg, string&& val)
{
    //cout<< "log_try_catch: " << arg <<" " << val << endl;
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
    //cout<< "log_try_catch: " << arg <<" " << val << endl;
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
        //string arg = argv[1];
        Bitmap img(argv[1]);
        //img.image.display("Before",false,0,true);

	cimg_library::CImg<unsigned char> ref;
	bool ref_available = false;

	for (int i = 2; i < argc; ++i)
	{
		string arg = argv[i];
        //cout<<"log_argv[i]_loop: " <<arg<<" "<< i<<"\n";

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
                img.filter(atoi(argv[i + 1])/2, atoi(argv[i + 2]),Bitmap::alpha);
                i += 2;
                continue;
            }
            else if (arg == "--cmean")
            {
                //cout << endl << "cmean" << endl;
                if (is_not_int(arg, argv[i + 1])) return 1;
                if (is_not_int(arg, argv[i + 2])) return 1;
                img.filter(atoi(argv[i + 1]), atoi(argv[i + 2]),Bitmap::contra);
                i += 2;
                continue;
            }
			if (arg == "--hexponent")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				if (is_not_int(arg, argv[i + 2])) return 1;
				img.hexponent(stoi(argv[i + 1]), stoi(argv[i + 2]));
				i += 2;
				continue;
			}
			if (arg == "--pmf")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				if (is_not_int(arg, argv[i + 2])) return 1;
				img.PMF(stoi(argv[i + 1]), stoi(argv[i + 2]));
				i += 2;
				continue;
			}
			if (arg == "--bpf")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				if (is_not_int(arg, argv[i + 2])) return 1;
				img.BPF(stoi(argv[i + 1]), stoi(argv[i + 2]));
				i += 2;
				continue;
			}
			if (arg == "--bcf")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				if (is_not_int(arg, argv[i + 2])) return 1;
				img.BCF(stoi(argv[i + 1]), stoi(argv[i + 2]));
				i += 2;
				continue;
			}
			if (arg == "--R1")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				if (is_not_int(arg, argv[i + 2])) return 1;
				if (strcmp(argv[i + 3],"-select") == 0)
				{
					cimg_library::CImg<unsigned char> seeds = img.select_seeds();
					seeds.display("seeds",false,0,true);
					img.R1(stoi(argv[i + 1]), seeds, stoi(argv[i + 2]));
				}
				else if (strcmp(argv[i + 3],"-self") == 0)
				{
					img.image.display("seeds",false,0,true);
					img.R1(stoi(argv[i + 1]), img.image, stoi(argv[i + 2]));
				}
				else
				{
					cimg_library::CImg<unsigned char> seeds(argv[i + 3]);
					seeds.display("seeds",false,0,true);
					img.R1(stoi(argv[i + 1]), seeds, stoi(argv[i + 2]));
				}
				i += 3;
				continue;
			}
			if (arg == "--R1grad")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				if (is_not_int(arg, argv[i + 2])) return 1;
				if (*argv[i + 3] != 'l' && *argv[i + 3] != 's')
				{
					cout << "type:"<< argv[i + 3] <<" does not exist"<<endl;
					return 1;
				}
				if (strcmp(argv[i + 4],"-select") == 0)
				{
					cimg_library::CImg<unsigned char> seeds = img.select_seeds();
					seeds.display("seeds",false,0,true);
					img.R1grad(stoi(argv[i + 1]), seeds, stoi(argv[i + 2]), *argv[i + 3]);
				}
				else if (strcmp(argv[i + 4],"-self") == 0)
				{
					img.image.display("seeds",false,0,true);
					img.R1grad(stoi(argv[i + 1]), img.image, stoi(argv[i + 2]), *argv[i + 3]);
				}
				else
				{
					cimg_library::CImg<unsigned char> seeds(argv[i + 4]);
					seeds.display("seeds",false,0,true);
					img.R1grad(stoi(argv[i + 1]), seeds, stoi(argv[i + 2]), *argv[i + 3]);
				}
				i += 4;
				continue;
			}

        }
		if (i + 1 < argc)      // must have 1 argument
		{
			if (arg == "-r")
			{
				ref.assign(argv[++i]);
				Bitmap::optimize(ref);
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
				img.shrink(atoi(argv[++i]));
				continue;
			}
			else if (arg == "--enlarge")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.enlarge(atoi(argv[++i]));
				continue;
			}
            else if (arg == "--slaplace")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				int mask_nr = stoi(argv[i + 1]);
				if (mask_nr < 0 || mask_nr > 2)		//	exit when mask is not known
				{
					wrong_value(arg);
					return 1;
				}
				img.operation_3x3(mask_nr,'l');
				i++;
				continue;
			}
			if (arg == "--dilation")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.operation_3x3(stoi(argv[++i]),'d');
				continue;
			}
			if (arg == "--erosion")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.operation_3x3(stoi(argv[++i]),'e');
				continue;
			}
			if (arg == "--HMT")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.operation_3x3(stoi(argv[++i]),'h');
				continue;
			}
			if (arg == "--opening")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.opening(stoi(argv[++i]));
				continue;
			}
			if (arg == "--closing")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.closing(stoi(argv[++i]));
				continue;
			}
			if (arg == "--M3")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.M3(stoi(argv[++i]));
				continue;
			}
			if (arg == "--lpf")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.LPF(stoi(argv[++i]));
				continue;
			}
			if (arg == "--hpf")
			{
				if (is_not_int(arg, argv[i + 1])) return 1;
				img.HPF(stoi(argv[++i]));
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
		if (arg == "--negative")
		{
			img.negative();
			continue;
		}
		if (arg == "--hflip")
		{
			img.hflip();
			continue;
		}
		if (arg == "--vflip")
		{
			img.vflip();
			continue;
		}
		if (arg == "--dflip")
		{
			img.dflip();
			continue;
		}
        if (arg == "--osobel")
        {
            img.filter(0,1,Bitmap::osobel);
            continue;
        }
		if (arg == "--histogram")
		{
			img.histogram();
			continue;
		}
        else if (arg == "--casyco")
        {
            cout << "Asymmetry\t" << img.casyco() << endl;
            continue;
        }
		else if (arg == "--dft")
		{
			img.DFT_2D();
			continue;
		}
		else if (arg == "--idft")
		{
			img.IDFT_2D();
			continue;
		}
		else if (arg == "--print-abs")
		{
			img.print_abs();
			continue;
		}
		else if (arg == "--print-arg")
		{
			img.print_arg();
			continue;
		}
		else if (arg == "--print-real")
		{
			img.print_real();
			continue;
		}
		else if (arg == "--print-imag")
		{
			img.print_imag();
			continue;
		}
		if (ref_available)
		{
			if (arg == "--all")
			{
				cout << img.mse(ref) << "\t"
					<< img.pmse(ref) << "\t"
					<< img.snr(ref) << "\t"
					<< img.psnr(ref) << "\t"
					<< img.md(ref) << "\t";
				continue;
			}
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
        cout << arg <<": Command not found or value not specified." << endl;
        return 1;
	}
    img.image.display("After",false,0,true);
}
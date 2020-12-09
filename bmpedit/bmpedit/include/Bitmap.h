#pragma once
#include <string>
#include <algorithm>
#include <cmath>
#include "CImg.h"

typedef unsigned char byte;
typedef byte(*func)(byte* tab, int size, int parameter);

//	Simple structural elements for morphological operations
constexpr bool SE[11][9]={
		{0,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0},
		{0,0,0,0,1,0,0,1,0},
		{1,1,1,1,1,1,1,1,1},
		{0,1,0,1,1,1,0,1,0},
		{0,0,0,0,1,1,0,1,0},
		{0,0,0,0,0,1,0,1,0},
		{0,0,0,1,1,1,0,0,0},
		{0,0,0,1,0,1,0,0,0},
		{0,0,0,1,1,0,1,0,0},
		{0,1,1,0,1,0,0,0,0}
};




class Bitmap 
{
public:
	cimg_library::CImg<byte> image;
	//make grayscale if possible
	static void optimize(cimg_library::CImg<byte>& image);
private:
	int H, W;
	long offset;
	std::string ifname;
	uint32_t h[256];



	//int mask[3][9];
	void set_new_image(cimg_library::CImg<byte>& tmp);
	void make_arr(byte* p, int& win_s, byte* tab) const;
    void copy_frame(cimg_library::CImg<byte>& tmp, int win_s);
public:
	static byte alpha(byte* tab, int size, int d);
	static byte contra(byte* tab, int size, int Q);

	Bitmap(std::string&& filename);
	~Bitmap();
	//modify brightness, contrast and RGB components
	void brightness(int val);
	void contrast(float a);
	void negative();
	//simple geometric operations.
	void hflip();
	void vflip();
	void dflip();
	//shrinking and enlargement
	void shrink(int k);			
	void enlarge(int k);

	//filters
	//Alpha - trimmed mean filter
		//win_s - window size - distance from a center pixel
		//d - number of discarded pixels
	void filter(int d, int win_s, func method); //pass d/2 for the alpha filter

	//	Similarity measures
    double mse(cimg_library::CImg<byte>& ref);
	double pmse(cimg_library::CImg<byte>& ref);
	double snr(cimg_library::CImg<byte>& ref);
	double psnr(cimg_library::CImg<byte>& ref);
	double md(cimg_library::CImg<byte>& ref);
	//  Task2
	void make_hist(int&& color = 0);
    void hexponent(int gmin, int gmax);
    void histogram();
    static byte osobel (byte* tab, int size, int d);
    double mean ();
    double casyco ();
    void slaplace(int* kernel);
	static byte mask0(const byte* i, int* kernel, int W);
    static byte mask9(const byte* i, int* kernel, int W);
    //	task 3
	void basic_morph_operation(unsigned SE_number, char type);
	static byte dilation(const byte* i, const bool se[], int W);
	static byte erosion(const byte* i, const bool se[], int W);

	void opening(unsigned SE_number);
	void closing(unsigned SE_number);



    void save(const std::string& ofname) const;	//method overloaded, save(const char* const)
};


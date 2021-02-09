//
// Created by daniel on 15.01.2021.
//

#include "include/Task4.h"


Task4::Task4(cimg_library::CImg<unsigned char>* img)
		:img(img), HEIGHT(0), WIDTH(0), img_transformed(WIDTH, std::vector<std::complex<double>>(HEIGHT))
{
}

void Task4::constructor() //	Width and height need to be assigned after Bitmap::image
{
	WIDTH = img->width();
	HEIGHT = img->height();
}

template<typename T>
void Task4::DFT(const T* input_row, int output_column_nr,
		std::vector<std::vector<std::complex<double>>>& output_matrix)
{
	for (int k = 0; k < WIDTH; k++)
	{
		std::complex<double> transformed_pixel;
		auto element = input_row;
		for (int n = 0; n < WIDTH; n++)
		{
			transformed_pixel += (std::complex<double>)*element * coefficients[k*n];
			element++;
		}
		output_matrix[k][output_column_nr] = transformed_pixel;
	}
}

void Task4::DFT_2D()
{
	img_transformed = std::vector(HEIGHT, std::vector<std::complex<double>>(WIDTH));
	set_DFT_coefficients(false);
	std::vector<std::vector<std::complex<double>>> matrix_tmp(WIDTH, std::vector<std::complex<double>>(HEIGHT));

	for (unsigned y = 0; y < HEIGHT; y++)
		DFT(img->data(0, y), y, matrix_tmp);

	std::swap_ranges(matrix_tmp.begin(), matrix_tmp.begin() + HEIGHT / 2, matrix_tmp.begin() + HEIGHT / 2);

	for (unsigned y = 0; y < HEIGHT; y++)
		DFT(&matrix_tmp[y][0], y, this->img_transformed);

	std::swap_ranges(img_transformed.begin(), img_transformed.begin() + HEIGHT / 2,
			img_transformed.begin() + HEIGHT / 2);

	coefficients = std::vector<std::complex<double>>(0);
}


void Task4::IDFT_2D()
{
	std::vector<std::vector<std::complex<double>>> matrix_tmp(WIDTH, std::vector<std::complex<double>>(HEIGHT));
	set_DFT_coefficients(true);

	for (unsigned y = 0; y < HEIGHT; y++)
		DFT(&img_transformed[y][0], y, matrix_tmp);
	//std::swap_ranges(matrix_tmp.begin(),matrix_tmp.begin()+HEIGHT/2,matrix_tmp.begin()+HEIGHT/2);

	for (unsigned y = 0; y < HEIGHT; y++)
		DFT(&matrix_tmp[y][0], y, this->img_transformed);
	//std::swap_ranges(img_transformed.begin(),img_transformed.begin()+HEIGHT/2,img_transformed.begin()+HEIGHT/2);

	auto* pixel = img->begin();
	for (const auto& row: img_transformed)
		for (const auto& value: row)
		{
			*pixel = lround(abs(value / double(HEIGHT * WIDTH)));
			pixel++;
		}

	coefficients = std::vector<std::complex<double>>(0);
}

template<typename T>
void Task4::FFT(const T* input_row, int stages, int output_column_nr,
		std::vector<std::vector<std::complex<double>>>& output_matrix)
{
	//	first iteration, 1-element lists, indexes need to be bit-reversed
	for (int i = 0; i < HEIGHT-1; i+=2)
	{	// get element row[decimated index]
		auto x1 = (std::complex<double>)*(input_row+reverse_bits(i,stages));
		auto x2 = (std::complex<double>)*(input_row+reverse_bits(i+1,stages));
		 output_matrix[i][output_column_nr] = x1 + x2;
		 output_matrix[i+1][output_column_nr] = x1 - x2;
	}

	unsigned hop = 1<<(stages-2); //set to the correct power of two. At the last stage should be equal 1.
	unsigned list_size = 2; //2,4,8,16... - length of odd or even list, half of next list length

	for (int stage = 1; stage < stages ; ++stage)
	{
		for (unsigned i = 0; i < WIDTH-list_size ; i+=list_size) //0..N-list_size/2 - changing sets of input lists
		{
			for (unsigned k=0, index=0 ; index < list_size; ++index) // iterating on even-indexed list, k - local index in even list
			{
				auto x_1 = output_matrix[i][output_column_nr];
				output_matrix[i][output_column_nr] +=
						output_matrix[i+list_size][output_column_nr] * coefficients[k];
				output_matrix[i+list_size][output_column_nr] =
						x_1 - output_matrix[i+list_size][output_column_nr] * coefficients[k];
				k+=hop;
				i++;
			}
		}
		hop>>=1;
		list_size<<=1;
	}
}

void Task4::FFT_2D()
{
	img_transformed = std::vector(HEIGHT, std::vector<std::complex<double>>(WIDTH));
	int steps = std::log2(WIDTH);
	if((unsigned )1<<steps!=WIDTH) return;

	set_FFT_coefficients(false, WIDTH);
	std::vector<std::vector<std::complex<double>>> matrix_tmp(WIDTH, std::vector<std::complex<double>>(HEIGHT));

	for (unsigned y = 0; y < HEIGHT; y++)
		FFT(img->data(0, y), steps, y, matrix_tmp);
	std::swap_ranges(matrix_tmp.begin(), matrix_tmp.begin() + HEIGHT / 2, matrix_tmp.begin() + HEIGHT / 2);

	if(HEIGHT!=WIDTH)	set_FFT_coefficients(false, HEIGHT);

	for (unsigned y = 0; y < WIDTH; y++)
		FFT(&matrix_tmp[y][0], steps,y, this->img_transformed);
	std::swap_ranges(img_transformed.begin(), img_transformed.begin() + HEIGHT / 2, img_transformed.begin() + HEIGHT / 2);
}

void Task4::IFFT_2D()
{
	int steps = std::log2(WIDTH);

	set_FFT_coefficients(true, WIDTH);
	std::vector<std::vector<std::complex<double>>> matrix_tmp(WIDTH, std::vector<std::complex<double>>(HEIGHT));

	for (unsigned y = 0; y < HEIGHT; y++)
		FFT(&img_transformed[y][0], steps, y, matrix_tmp);

	if (HEIGHT != WIDTH) set_FFT_coefficients(false, HEIGHT);

	for (unsigned y = 0; y < WIDTH; y++)
		FFT(&matrix_tmp[y][0], steps, y, this->img_transformed);

	auto* pixel = img->begin();
	for (const auto& row: img_transformed)
		for (auto value: row)
		{
			int val = lround(abs(value / double(HEIGHT * WIDTH)));
			if (val<256)
				*pixel = val;
			else
				*pixel = 255;
			pixel++;
		}
	coefficients = std::vector<std::complex<double>>(0);
}

void Task4::set_DFT_coefficients(bool inverse)
{
	double tau = inverse ? 2 * M_PI: -2 * M_PI;
	unsigned size = std::max(HEIGHT,WIDTH);
	coefficients = std::vector<std::complex<double>>(size*size);
	for (unsigned k = 0; k < size; k++)
		for (unsigned n = 0; n < size; n++)
			coefficients[k*n] = std::polar(1.0,tau * k * n / WIDTH);
}

void Task4::set_FFT_coefficients(bool inverse, unsigned size)
{
	std::complex<double> W;
	if(inverse)
		W = std::polar(1.0, 2 * M_PI / size);
	else
		W = std::polar(1.0, -2 * M_PI / size);

	coefficients = std::vector<std::complex<double>>(size/2);

	coefficients[0]=1;
	for (unsigned n = 1; n < size/2; n++)
		coefficients[n] = coefficients[n-1]*W;
}

uint32_t Task4::reverse_bits(uint32_t n, size_t size)
{
	uint32_t x = 0;
	for (; size > 0; size--)
	{
		x <<= 1;
		x |= n & 1;
		n >>= 1;
	}
	return x;
}


void Task4::print_abs(bool save)
{
	cimg_library::CImg<double> new_img(HEIGHT, WIDTH);
	auto pixel = new_img.begin();

	for (const auto& row : img_transformed)
		for (const auto& value : row)
			*pixel++ = log10(abs(value));

	if (save)
	{
		double scale = 256/new_img(WIDTH/2,HEIGHT/2);
		for (auto& pixel : new_img)
			pixel *= scale;
		new_img.save("DFT_abs.bmp");
	}
	else
		new_img.display("DFT. Magnitude spectrum", false, nullptr, true);
}

void Task4::print_arg(bool save)
{
	cimg_library::CImg<double> new_img(HEIGHT, WIDTH);
	auto pixel = new_img.begin();

	for (const auto& row : img_transformed)
		for (const auto& value : row)
			*pixel++ = arg(value);

	if (save)
		new_img.save("DFT_arg.bmp");
	else
		new_img.display("DFT. Phase spectrum", false, nullptr, true);
}

void Task4::print_real(bool save)
{
	cimg_library::CImg<double> new_img(HEIGHT, WIDTH);
	auto pixel = new_img.begin();
	for (const auto& row : img_transformed)
		for (const auto& value : row)
		{
			if(value.real()>0)
				*pixel++ = log10(real(value));
			else
				*pixel++ = -log10(real(-value));
		}
	if (save)
		new_img.save("DFT_real.bmp");
	else
		new_img.display("DFT. Real spectrum", false, nullptr, true);
}

void Task4::print_imag(bool save)
{
	cimg_library::CImg<double> new_img(HEIGHT, WIDTH);
	auto pixel = new_img.begin();
	for (const auto& row : img_transformed)
		for (const auto& value : row)
			*pixel++ = imag(value);
	if (save)
		new_img.save("DFT_imag.bmp");
	else
		new_img.display("DFT. Imaginary spectrum", false, nullptr, true);
}

template<typename T>
void Task4::apply_mask(T* mask_value)
{
	for (auto& row : img_transformed)
		for (auto& value : row)
			if (*mask_value++ == 0)
			{
				value.real(0.0);
				value.imag(0.0);
			}

}

void Task4::LPF(int size)
{
	cimg_library::CImg<double> mask(WIDTH, HEIGHT, 1, 1, 0);

	const double color[] = { 1 };
	mask.draw_circle(HEIGHT / 2, WIDTH / 2, size, color);
	apply_mask(mask.begin());
}

void Task4::HPF(int size)
{
	cimg_library::CImg<double> mask(WIDTH, HEIGHT, 1, 1, 1);

	const double color[] = { 0 };
	mask.draw_circle(HEIGHT / 2, WIDTH / 2, size, color);
	mask(HEIGHT / 2, WIDTH / 2) = 1;
	apply_mask(mask.begin());
}

void Task4::PMF(int k, int l)
{
	double coefficient_k = -2 * M_PI * k / WIDTH;
	double coefficient_l = -2 * M_PI * l / HEIGHT;
	double k_l_pi = (k + l) * M_PI;
	for (int m = 0; m < HEIGHT; ++m)
		for (int n = 0; n < WIDTH; ++n)
			img_transformed[m][n] *= std::polar(1.0, coefficient_k * n + coefficient_l * m + k_l_pi);
}

void Task4::BPF(int min, int max)
{
	cimg_library::CImg<double> mask(WIDTH, HEIGHT, 1, 1, 0);

	double color[] = { 1 };
	mask.draw_circle(HEIGHT / 2, WIDTH / 2, max, color);
	color[0] = { 0 };
	mask.draw_circle(HEIGHT / 2, WIDTH / 2, min, color);
	mask(HEIGHT / 2, WIDTH / 2) = 1;

	apply_mask(mask.begin());
}

void Task4::BCF(int min, int max)
{
	cimg_library::CImg<double> mask(WIDTH, HEIGHT, 1, 1, 1);

	double color[] = { 0 };
	mask.draw_circle(HEIGHT / 2, WIDTH / 2, max, color);
	color[0] = { 1 };
	mask.draw_circle(HEIGHT / 2, WIDTH / 2, min, color);

	apply_mask(mask.begin());
}

void Task4::EDF(double angle, double approx, int size)
{
	cimg_library::CImg<double> mask(WIDTH, HEIGHT, 1, 1, 0);

	if (approx>45) approx=45;

	double alpha = 2*M_PI * angle/360;
	approx = 2*M_PI * approx/360;
	double r = 1.43*HEIGHT;


	int x = lround(std::cos(alpha-approx)*r);
	int y = lround(std::sin(alpha-approx)*r);
	int x1 = lround(std::cos(alpha+approx)*r);
	int y1 = lround(std::sin(alpha+approx)*r);


	double color[] = { 1 };

	mask.draw_triangle(x1+WIDTH/2,y1+HEIGHT/2,x+WIDTH/2,y+HEIGHT/2,WIDTH/2,HEIGHT/2,color);
	mask.draw_triangle(-x1+WIDTH/2,-y1+HEIGHT/2,-x+WIDTH/2,-y+HEIGHT/2,WIDTH/2,HEIGHT/2,color);
	color[0] = { 0 };
	mask.draw_circle(HEIGHT / 2, WIDTH / 2, size, color);

	mask(HEIGHT/2,WIDTH/2)=1;

	apply_mask(mask.begin());
}

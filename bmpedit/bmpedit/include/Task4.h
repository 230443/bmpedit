//
// Created by daniel on 15.01.2021.
//

#ifndef BMPEDIT_TASK4_H
#define BMPEDIT_TASK4_H

#include "Bitmap.h"
#include "complex"
//#include <cmath>

const double PI = 3.141592653589793238463;
class Task4 : public Bitmap
{
private:
	cimg_library::CImg<std::complex<double>> img_transformed;
	void DFT_1D();
	void transform_row(int row_number);
	void DFT();


public:
	Task4(std::string&& filename);


};


#endif //BMPEDIT_TASK4_H

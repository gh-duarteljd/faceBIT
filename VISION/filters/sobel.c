#include "filters.h"

image
sobel_x_kernel()
{
	image sobel_x;

	float** Sx = malloc(3 * sizeof(float*));
	for (int i = 0; i < 3; i++)
	{
		Sx[i] = malloc( 3 * sizeof(float));
	}

	Sx[0][0] =  1;
	Sx[0][1] =  0;
	Sx[0][2] = -1;
	Sx[1][0] =  2;
	Sx[1][1] =  0;
	Sx[1][2] = -2;
	Sx[2][0] =  1;
	Sx[2][1] =  0;
	Sx[2][2] = -1;

	sobel_x.pixels_map = Sx;
	sobel_x.width = 3;
	sobel_x.height = 3;

	return sobel_x;
}

image
sobel_y_kernel()
{
	image sobel_y;

	float** Sy = malloc(3 * sizeof(float*));
	for (int i = 0; i < 3; i++)
	{
		Sy[i] = malloc( 3 * sizeof(float));
	}

	Sy[0][0] =  1;
	Sy[0][1] =  2;
	Sy[0][2] =  1;
	Sy[1][0] =  0;
	Sy[1][1] =  0;
	Sy[1][2] =  0;
	Sy[2][0] = -1;
	Sy[2][1] = -2;
	Sy[2][2] = -1;

	sobel_y.pixels_map = Sy;
	sobel_y.width = 3;
	sobel_y.height = 3;

	return sobel_y;
}

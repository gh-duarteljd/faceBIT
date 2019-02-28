#include "filters.h"

image
dx_kernel()
{
	image dx;

	float** DX = malloc(3 * sizeof(float*));
	for (int i = 0; i < 3; i++)
	{
		DX[i] = malloc(3 * sizeof(float));
	}

	DX[0][0] =  0;
	DX[0][1] =  0;
	DX[0][2] =  0;
	DX[1][0] =  1;
	DX[1][1] =  0;
	DX[1][2] = -1;
	DX[2][0] =  0;
	DX[2][1] =  0;
	DX[2][2] =  0;

	dx.pixels_map = DX;
	dx.width = 3;
	dx.height = 3;

	return dx;
}

image
dy_kernel()
{
	image dy;

	float** DY = malloc(3 * sizeof(float*));
	for (int i = 0; i < 3; i++)
	{
		DY[i] = malloc( 3 * sizeof(float));
	}

	DY[0][0] =  0;
	DY[0][1] =  1;
	DY[0][2] =  0;
	DY[1][0] =  0;
	DY[1][1] =  0;
	DY[1][2] =  0;
	DY[2][0] =  0;
	DY[2][1] = -1;
	DY[2][2] =  0;

	dy.pixels_map = DY;
	dy.width = 3;
	dy.height = 3;

	return dy;
}

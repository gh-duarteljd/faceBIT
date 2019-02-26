#include "processing.h"

void
grad2d(image 	input_image,
			 image* mag,
			 image* angle)
{
	image dx = dx_kernel();
	image dy = dy_kernel();
	image Ix = conv2d(input_image, dx);
	image Iy = conv2d(input_image, dy);

	float** M = (float**)malloc(input_image.height * sizeof(float*));
	float** A = (float**)malloc(input_image.height * sizeof(float*));
	for (int i = 0; i < input_image.height; i++)
	{
		M[i] = (float*) malloc( input_image.width * sizeof(float));
		A[i] = (float*) malloc( input_image.width * sizeof(float));

		for (int j = 0; j < input_image.width ; j++)
		{
			M[i][j]= sqrt(Ix.pixels_map[i][j]*Ix.pixels_map[i][j] + Iy.pixels_map[i][j]*Iy.pixels_map[i][j]);
			A[i][j] = (atan2(Iy.pixels_map[i][j], Ix.pixels_map[i][j])) * 180.0 / 3.14159265359;
		}
	}
	mag->pixels_map = M;
	mag->width = input_image.width;
	mag->height = input_image.height;

	angle->pixels_map = A;
	angle->width = input_image.width;
	angle->height = input_image.height;

	for (int i = 0; i < Ix.height; i++)
	{
		free(Ix.pixels_map[i]);
		free(Iy.pixels_map[i]);
	}
	free(Ix.pixels_map);
	free(Iy.pixels_map);

	for (int i = 0; i < dx.height; i++)
	{
		free(dx.pixels_map[i]);
		free(dy.pixels_map[i]);
	}
	free(dx.pixels_map);
	free(dy.pixels_map);

}

image mag2d(image input_image)
{
	image dx = dx_kernel();
	image dy = dy_kernel();
	image Ix = conv2d(input_image,  dx);
	image Iy = conv2d(input_image,  dy);

	image mag;
	float** M = (float**)malloc(input_image.height * sizeof(float*));
	for (int i = 0; i < input_image.height; i++)
	{
		M[i] = (float*) malloc( input_image.width * sizeof(float));

		for (int j = 0; j < input_image.width ; j++)
		{
			M[i][j]= sqrt(Ix.pixels_map[i][j]*Ix.pixels_map[i][j] + Iy.pixels_map[i][j]*Iy.pixels_map[i][j]);
		}
	}
	mag.pixels_map = M;
	mag.width = input_image.width;
	mag.height = input_image.height;

	for (int i = 0; i < Ix.height; i++)
	{
		free(Ix.pixels_map[i]);
		free(Iy.pixels_map[i]);
	}
	free(Ix.pixels_map);
	free(Iy.pixels_map);

	for (int i = 0; i < dx.height; i++)
	{
		free(dx.pixels_map[i]);
		free(dy.pixels_map[i]);
	}
	free(dx.pixels_map);
	free(dy.pixels_map);

	return mag;
}

image phase2d(image input_image)
{
	image dx = dx_kernel();
	image dy = dy_kernel();
	image Ix = conv2d(input_image,  dx_kernel());
	image Iy = conv2d(input_image,  dy_kernel());

	image angle;
	float** A = (float**)malloc(input_image.height * sizeof(float*));
	for (int i = 0; i < input_image.height; i++)
	{
		A[i] = (float*) malloc( input_image.width * sizeof(float));

		for (int j = 0; j < input_image.width ; j++)
		{
			A[i][j] = (atan2(Iy.pixels_map[i][j], Ix.pixels_map[i][j])) * 180.0 / 3.14159265359;
		}
	}
	angle.pixels_map = A;
	angle.width = input_image.width;
	angle.height = input_image.height;

	for (int i = 0; i < Ix.height; i++)
	{
		free(Ix.pixels_map[i]);
		free(Iy.pixels_map[i]);
	}
	free(Ix.pixels_map);
	free(Iy.pixels_map);

	for (int i = 0; i < dx.height; i++)
	{
		free(dx.pixels_map[i]);
		free(dy.pixels_map[i]);
	}
	free(dx.pixels_map);
	free(dy.pixels_map);

	return angle;
}

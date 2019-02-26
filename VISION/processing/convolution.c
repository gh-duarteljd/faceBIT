#include "processing.h"

image
conv2d(image input_image, image kernel)
{
	image output_image;

	int Iw = input_image.width;
	int Ih = input_image.height;

	int Kw = kernel.width;
	int Kh = kernel.height;

	int Ow = input_image.width;
	int Oh = input_image.height;

	float** I = input_image.pixels_map;
	float** K = kernel.pixels_map;
	float** O;

	O = (float**)malloc(Oh * sizeof(float*));
	for (int i = 0; i < Oh; i++)
	{
		O[i] = (float*) calloc( Ow * sizeof(float), 1);
	}

	for (int i = 0; i < Oh; i++)
	{
		for (int j = 0; j < Ow; j++)
		{
			for (int k = (i - Kh / 2), p = 0; k < (i + Kh / 2 + 1)  ; k++, p++)
			{
				for (int l =  (j - Kw / 2), q = 0; l < (j + Kw / 2 + 1) ; l++, q++)
				{
					if ((k < 0) || (l < 0) || (k > (Ih - 1)) || l > (Iw - 1))
					{
						O[i][j] = 0;
					}
					else
					{
						O[i][j] = O[i][j] + (float)I[k][l] * K[p][q];
					}
				}
			}
		}
	}

	output_image.pixels_map = O;
	output_image.width = Ow;
	output_image.height = Oh;

	return output_image;
}

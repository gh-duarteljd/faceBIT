#include "tools.h"

image
histogram_equalizer(image input_image)
{

  float max = 0, min = 0;

  for (int i = 0; i < input_image.height; i++)
  {
    for (int j = 0; j < input_image.width; j++)
    {
      if (max < input_image.pixels_map[i][j])
      {
        max = input_image.pixels_map[i][j];
      }
      if (min > input_image.pixels_map[i][j])
      {
        min = input_image.pixels_map[i][j];
      }
    }
  }

  float a = 255.0/(max - min);
  float b = -a * min;

	float** B = (float**)malloc(input_image.height * sizeof(float*));
	for (int i = 0; i < input_image.height; i++)
	{
    B[i] = (float*) malloc(input_image.width * sizeof(float));

		for (int j = 0; j < input_image.width ; j++)
		{
			B[i][j]= a * input_image.pixels_map[i][j] + b;
	  }
  }

  image buffer;
  buffer.width = input_image.width;
	buffer.height = input_image.height;
  buffer.pixels_map = B;

  return buffer;
}

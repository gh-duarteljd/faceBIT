#include "tools.h"

image_pyramid
pyramid(image input_image, float step_scale)
{

	if (input_image.width < 92 || input_image.height < 112)
  {
		fprintf(stderr, "The image is too small to create the pyramid.\n");
    exit(1);
 	}

	image_pyramid buffer;

	buffer.pyramid_height = 1;

	while (((input_image.width / pow(step_scale, buffer.pyramid_height)) > 92) && ((input_image.height / pow(step_scale, buffer.pyramid_height)) > 112))
	{
		buffer.pyramid_height++;
	}

	buffer.layer = (image*)malloc(buffer.pyramid_height * sizeof(image));

	for (int  i = 0; i < buffer.pyramid_height; i++)
	{
		buffer.layer[i] = down_scale(input_image, pow(step_scale, i));
	}

	return 	buffer;
}

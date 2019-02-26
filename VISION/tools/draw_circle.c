#include "tools.h"

void
draw_circle(image input_image,
						int 	x,
						int 	y,
						int 	r,
						int 	pixels_width,
						int 	color)
{
	x = x - r;
	y = y - r;

	if (((x + r) > input_image.width) || ((x - r) < 0) || ((y + r) > input_image.height) || ((y - r) < 0))
	{
		fprintf(stderr, "Could not draw circle.\n");
    exit(2);
	}

	for (int i = y; i <= (2 * r) + y; i++)
  {
  	for (int j = x; j <= (2 * r) + x; j++)
    {
    	float d = sqrt((float)(i - (r + y))*(i - (r + y)) + (j - (r + x))*(j - (r + x)));
      if (d > r - ceil(pixels_width / 2.0) && d < r + ceil(pixels_width / 2.0))
      {
      	input_image.pixels_map[i][j] = color;
      }
		}
	}
}

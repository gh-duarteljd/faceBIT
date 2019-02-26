#ifndef _INTERFACE_
#define _INTERFACE_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
  int		   width;
  int		   height;
  float** pixels_map;
}
image;

image
load_image(char* file_path);

void
save_image(image input_image, char* file_path);

#endif // _INTERFACE_

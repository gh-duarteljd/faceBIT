#ifndef _PROCESSING_
#define _PROCESSING_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../filters/filters.h"
#include "../interface/interface.h"

image
conv2d(image input_image, image kernel);

void
grad2d(image  input_image,
       image* mag,
       image* angle);

image
mag2d(image input_image);

image
phase2d(image input_image);

void
free_image(image input_image);

#endif // _PROCESSING_

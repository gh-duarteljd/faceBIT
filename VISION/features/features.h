#ifndef _FEATURES_
#define _FEATURES_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../processing/processing.h"
#include "../interface/interface.h"
#include "../filters/filters.h"

typedef struct
{
  int     size;
  float*	hog_vector;
}
hog_features;

hog_features
hog(image input_image,
    int   cell_width_in_pixels,
    int   block_width_in_pixels,
    int   number_of_angle_bins);

float*
histogram(float** cell_mag,
          float** cell_pha,
          int 		 cell_width_in_pixels,
          int 		 NUMBER_OF_ANGLE_BINS);

#endif // _FEATURES_

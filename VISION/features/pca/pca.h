#ifndef _PCA_
#define _PCA_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../processing/processing.h"
#include "../../interface/interface.h"
#include "../../filters/filters.h"
#include "../../pca/pca.h"

typedef struct
{
  int     size;
  float*	pca_vector;
}
pca_features;

pca_features
pca(image    input_image,
    float** p,
    float** u,
    int 	   WIDTH_OF_WINDOW,
    int 	   HEIGHT_OF_WINDOW,
    int      NUMBER_OF_COMPONENTS);

#endif // _PCA_

#ifndef _FILTERS_
#define _FILTERS_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../interface/interface.h"

image sobel_x_kernel();

image sobel_y_kernel();

image dx_kernel();

image dy_kernel();

image g_kernel();

#endif // _FILTERS_

#ifndef _TOOLS_
#define _TOOLS_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../features/hog/hog.h"
#include "../interface/interface.h"

#define FACE 	 	1
#define NOFACE 	0

typedef struct
{
	image*	layer;
	int			pyramid_height;
}
image_pyramid;

typedef struct
{
	int 		x;
	int 		y;
	int 		w;
	int 		h;
	float	p;
}
face;

image
down_scale(image input_image, float scale);

void
draw_rect(image input_image,
					int		x,
					int 	y,
					int   w,
					int   h,
					int   pixels_width,
					int   color);

void
draw_circle(image input_image,
						int		x,
						int		y,
						int		r,
						int		pixels_width,
						int   color);

face*
face_detector(image  input_image,
		    			float* W,
		    			float  B,
		    			int 	 WIDTH_OF_WINDOW,
		    			int 	 HEIGHT_OF_WINDOW,
		    			int 	 PIXELS_PER_CELL,
		    			int 	 CELLS_PER_BLOCK,
		    			int 	 STEP_WINDOW,
		    			int 	 NUMBER_OF_ANGLE_BINS,
		    			int* 	 number_of_faces);

float
svm_calculation_probability(hog_features features,
														float* 			 W,
														float 			 B,
														int 				 NUMBER_OF_FEATURES);

face*
non_maximum_suppression(face* faces,
												int 	number_of_faces,
												int* 	final_number_faces);

image_pyramid
pyramid(image input_image, float step_scale);

image
histogram_equalizer(image input_image);

#endif // _TOOLS_

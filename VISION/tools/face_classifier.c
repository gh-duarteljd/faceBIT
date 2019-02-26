#include "tools.h"

char
hog_face_classifier(image  input_image,
		    						float* W,
		    						float  B,
		    						int 	 WIDTH_OF_WINDOW,
		    						int 	 HEIGHT_OF_WINDOW,
		    						int 	 PIXELS_PER_CELL,
		    						int 	 CELLS_PER_BLOCK,
		    						int 	 NUMBER_OF_ANGLE_BINS)
{

	int NUMBER_OF_FEATURES = ((HEIGHT_OF_WINDOW / PIXELS_PER_CELL) - (CELLS_PER_BLOCK -1)) * ((WIDTH_OF_WINDOW /  PIXELS_PER_CELL) - (CELLS_PER_BLOCK -1)) * CELLS_PER_BLOCK * CELLS_PER_BLOCK * NUMBER_OF_ANGLE_BINS;

	hog_features features = hog(input_image, PIXELS_PER_CELL, CELLS_PER_BLOCK, NUMBER_OF_ANGLE_BINS);

	float L = 0;
	for (int i = 0; i < NUMBER_OF_FEATURES; i++)
	{
		L = L + W[i] * features.hog_vector[i];
	}
	L = L + B;

	if (L > 0)
	{
		return FACE;
	}
	else
	{
		return NOFACE;
	}
}

/*



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
		    			int* 	 number_of_faces)
{
	int NUMBER_OF_FEATURES = ((HEIGHT_OF_WINDOW / PIXELS_PER_CELL) - (CELLS_PER_BLOCK -1)) * ((WIDTH_OF_WINDOW /  PIXELS_PER_CELL) - (CELLS_PER_BLOCK -1)) * CELLS_PER_BLOCK * CELLS_PER_BLOCK * NUMBER_OF_ANGLE_BINS;

	image image_buffer;
	image_buffer.width = WIDTH_OF_WINDOW;
	image_buffer.height = HEIGHT_OF_WINDOW;
	image_buffer.pixels_map = (double**)malloc(HEIGHT_OF_WINDOW * sizeof(double*));
	for (int i = 0; i < HEIGHT_OF_WINDOW; i++)
	{
		image_buffer.pixels_map[i] = (double*) malloc( WIDTH_OF_WINDOW * sizeof(double));
	}

	int window_number = 0;

	face* face_buffer = NULL;

	*number_of_faces = 0;

	for (int i = 0; i < (int)((input_image.height - HEIGHT_OF_WINDOW) / STEP_WINDOW + 1); i++)
	{
		 for (int j = 0; j < (int)((input_image.width - WIDTH_OF_WINDOW) / STEP_WINDOW + 1); j++)
		{
			for (int p = (i * STEP_WINDOW), a = 0; p < (HEIGHT_OF_WINDOW + i * STEP_WINDOW); p++, a++)
			{
				for (int q = (j * STEP_WINDOW), b = 0; q < (WIDTH_OF_WINDOW + j * STEP_WINDOW); q++, b++)
				{
					image_buffer.pixels_map[a][b] = input_image.pixels_map[p][q];
				}
			}

			hog_features hog_buffer = hog(image_buffer, PIXELS_PER_CELL, CELLS_PER_BLOCK, NUMBER_OF_ANGLE_BINS);

			double prob = svm_calculation_probability(hog_buffer, W, B, NUMBER_OF_FEATURES);

			if (prob > 80)
			{
				face_buffer = (face*)realloc(face_buffer, (int)(*number_of_faces + 1) * sizeof(face));

				face_buffer[*number_of_faces].p = prob;
				face_buffer[*number_of_faces].x = (int)(window_number % (int)((input_image.width - WIDTH_OF_WINDOW) / STEP_WINDOW + 1)) * STEP_WINDOW + WIDTH_OF_WINDOW / 2;
				face_buffer[*number_of_faces].y = (int)(window_number / (int)((input_image.width - WIDTH_OF_WINDOW) / STEP_WINDOW + 1)) * STEP_WINDOW + HEIGHT_OF_WINDOW / 2;
				face_buffer[*number_of_faces].w = WIDTH_OF_WINDOW;
				face_buffer[*number_of_faces].h = HEIGHT_OF_WINDOW;
				*number_of_faces = *number_of_faces + 1;
			}

			free(hog_buffer.hog_vector);

			window_number++;
		}
	}

	free_image(image_buffer);

	return face_buffer;
}

double
svm_calculation_probability(hog_features hog_buffer,
														float* 			 W,
														float 			 B,
														int 				 NUMBER_OF_FEATURES)
{
	float F = 0;
	for (int i = 0; i < NUMBER_OF_FEATURES; i++)
	{
		F = F + W[i] * hog_buffer.hog_vector[i];
	}
	F = 100 / (1 + exp(-10 *(F + B)));
	return F;
}
*/

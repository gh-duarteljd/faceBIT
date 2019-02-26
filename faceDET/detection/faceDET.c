#include "../../VISION/VISION.h"

#define WIDTH_OF_WINDOW  168

#define HEIGHT_OF_WINDOW 192

#define PIXELS_PER_CELL 8

#define CELLS_PER_BLOCK 4

#define NUMBER_OF_ANGLE_BINS 9

#define NUMBER_OF_FEATURES ((HEIGHT_OF_WINDOW /  PIXELS_PER_CELL) - (CELLS_PER_BLOCK - 1)) * ((WIDTH_OF_WINDOW /  PIXELS_PER_CELL) - (CELLS_PER_BLOCK - 1)) * CELLS_PER_BLOCK * CELLS_PER_BLOCK * NUMBER_OF_ANGLE_BINS

#define STEP_SCALE 1.3

#define NUMBER_OF_FACES 2019

#define NUMBER_OF_COMPONENTS 100

int main(int argc, char** argv)
{

	float B;
	FILE* ptrB = fopen("../training/SVM/B", "r");
	if (ptrB == NULL)
   	{
		fprintf(stderr, "Unable to open training data.\n");
      		exit(1);
   	}
	fread(&B, sizeof(float), 1, ptrB);

	FILE* ptrW = fopen("../training/SVM/W", "r");
	if (ptrW == NULL)
   	{
		fprintf(stderr, "Unable to open training data.\n");
      		exit(1);
   	}
	float* W = (float*)malloc(NUMBER_OF_FEATURES * sizeof(float));
	fread(W, sizeof(float), NUMBER_OF_FEATURES, ptrW);

	image input_image = load_image(argv[1]);
	image current_image = histogram_equalizer(input_image);

	char class = hog_face_classifier(current_image,
			    												 W,
			    										 		 B,
			    										 		 WIDTH_OF_WINDOW,
			    										 		 HEIGHT_OF_WINDOW,
			    										 		 PIXELS_PER_CELL,
			    										 		 CELLS_PER_BLOCK,
			    										 		 NUMBER_OF_ANGLE_BINS);

	free(W);
	fclose(ptrW);
	fclose(ptrB);

	free_image(input_image);
}

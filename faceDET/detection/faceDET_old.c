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
  // ensure proper usage
	if (argc != 3)
	{
			printf("Usage: ./faceDET input_image output_image \n");
			exit(1);
	}

	FILE* imgptr_P = fopen("../../VISION/pca/P", "r");
	if (imgptr_P == NULL)
	{
		exit(1);
	}

	FILE* imgptr_U = fopen("../../VISION/pca/U", "r");
	if (imgptr_U == NULL)
	{
		exit(1);
	}

	double** p;
	int l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
	int c = NUMBER_OF_FACES;
	p = malloc(l * sizeof(double*));
	for (int i = 0; i < l; i++)
	{
		p[i] = malloc(c * sizeof(double));
		for (int j = 0; j < c; j++)
		{
			fread(&p[i][j], sizeof(double), 1, imgptr_P);
		}
	}

	double** u;
	l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
	c = 1;
	u = malloc(l * sizeof(double*));
	for (int i = 0; i < l; i++)
	{
		u[i] = malloc(c * sizeof(double));
		for (int j = 0; j < c; j++)
		{
			fread(&u[i][j], sizeof(double), 1, imgptr_U);
		}
	}

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

	image_pyramid input_pyramid = pyramid(input_image, STEP_SCALE);

	face* faces[input_pyramid.pyramid_height];

	int number_of_faces[input_pyramid.pyramid_height];

	int step_window[input_pyramid.pyramid_height];
	for (int i = 0; i < input_pyramid.pyramid_height; i++)
	{
		step_window[i] = 10;
	}

	for (int i = 0; i < input_pyramid.pyramid_height; i++)
	{
		faces[i] = face_detector(input_pyramid.layer[i],
														 W,
														 B,
														 WIDTH_OF_WINDOW,
														 HEIGHT_OF_WINDOW,
													 	 PIXELS_PER_CELL,
														 CELLS_PER_BLOCK,
														 step_window[i],
														 NUMBER_OF_ANGLE_BINS,
														 &number_of_faces[i]);

		for(int  j = 0; j < number_of_faces[i]; j++)
		{

			char confirm = pca_face_detector(input_pyramid.layer[i],
			             										 faces[i][j],
									 									 	 p,
					    		 										 u,
			             										 WIDTH_OF_WINDOW,
			             										 HEIGHT_OF_WINDOW,
			             										 NUMBER_OF_COMPONENTS);
			if(confirm == 1)
			{
				faces[i][j].p = 0;
				faces[i][j].x = 0;
				faces[i][j].y = 0;
				faces[i][j].w = 0;
				faces[i][j].h = 0;
			}
		}

		for(int  j = 0; j < number_of_faces[i]; j++)
		{
			faces[i][j].x = faces[i][j].x * pow(STEP_SCALE, i);
			faces[i][j].y = faces[i][j].y * pow(STEP_SCALE, i);
			faces[i][j].w = faces[i][j].w * pow(STEP_SCALE, i);
			faces[i][j].h = faces[i][j].h * pow(STEP_SCALE, i);
		}

		free_image(input_pyramid.layer[i]);
	}

	int total_faces = 0;

	for (int i = 0; i < input_pyramid.pyramid_height; i++)
	{
		total_faces = total_faces + number_of_faces[i];
	}

	face face_vector[total_faces];

	int aux_number_of_faces = 0;

	for (int i = 0; i < input_pyramid.pyramid_height; i++)
	{
		for(int  j = 0; j < number_of_faces[i]; j++)
		{
			face_vector[aux_number_of_faces] = faces[i][j];
			aux_number_of_faces++;
		}
	}

	int final_number_faces = 0;
	face* face_buffer = non_maximum_suppression(face_vector, total_faces, &final_number_faces);
  printf("final_number_faces: %d, total_faces: %d\n", final_number_faces, total_faces);

	for(int i = 0; i < final_number_faces; i++)
	{
		if (face_buffer[i].w <= 0 || face_buffer[i].h <= 0)
		{
			continue;
		}
		draw_rect(input_image, face_buffer[i].x, face_buffer[i].y, face_buffer[i].w, face_buffer[i].h, 5, 0);
	}

	save_image(input_image, argv[2]);

	free(W);
	fclose(ptrW);
	fclose(ptrB);

	free_image(input_image);
}

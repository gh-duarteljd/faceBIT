#include "../../VISION/VISION.h"

// HOG parameters.
#define PIXELS_PER_CELL 	 		8
#define CELLS_PER_BLOCK 	 		4
#define WIDTH_OF_WINDOW 	 		168
#define HEIGHT_OF_WINDOW			192
#define NUMBER_OF_ANGLE_BINS	9

#define NUMBER_OF_FEATURES	((HEIGHT_OF_WINDOW /  PIXELS_PER_CELL) - (CELLS_PER_BLOCK -1)) * ((WIDTH_OF_WINDOW /  PIXELS_PER_CELL) - (CELLS_PER_BLOCK -1)) * CELLS_PER_BLOCK * CELLS_PER_BLOCK * 9

// Frontal faces database parameters.
#define NUMBER_OF_IMAGES 	1800
#define NUMBER_OF_FACES 	500
#define NUMBER_OF_NOFACES	1300

// Fuctions prototypes.
FILE* 		database_file_creator();
float** 	database_hog_features_decriptor();
void 			fill_database_file(FILE* hog_database_file, float** hog_database);

int main(int argc, char const *argv[])
{
	// ensure proper usage
	if (argc != 1)
	{
			printf("Usage: ./database_hog_descriptor \n");
			exit(1);
	}

	// Create empty file to hog database.
	FILE* hog_database_file = database_file_creator();

	// Create hog database with image database.
	float** hog_database = database_hog_features_decriptor();

	// Fill empty file with hog features.
	fill_database_file(hog_database_file, hog_database);

	return 0;
}

FILE* database_file_creator()
{
	FILE* hog_database_file = fopen("CSV/hog_descriptor.csv", "w");
  if (hog_database_file == NULL)
  {
		fclose(hog_database_file);
  	fprintf(stderr, "Could not create CSV/hog_descriptor.csv.\n");
    exit(6);
  }
	else
	{
		return hog_database_file;
	}
}

float** database_hog_features_decriptor()
{
	float** hog_database = (float**)malloc(NUMBER_OF_IMAGES * sizeof(float*));

	char path[50];

	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
	{

		if (i < NUMBER_OF_FACES)
		{
			char folder[] = "../../databases/faces/train";
			sprintf(path, "%s/%d.pgm", folder, i + 1);
		}
		else
		{
			char folder[] = "../../databases/nofaces";
			sprintf(path, "%s/%d.pgm", folder, (i - NUMBER_OF_FACES + 1));
		}

		image aux_image = load_image(path);
		image current_image = histogram_equalizer(aux_image);

		hog_database[i] = (hog(current_image, PIXELS_PER_CELL, CELLS_PER_BLOCK, NUMBER_OF_ANGLE_BINS)).hog_vector;

		free_image(aux_image);
		free_image(current_image);
	}

	return hog_database;
}

void fill_database_file(FILE* hog_database_file, float** hog_database)
{
	fprintf(hog_database_file, "target, ");
	for (int i = 0; i < NUMBER_OF_FEATURES; i++)
	{
		if (i < (NUMBER_OF_FEATURES - 1))
		{
			fprintf(hog_database_file, "%d, ", i);
		}
		else
		{
			fprintf(hog_database_file, "%d", i);
		}
	}
	fprintf(hog_database_file, "\n");

	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
	{

		unsigned char target;
		if (i < NUMBER_OF_FACES)
		{
			target = 1;
		}
		else
		{
			target = 0;
		}
		fprintf(hog_database_file, "%d, ", target);

		for (int j = 0; j < NUMBER_OF_FEATURES; j++)
		{
			if (j < (NUMBER_OF_FEATURES - 1))
			{
				fprintf(hog_database_file, "%f, ", hog_database[i][j]);
			}
			else
			{
				fprintf(hog_database_file, "%f", hog_database[i][j]);
			}
		}
		fprintf(hog_database_file, "\n");
	}

	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
	{
		free(hog_database[i]);
	}
	free(hog_database);

	fclose(hog_database_file);
}

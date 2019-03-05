#include "../../../VISION/VISION.h"

#define WIDTH_OF_WINDOW 	 		168
#define HEIGHT_OF_WINDOW			192

#define NUMBER_OF_IMAGES 			1800
#define NUMBER_OF_FACES 			500
#define NUMBER_OF_NOFACES			1300

#define NUMBER_OF_COMPONENTS	100

FILE* 		database_file_creator();
float** 	database_pca_features_decriptor(pca_matrix P, pca_matrix U);
void 			fill_database_file(FILE* pca_database_file, float** pca_database);

int main(int argc, char const *argv[])
{
	if (argc != 1)
	{
			printf("usage: ./database_pca_descriptor\n");
			exit(1);
	}

	FILE* ptr_P = fopen("../../../VISION/pca/files/P", "r");
	if (ptr_P == NULL)
  {
		fclose(ptr_P);
		printf("P file not found\n");
    exit(1);
  }

  FILE* ptr_U = fopen("../../../VISION/pca/files/U", "r");
	if (ptr_U == NULL)
  {
		fclose(ptr_U);
		printf("U file not found\n");
    exit(1);
  }

	pca_matrix P;
	P.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
	P.c = NUMBER_OF_COMPONENTS;
	P.matrix = malloc(P.l * sizeof(float*));
  for (int i = 0; i < P.l; i++)
  {
    P.matrix[i] = malloc(P.c * sizeof(float));
		for (int j = 0; j < P.c; j++)
    {
      fread(&P.matrix[i][j], sizeof(float), 1, ptr_P);
    }
  }

	pca_matrix U;
	U.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
	U.c = 1;
	U.matrix = malloc(U.l * sizeof(float*));
  for (int i = 0; i < U.l; i++)
  {
    U.matrix[i] = malloc(U.c * sizeof(float));
		for (int j = 0; j < U.c; j++)
    {
      fread(&U.matrix[i][j], sizeof(float), 1, ptr_U);
    }
  }

	FILE* pca_database_file = database_file_creator();

	float** pca_database = database_pca_features_decriptor(P, U);

	fill_database_file(pca_database_file, pca_database);

	pca_matrix_free(P);

	pca_matrix_free(U);

	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
	{
		free(pca_database[i]);
	}
	free(pca_database);

	fclose(ptr_P);

	fclose(ptr_U);

	fclose(pca_database_file);

	return 0;
}

FILE* database_file_creator()
{
	FILE* pca_database_file = fopen("CSV/pca_descriptor.csv", "w");
  if (pca_database_file == NULL)
  {
		fclose(pca_database_file);
  	printf("could not create file pca_descriptor.csv\n");
    exit(1);
  }
	else
	{
		return pca_database_file;
	}
}

float** database_pca_features_decriptor(pca_matrix P, pca_matrix U)
{
	float** pca_database = malloc(NUMBER_OF_IMAGES * sizeof(float*));

	char path[100];

	for (int i = 0; i < NUMBER_OF_IMAGES; i++)
	{

		if (i < NUMBER_OF_FACES)
		{
			char folder[] = "../../../databases/faces/train";
			sprintf(path, "%s/%d.pgm", folder, i + 1);
		}
		else
		{
			char folder[] = "../../../databases/nofaces";
			sprintf(path, "%s/%d.pgm", folder, (i - NUMBER_OF_FACES + 1));
		}

		image current_image = load_image(path);

		pca_database[i] = (pca(current_image, P.matrix, U.matrix, WIDTH_OF_WINDOW, HEIGHT_OF_WINDOW, NUMBER_OF_COMPONENTS)).pca_vector;

		free_image(current_image);
	}

	return pca_database;
}

void fill_database_file(FILE* pca_database_file, float** pca_database)
{
	fprintf(pca_database_file, "target, ");
	for (int i = 0; i < NUMBER_OF_COMPONENTS; i++)
	{
		if (i < (NUMBER_OF_COMPONENTS - 1))
		{
			fprintf(pca_database_file, "%d, ", i);
		}
		else
		{
			fprintf(pca_database_file, "%d", i);
		}
	}
	fprintf(pca_database_file, "\n");

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
		fprintf(pca_database_file, "%d, ", target);

		for (int j = 0; j < NUMBER_OF_COMPONENTS; j++)
		{
			if (j < (NUMBER_OF_COMPONENTS - 1))
			{
				fprintf(pca_database_file, "%f, ", pca_database[i][j]);
			}
			else
			{
				fprintf(pca_database_file, "%f", pca_database[i][j]);
			}
		}
		fprintf(pca_database_file, "\n");
	}
}

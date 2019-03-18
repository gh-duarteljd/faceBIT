#include "../../../VISION/VISION.h"

#define NUMBER_OF_COMPONENTS	30

#define WIDTH_OF_WINDOW  168

#define HEIGHT_OF_WINDOW 192

#define NUMBER_OF_FACES 2402

int svm_classification(pca_features features, float* W, float B);

int main(int argc, char** argv)
{
	if (argc != 1)
	{
			printf("usage: ./faceDET\n");
			exit(1);
	}

	FILE* ptr_P = fopen("../../../VISION/pca/files/P", "rb");
  if (ptr_P == NULL)
  {
		fclose(ptr_P);
		printf("P file not found\n");
    exit(1);
  }
  FILE* ptr_U = fopen("../../../VISION/pca/files/U", "rb");
  if (ptr_U == NULL)
  {
		fclose(ptr_U);
		printf("U file not found\n");
    exit(1);
  }

	FILE* ptr_B = fopen("../../training/pca/SVM/B", "rb");
	if (ptr_B == NULL)
  {
		fclose(ptr_B);
		printf("B file not found\n");
  	exit(1);
  }

	FILE* ptr_W = fopen("../../training/pca/SVM/W", "rb");
	if (ptr_W == NULL)
  {
		fclose(ptr_W);
		printf("W file not found\n");
    exit(1);
	}

	pca_matrix P;
	P.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
	P.c = NUMBER_OF_COMPONENTS;
	P.matrix = malloc(P.l * sizeof(float*));
  for (int i = 0; i < P.l; i++)
  {
    P.matrix[i] = malloc(P.c * sizeof(float));
		fread(P.matrix[i], sizeof(float), P.c, ptr_P);
  }

	pca_matrix U;
	U.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
	U.c = 1;
	U.matrix = malloc(U.l * sizeof(float*));
  for (int i = 0; i < U.l; i++)
  {
    U.matrix[i] = malloc(U.c * sizeof(float));
		fread(U.matrix[i], sizeof(float), U.c, ptr_U);
  }

	float B;
	fread(&B, sizeof(float), 1, ptr_B);

	float* W = malloc(NUMBER_OF_COMPONENTS * sizeof(float));
	fread(W, sizeof(float), NUMBER_OF_COMPONENTS, ptr_W);

	//////////

	char path[100];

	for (int i = 0; i < NUMBER_OF_FACES; i++)
	{
		sprintf(path, "../../../metrics/faces/%d.pgm", i + 1);

		image current_image = load_image(path);

		//image current_image = histogram_equalizer(input_image);

		pca_features features = pca(current_image, P.matrix, U.matrix, WIDTH_OF_WINDOW, HEIGHT_OF_WINDOW, NUMBER_OF_COMPONENTS);

		int class = svm_classification(features, W, B);

		if (class == 0)
		{
			sprintf(path, "../../../metrics/investigate_nofaces/%d.pgm", i + 1);
			save_image(current_image, path);
		}
		else
		{
			sprintf(path, "../../../metrics/investigate_faces/%d.pgm", i + 1);
			save_image(current_image, path);
		}

		free(features.pca_vector);

		free_image(current_image);

		//free_image(input_image);
	}

	//////////

	for (int i = 0; i < P.l; i++)
  {
		free(P.matrix[i]);
		free(U.matrix[i]);
	}
	free(P.matrix);
	free(U.matrix);

	free(W);

	fclose(ptr_W);
	fclose(ptr_B);
	fclose(ptr_P);
	fclose(ptr_U);
}

int svm_classification(pca_features features, float* W, float B)
{
	float F = 0;
	for (int i = 0; i < NUMBER_OF_COMPONENTS; i++)
	{
		F = F + W[i] * features.pca_vector[i];
	}
	if ((F + B) > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#include "../../../VISION/VISION.h"

#define NUMBER_OF_COMPONENTS	100

#define WIDTH_OF_WINDOW  168

#define HEIGHT_OF_WINDOW 192

float
pca_probability(pca_features pca_buffer,
														float* 			 W,
														float 			 B);

int main(int argc, char** argv)
{
  // ensure proper usage
	if (argc != 3)
	{
			printf("Usage: ./faceDET input_image output_image \n");
			exit(1);
	}

	// Open eigenfaces and mean face.
	FILE* outptr_P = fopen("../../../VISION/pca/eigen/P", "r");
  if (outptr_P == NULL)
  {
		printf("ERRO P \n");
    exit(1);
  }
  FILE* outptr_U = fopen("../../../VISION/pca/eigen/U", "r");
  if (outptr_U == NULL)
  {
		printf("ERRO U \n");
    exit(1);
  }

	pca_matrix U;
	U.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
	U.c = 1;
	U.matrix = malloc(U.l * sizeof(float*));
  for (int i = 0; i < U.l; i++)
  {
    U.matrix[i] = malloc(U.c * sizeof(float));
  }
	pca_matrix P;
	P.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
	P.c = NUMBER_OF_COMPONENTS;
	P.matrix = malloc(P.l * sizeof(float*));
  for (int i = 0; i < P.l; i++)
  {
    P.matrix[i] = malloc(P.c * sizeof(float));
  }

	for (int i = 0; i < U.l; i++)
  {
    for (int j = 0; j < U.c; j++)
    {
      fread(&U.matrix[i][j], sizeof(float), 1, outptr_U);
    }
  }

  for (int i = 0; i < P.l; i++)
  {
    for (int j = 0; j < P.c; j++)
    {
      fread(&P.matrix[i][j], sizeof(float), 1, outptr_P);
    }
  }

	float B;
	FILE* ptrB = fopen("../../training/pca/SVM/B", "r");
	if (ptrB == NULL)
   	{
		fprintf(stderr, "Unable to open training data.\n");
      		exit(1);
   	}
	fread(&B, sizeof(float), 1, ptrB);

	FILE* ptrW = fopen("../../training/pca/SVM/W", "r");
	if (ptrW == NULL)
   	{
		fprintf(stderr, "Unable to open training data.\n");
      		exit(1);
   	}
	float* W = malloc(NUMBER_OF_COMPONENTS * sizeof(float));
	fread(W, sizeof(float), NUMBER_OF_COMPONENTS, ptrW);

	image input_image = load_image(argv[1]);

	pca_features pca_buffer = pca(input_image, P.matrix, U.matrix, WIDTH_OF_WINDOW, HEIGHT_OF_WINDOW, NUMBER_OF_COMPONENTS);

	float prob = pca_probability(pca_buffer, W, B);

	printf("%f\n", prob);

	free(W);
	fclose(ptrW);
	fclose(ptrB);

	free_image(input_image);
}

float
pca_probability(pca_features pca_buffer,
														float* 			 W,
														float 			 B)
{
	float F = 0;
	for (int i = 0; i < NUMBER_OF_COMPONENTS; i++)
	{
		F = F + W[i] * pca_buffer.pca_vector[i];
		printf("%f, %f\n", W[i], pca_buffer.pca_vector[i]);
	}

	return F + B;
}

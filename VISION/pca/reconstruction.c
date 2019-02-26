#include "../VISION/VISION.h"
#include "pca.h"

#define WIDTH_OF_WINDOW        168
#define HEIGHT_OF_WINDOW       192
#define NUMBER_OF_FACES        2019
#define NUMBER_OF_COMPONENTS   100

int main(int argc, char** argv)
{
  FILE* imgptr_P = fopen("P", "r");
  if (imgptr_P == NULL)
  {
    exit(1);
  }

  FILE* imgptr_U = fopen("U", "r");
  if (imgptr_U == NULL)
  {
    exit(1);
  }

  pca_matrix P;
  P.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
  P.c = NUMBER_OF_FACES;
  P.matrix = malloc(P.l * sizeof(double*));
  for (int i = 0; i < P.l; i++)
  {
    P.matrix[i] = malloc(P.c * sizeof(double));
    for (int j = 0; j < P.c; j++)
    {
      fread(&P.matrix[i][j], sizeof(double), 1, imgptr_P);
    }
  }

  pca_matrix U;
  U.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
  U.c = 1;
  U.matrix = malloc(U.l * sizeof(double*));
  for (int i = 0; i < U.l; i++)
  {
    U.matrix[i] = malloc(U.c * sizeof(double));
    for (int j = 0; j < U.c; j++)
    {
      fread(&U.matrix[i][j], sizeof(double), 1, imgptr_U);
    }
  }

  image input_image = load_image(argv[1]);

  pca_matrix X;
  X.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
  X.c = 1;
  X.matrix = malloc(X.l * sizeof(double*));
  for (int i = 0; i < X.l; i++)
  {
    X.matrix[i] = malloc(X.c * sizeof(double));
  }

  for (int i = 0, k = 0; i < HEIGHT_OF_WINDOW; i++)
  {
    for (int j = 0; j < WIDTH_OF_WINDOW; j++, k++)
    {
      X.matrix[k][0] = input_image.pixels_map[i][j];
    }
  }

  P.c = NUMBER_OF_COMPONENTS;

  pca_matrix D = mean_deviations(X, U);

  pca_matrix P_T = transposed_matrix(P);

  pca_matrix W = matrix_multiplication(P_T, D);

  pca_matrix D_F = matrix_multiplication(P, W);

  double e = 0;

  for (int i = 0; i < (HEIGHT_OF_WINDOW * WIDTH_OF_WINDOW); i++)
  {
    e = e + ((D.matrix[i][0] - D_F.matrix[i][0]) * (D.matrix[i][0] - D_F.matrix[i][0]));
  }

  printf("erro: %lf\n", sqrt(e));
}

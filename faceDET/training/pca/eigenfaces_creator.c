#include "../VISION/VISION.h"
#include "pca.h"

#define WIDTH_OF_WINDOW        168
#define HEIGHT_OF_WINDOW       192
#define NUMBER_OF_FACES        2019

int main()
{
  pca_matrix Xt;
  Xt.l = NUMBER_OF_FACES;
  Xt.c = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
  Xt.matrix = malloc(Xt.l * sizeof(double*));

  for (int i = 0; i < NUMBER_OF_FACES; i++)
  {
    char path[30];
    char folder[] = "../databases/Yale";
    sprintf(path, "%s/%d.pgm", folder, i + 1);
    image current_image = load_image(path);
    Xt.matrix[i] = image_unidimensionalization(current_image.pixels_map, HEIGHT_OF_WINDOW, WIDTH_OF_WINDOW);
    free_image(current_image);
  }

  pca_matrix X = transposed_matrix(Xt);

  pca_matrix U = empirical_mean(X);

  pca_matrix B = mean_deviations(X, U);

  pca_matrix C = covariance_matrix(B);

  gsl_vector *eval_gsl = gsl_vector_alloc (C.c);
  gsl_matrix *evec_gsl = gsl_matrix_alloc (C.l, C.c);

  eigenvectors_and_eigenvalues(C, eval_gsl, evec_gsl);

  double* evec = gsl_matrix_ptr(evec_gsl, 0, 0);
  double* eval = gsl_vector_ptr(eval_gsl, 0);

  pca_matrix V;
  V.l = C.l;
  V.c = C.c;
  V.matrix = malloc(V.l * sizeof(double*));
  for (int i = 0, k = 0; i < V.l; i++)
  {
    V.matrix[i] = malloc(V.c * sizeof(double));
    for (int j = 0; j < V.c; j++, k++)
    {
      V.matrix[i][j] = evec[k];
    }
  }

   pca_matrix P = matrix_multiplication(B, V);
   for (int i = 0; i < P.c; i++)
   {
     double norm_2 = 0;
     for (int j = 0; j < P.l; j++)
     {
       norm_2 = norm_2 + P.matrix[j][i] * P.matrix[j][i];
     }
     for (int j = 0; j < P.l; j++)
     {
       P.matrix[j][i] = P.matrix[j][i] / sqrt(norm_2);
     }
   }

  FILE* outptr_P = fopen("P", "w");
  if (outptr_P == NULL)
  {
    exit(1);
  }
  FILE* outptr_U = fopen("U", "w");
  if (outptr_U == NULL)
  {
    exit(1);
  }

  for (int i = 0; i < U.l; i++)
  {
    for (int j = 0; j < U.c; j++)
    {
      fwrite(&U.matrix[i][j], sizeof(double), 1, outptr_U);
    }
  }

  for (int i = 0; i < P.l; i++)
  {
    for (int j = 0; j < P.c; j++)
    {
      fwrite(&P.matrix[i][j], sizeof(double), 1, outptr_P);
    }
  }

  gsl_vector_free (eval_gsl);
  gsl_matrix_free (evec_gsl);
}

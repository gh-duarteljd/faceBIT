#include "../VISION.h"
#include "pca.h"

#define WIDTH_OF_WINDOW        168
#define HEIGHT_OF_WINDOW       192
#define NUMBER_OF_FACES        500
#define NUMBER_OF_COMPONENTS   100

float* matrix_planning(float** m, int height, int width);

int main()
{
  pca_matrix I;
  I.l = NUMBER_OF_FACES;
  I.c = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
  I.matrix = malloc(I.l * sizeof(float*));

  for (int i = 0; i < NUMBER_OF_FACES; i++)
  {
    char path[100];
    sprintf(path, "../../databases/faces/train/%d.pgm", i + 1);
    image current_image = load_image(path);
    I.matrix[i] = matrix_planning(current_image.pixels_map, HEIGHT_OF_WINDOW, WIDTH_OF_WINDOW);
    free_image(current_image);
  }

  pca_matrix X = transposed_matrix(I);

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
  V.matrix = malloc(V.l * sizeof(float*));
  for (int i = 0, k = 0; i < V.l; i++)
  {
    V.matrix[i] = malloc(V.c * sizeof(float));
    for (int j = 0; j < V.c; j++, k++)
    {
      V.matrix[i][j] = (float)evec[k];
    }
  }

   pca_matrix P = matrix_multiplication(B, V);
   P.c = NUMBER_OF_COMPONENTS;
   for (int i = 0; i < P.c; i++)
   {
     float norm_2 = 0;
     for (int j = 0; j < P.l; j++)
     {
       norm_2 = norm_2 + P.matrix[j][i] * P.matrix[j][i];
     }
     for (int j = 0; j < P.l; j++)
     {
       P.matrix[j][i] = P.matrix[j][i] / sqrt(norm_2);
     }
   }

  FILE* ptr_P = fopen("files/P", "wb");
  if (ptr_P == NULL)
  {
		printf("could not create file P\n");
    exit(1);
  }
  FILE* ptr_U = fopen("files/U", "wb");
  if (ptr_U == NULL)
  {
    printf("could not create file U\n");
    exit(1);
  }

  for (int i = 0; i < U.l; i++)
  {
    for (int j = 0; j < U.c; j++)
    {
      fwrite(&U.matrix[i][j], sizeof(float), 1, ptr_U);
    }
  }

  for (int i = 0; i < P.l; i++)
  {
    for (int j = 0; j < P.c; j++)
    {
      fwrite(&P.matrix[i][j], sizeof(float), 1, ptr_P);
    }
  }

  gsl_vector_free (eval_gsl);

  gsl_matrix_free (evec_gsl);

  pca_matrix_free(I);

  pca_matrix_free(X);

  pca_matrix_free(U);

  pca_matrix_free(B);

  pca_matrix_free(C);

  pca_matrix_free(V);

  pca_matrix_free(P);

  fclose(ptr_P);

  fclose(ptr_U);
}

float* matrix_planning(float** m, int height, int width)
{
  float* v = malloc(width * height * sizeof(float));

  for (int i = 0, k = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++, k++)
    {
      v[k] = m[i][j];
    }
  }
  return v;
}

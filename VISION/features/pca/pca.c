#include "pca.h"

pca_features
pca(image    input_image,
    float**  p,
    float**  u,
    int 	   WIDTH_OF_WINDOW,
    int 	   HEIGHT_OF_WINDOW,
    int      NUMBER_OF_COMPONENTS)
{
  pca_matrix P;
  P.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
  P.c = NUMBER_OF_COMPONENTS;
  P.matrix = p;

  pca_matrix U;
  U.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
  U.c = 1;
  U.matrix = u;

  pca_matrix X;
  X.l = WIDTH_OF_WINDOW * HEIGHT_OF_WINDOW;
  X.c = 1;
  X.matrix = malloc(X.l * sizeof(float*));
  for (int i = 0; i < X.l; i++)
  {
    X.matrix[i] = malloc(X.c * sizeof(float));
  }

  for (int i = 0, k = 0; i < HEIGHT_OF_WINDOW; i++)
  {
    for (int j = 0; j < WIDTH_OF_WINDOW; j++, k++)
    {
      X.matrix[k][0] = input_image.pixels_map[i][j];
    }
  }

  pca_matrix D = mean_deviations(X, U);

  pca_matrix P_T = transposed_matrix(P);

  pca_matrix W = matrix_multiplication(P_T, D);

  pca_features output_pca;
  output_pca.size = NUMBER_OF_COMPONENTS;
  output_pca.pca_vector = malloc(output_pca.size * sizeof(float));

  for (int i = 0; i < output_pca.size; i++)
  {
    output_pca.pca_vector[i] = W.matrix[i][0];
  }

  pca_matrix_free(D);

  pca_matrix_free(P_T);

  pca_matrix_free(W);

  return output_pca;
}

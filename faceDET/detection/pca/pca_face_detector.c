#include "pca.h"

char
pca_face_detector(image    input_image,
                  face     input_face,
						      double** p,
		    		      double** u,
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
  X.matrix = malloc(X.l * sizeof(double*));
  for (int i = 0; i < X.l; i++)
  {
    X.matrix[i] = malloc(X.c * sizeof(double));
  }

	image image_buffer;
	image_buffer.width = WIDTH_OF_WINDOW;
	image_buffer.height = HEIGHT_OF_WINDOW;
	image_buffer.pixels_map = (double**)malloc(HEIGHT_OF_WINDOW * sizeof(double*));
	for (int i = 0; i < HEIGHT_OF_WINDOW; i++)
	{
		image_buffer.pixels_map[i] = (double*) malloc( WIDTH_OF_WINDOW * sizeof(double));
	}

  for (int p = input_face.y - (input_face.h / 2), a = 0; p < input_face.y + (input_face.h / 2); p++, a++)
  {
    for (int q = input_face.x - (input_face.w / 2), b = 0; q < input_face.x + (input_face.w / 2); q++, b++)
    {
      image_buffer.pixels_map[a][b] = input_image.pixels_map[p][q];
    }
  }

  for (int i = 0, k = 0; i < HEIGHT_OF_WINDOW; i++)
  {
    for (int j = 0; j < WIDTH_OF_WINDOW; j++, k++)
    {
      X.matrix[k][0] = image_buffer.pixels_map[i][j];
    }
  }

  pca_matrix D = mean_deviations(X, U);

  pca_matrix P_T = transposed_matrix(P);

  pca_matrix W = matrix_multiplication(P_T, D);

  pca_matrix D_F = matrix_multiplication(P, W);

  double e = 0;

  for (int i = 0; i < (HEIGHT_OF_WINDOW * WIDTH_OF_WINDOW); i++)
  {
    e = e + ((D.matrix[i][0] - D_F.matrix[i][0]) * (D.matrix[i][0] - D_F.matrix[i][0]));
  }

  printf("\nerro: %lf\n", sqrt(e));

  for (int i = 0; i < D.l; i++)
  {
    free(D.matrix[i]);
  }
  free(D.matrix);

  for (int i = 0; i < P_T.l; i++)
  {
    free(P_T.matrix[i]);
  }
  free(P_T.matrix);

  for (int i = 0; i < W.l; i++)
  {
    free(W.matrix[i]);
  }
  free(W.matrix);

  for (int i = 0; i < D_F.l; i++)
  {
    free(D_F.matrix[i]);
  }
  free(D_F.matrix);

	free_image(image_buffer);

	if (sqrt(e) > 5000)
	{
  		return 1;
	}

	return 0;
}

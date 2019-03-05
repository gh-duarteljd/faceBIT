#include "pca.h"

pca_matrix empirical_mean(pca_matrix X)
{
  pca_matrix U;

  U.l = X.l;
  U.c = 1;

  U.matrix = malloc(U.l * sizeof(float*));
  for (int i = 0; i < U.l; i++)
  {
    U.matrix[i] = calloc(U.c, sizeof(float));
    for(int j = 0; j < X.c; j++)
    {
       U.matrix[i][0] =  U.matrix[i][0] + X.matrix[i][j];
    }
    U.matrix[i][0] =  U.matrix[i][0] / X.c;
  }

  return U;
}

pca_matrix mean_deviations(pca_matrix X, pca_matrix U)
{
  pca_matrix B;

  B.l = X.l;
  B.c = X.c;

  B.matrix = malloc(B.l * sizeof(float*));
  for (int i = 0; i < B.l; i++)
  {
    B.matrix[i] = malloc(B.c * sizeof(float));
    for (int j = 0; j < B.c; j++)
    {
      B.matrix[i][j] = X.matrix[i][j] - U.matrix[i][0];
    }
  }

  return B;
}

pca_matrix matrix_multiplication(pca_matrix A, pca_matrix B)
{
  pca_matrix C;

  C.l = A.l;
  C.c = B.c;

  C.matrix = malloc(C.l * sizeof(float*));
  for (int i = 0; i < C.l; i++)
  {
    C.matrix[i] = calloc(C.c, sizeof(float));
    for (int j = 0; j < C.c; j++)
    {
      for(int k = 0; k < A.c; k++)
      {
         C.matrix[i][j] = C.matrix[i][j] + (B.matrix[k][j] * A.matrix[i][k]);
      }
    }
  }

  return C;
}

pca_matrix transposed_matrix(pca_matrix A)
{
  pca_matrix B;
  B.l = A.c;
  B.c = A.l;

  B.matrix = malloc(B.l * sizeof(float*));
  for (int i = 0; i < B.l; i++)
  {
    B.matrix[i] = malloc(B.c * sizeof(float));
    for (int j = 0; j < B.c; j++)
    {
      B.matrix[i][j] = A.matrix[j][i];
    }
  }

  return B;
}

pca_matrix covariance_matrix(pca_matrix B)
{
  pca_matrix Bt = transposed_matrix(B);
  pca_matrix C = matrix_multiplication(Bt, B);

  return C;
}

void eigenvectors_and_eigenvalues(pca_matrix C, gsl_vector *eval, gsl_matrix *evec)
{
  double* c = malloc(sizeof(double) * C.l * C.c);
  for(int i = 0, k = 0; i < C.l; i++)
  {
    for(int j = 0; j < C.c; j++, k++)
    {
        c[k] = C.matrix[i][j];
    }
  }

  gsl_matrix_view m = gsl_matrix_view_array (c, C.l, C.c);

  gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc (C.l);

  gsl_eigen_symmv (&m.matrix, eval, evec, w);

  gsl_eigen_symmv_free (w);

  gsl_eigen_symmv_sort (eval, evec, GSL_EIGEN_SORT_VAL_DESC);

  return;
}

void pca_matrix_free(pca_matrix M)
{
  for (int i = 0; i < M.l; i++)
  {
    free(M.matrix[i]);
  }
  free(M.matrix);
}

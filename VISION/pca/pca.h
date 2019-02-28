#ifndef PCA_H
#define PCA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

#include "../VISION.h"

typedef struct
{
    float** matrix;
    int l;
    int c;
}
pca_matrix;

pca_matrix empirical_mean(pca_matrix X);

pca_matrix mean_deviations(pca_matrix X, pca_matrix U);

pca_matrix matrix_multiplication(pca_matrix A, pca_matrix B);

pca_matrix transposed_matrix(pca_matrix A);

pca_matrix covariance_matrix(pca_matrix B);

void eigenvectors_and_eigenvalues(pca_matrix C, gsl_vector *eval, gsl_matrix *evec);

float* matrix_planning(float** m, int height, int width);

#endif /* PCA_H */

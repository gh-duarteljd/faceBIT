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
    double** matrix;
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

double* image_unidimensionalization(double** Xi, int height_of_face, int width_of_face);

char
pca_face_detector(image    input_image,
             face     input_face,
						 double** p,
		    		 double** u,
             int 	    WIDTH_OF_WINDOW,
             int 	    HEIGHT_OF_WINDOW,
             int      NUMBER_OF_FACES);

#endif /* PCA_H */

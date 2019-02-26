#include "features.h"

hog_features hog(image input_image,
								int 	 cell_width_in_pixels,
								int 	 block_width_in_pixels,
								int 	 number_of_angle_bins)
{
	image input_image_mag;
	image input_image_pha;
	grad2d(input_image, &input_image_mag, &input_image_pha);

	int height_in_cells = input_image.height /  cell_width_in_pixels;
	int width_in_cells = input_image.width /  cell_width_in_pixels;

	int height_in_blocks = height_in_cells - (block_width_in_pixels -1);
	int width_in_blocks = width_in_cells - (block_width_in_pixels -1);

	int number_of_cells = height_in_cells * width_in_cells;
	int number_of_blocks = height_in_blocks * width_in_blocks;

	hog_features output_hog;
	output_hog.size = height_in_blocks * width_in_blocks * block_width_in_pixels * block_width_in_pixels * number_of_angle_bins;
	output_hog.hog_vector = (float*)malloc(output_hog.size * sizeof(float));

	float** cell_mag = (float**)malloc(cell_width_in_pixels * sizeof(float*));
	float** cell_pha = (float**)malloc(cell_width_in_pixels * sizeof(float*));
	for (int i = 0; i < cell_width_in_pixels; i++)
	{
		cell_mag[i] = (float*) malloc( cell_width_in_pixels * sizeof(float));
		cell_pha[i] = (float*) malloc( cell_width_in_pixels * sizeof(float));
	}

	float* hog_cub_cells[height_in_cells][width_in_cells];

	for (int i = 0; i < height_in_cells; i++)
	{
		for (int j = 0; j < width_in_cells; j++)
		{
			for (int p = (i * cell_width_in_pixels), a = 0; p < ((i + 1) * cell_width_in_pixels); p++, a++)
			{
				for (int q = (j * cell_width_in_pixels), b = 0; q < ((j + 1) * cell_width_in_pixels); q++, b++)
				{
					cell_mag[a][b] = input_image_mag.pixels_map[p][q];
					cell_pha[a][b] = input_image_pha.pixels_map[p][q];
				}
			}
			hog_cub_cells[i][j] = histogram( cell_mag,
																			 cell_pha,
																			 cell_width_in_pixels,
																			 number_of_angle_bins);
		}
	}

	float* hog_cub_blocks[height_in_blocks][width_in_blocks];

	for (int i = 0; i < height_in_blocks; i++)
	{
		for(int j = 0; j < width_in_blocks; j++)
		{
			hog_cub_blocks[i][j] = (float*) malloc(block_width_in_pixels * block_width_in_pixels * number_of_angle_bins * sizeof(float));

			int histogram_class = 0;

			for (int p = i; p < (i + block_width_in_pixels); p++)
			{
				for (int q = j; q < (j + block_width_in_pixels); q++)
				{
					for (int r = 0; r < number_of_angle_bins; r++)
					{
						hog_cub_blocks[i][j][histogram_class] = hog_cub_cells[p][q][r];
						histogram_class ++;
					}
				}
			}

			float quadratic_sum = 0;
			for (int k = 0; k < (block_width_in_pixels * block_width_in_pixels * number_of_angle_bins); k++)
			{
				quadratic_sum = quadratic_sum + (hog_cub_blocks[i][j][k] * hog_cub_blocks[i][j][k]);
			}

			for (int k = 0; k < (block_width_in_pixels * block_width_in_pixels * number_of_angle_bins); k++)
			{
				hog_cub_blocks[i][j][k] = hog_cub_blocks[i][j][k] / sqrt(quadratic_sum + 0.0001);
			}
		}
	}

	int line = 0, column = 0, element = 0;

	for (int k = 0; k < (height_in_blocks * width_in_blocks * block_width_in_pixels * block_width_in_pixels * number_of_angle_bins); k++)
	{
		output_hog.hog_vector[k] = hog_cub_blocks[line][column][element];
		element ++;

		if (element == block_width_in_pixels * block_width_in_pixels  * number_of_angle_bins)
		{
			column++;
			element = 0;
		}

		if (column == width_in_blocks)
		{
			line++;
			column = 0;
		}

	}

	for (int i = 0; i < input_image_mag.height; i++)
	{
		free(input_image_mag.pixels_map[i]);
		free(input_image_pha.pixels_map[i]);
	}
	free(input_image_mag.pixels_map);
	free(input_image_pha.pixels_map);

	for (int i = 0; i < height_in_cells; i++)
	{
		for (int j = 0; j < width_in_cells; j++)
		{
			free(hog_cub_cells[i][j]);
		}
	}

	for (int i = 0; i < height_in_blocks; i++)
	{
		for (int j = 0; j < width_in_blocks; j++)
		{
			free (hog_cub_blocks[i][j]);
		}
	}

	for (int i = 0; i < cell_width_in_pixels; i++)
	{
		free(cell_mag[i]);
		free(cell_pha[i]);
	}
	free(cell_mag);
	free(cell_pha);

	return output_hog;
}

float*
histogram(float** cell_mag,
					float** cell_pha,
					int 		 cell_width_in_pixels,
					int 		 number_of_angle_bins)
{
	float* histogram_vector = (float*) calloc(number_of_angle_bins * sizeof(float), 1);

	float percentage = 0;

	for (int i = 0; i < cell_width_in_pixels; i++)
	{
		for (int j = 0; j < cell_width_in_pixels; j++)
		{
			if (cell_pha[i][j] < 0)
			{
				cell_pha[i][j] = cell_pha[i][j] + 180;
			}

			if ((cell_pha[i][j] >= 0) && (cell_pha[i][j] < 20))
			{
				percentage = (cell_pha[i][j] - 0) / 20.0;
				histogram_vector[0] += cell_mag[i][j] * (1.0 - percentage);
				histogram_vector[1] += cell_mag[i][j] * percentage;
			}
			else if ((cell_pha[i][j] >= 20) && (cell_pha[i][j] < 40))
			{
				percentage = (cell_pha[i][j] - 20) / 20.0;
				histogram_vector[1] += cell_mag[i][j] * (1.0 - percentage);
				histogram_vector[2] += cell_mag[i][j] * percentage;
			}
			else if ((cell_pha[i][j] >= 40) && (cell_pha[i][j] < 60))
			{
				percentage = (cell_pha[i][j] - 40) / 20.0;
				histogram_vector[2] += cell_mag[i][j] * (1.0 - percentage);
				histogram_vector[3] += cell_mag[i][j] * percentage;
			}
			else if ((cell_pha[i][j] >= 60) && (cell_pha[i][j] < 80))
			{
				percentage = (cell_pha[i][j] - 60) / 20.0;
				histogram_vector[3] += cell_mag[i][j] * (1.0 - percentage);
				histogram_vector[4] += cell_mag[i][j] * percentage;
			}
			else if ((cell_pha[i][j] >= 80) && (cell_pha[i][j] < 100))
			{
				percentage = (cell_pha[i][j] - 80) / 20.0;
				histogram_vector[4] += cell_mag[i][j] * (1.0 - percentage);
				histogram_vector[5] += cell_mag[i][j] * percentage;
			}
			else if ((cell_pha[i][j] >= 100) && (cell_pha[i][j] < 120))
			{
				percentage = (cell_pha[i][j] - 100) / 20.0;
				histogram_vector[5] += cell_mag[i][j] * (1.0 - percentage);
				histogram_vector[6] += cell_mag[i][j] * percentage;
			}
			else if ((cell_pha[i][j] >= 120) && (cell_pha[i][j] < 140))
			{
				percentage = (cell_pha[i][j] - 120) / 20.0;
				histogram_vector[6] += cell_mag[i][j] * (1.0 - percentage);
				histogram_vector[7] += cell_mag[i][j] * percentage;
			}
			else if ((cell_pha[i][j] >= 140) && (cell_pha[i][j] < 160))
			{
				percentage = (cell_pha[i][j] - 140) / 20.0;
				histogram_vector[7] += cell_mag[i][j] * (1.0 - percentage);
				histogram_vector[8] += cell_mag[i][j] * percentage;
			}
			else if ((cell_pha[i][j] >= 160) && (cell_pha[i][j] <= 180))
			{
				percentage = (cell_pha[i][j] - 160) / 20.0;
				histogram_vector[8] += cell_mag[i][j] * (1.0 - percentage);
				histogram_vector[0] += cell_mag[i][j] * percentage;
			}
		}
	}

	return histogram_vector;
}

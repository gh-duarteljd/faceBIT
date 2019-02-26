#include "interface.h"

image
load_image(char* file_path)
{
  image buffer;
  char  magic_number[3];
  int   max_pixel_intensity;
  int current_file_pointer = 0;

  FILE* imgptr = fopen(file_path, "r");
  if (imgptr == NULL)
  {
    fprintf(stderr, "\nNão foi possível abrir a imagem %s.\n", file_path);
    exit(1);
  }

	fscanf(imgptr,"%s", magic_number);
	if ((magic_number[0] =! 'P') || (magic_number[1] =! '5'))
	{
    fprintf(stderr, "O arquivo %s não é uma imagem PGM válida. \n", file_path);
    exit(1);
	}

  while ((current_file_pointer = fgetc(imgptr)) != EOF)
  {
    if ((current_file_pointer != '\n') || ((current_file_pointer == '\n') && ((current_file_pointer = fgetc(imgptr)) == '#')));
    else
    {
      fseek(imgptr, -1, SEEK_CUR);
      break;
    }
  }

	fscanf(imgptr, "%i %i\n%i\n", &buffer.width, &buffer.height, &max_pixel_intensity);

	buffer.pixels_map = (float**)malloc(buffer.height * sizeof(float*));
	for (int i = 0; i < buffer.height; i++)
	{
    buffer.pixels_map[i] = (float*)malloc(buffer.width * sizeof(float));
		for (int j = 0; j < buffer.width; j++)
    {
      unsigned char  pixel;
			fread(&pixel, sizeof(unsigned char), 1, imgptr);
			buffer.pixels_map[i][j] = (float)pixel;
    }
  }

	fclose(imgptr);

	return buffer;
}

void
save_image(image input_image, char* file_path)
{
  FILE* outptr = fopen(file_path, "w");
  if (outptr == NULL)
  {
    fprintf(stderr, "\nNão foi prossível criar o arquivo %s. :(\n", file_path);
    exit(1);
  }

	fprintf(outptr,"P5\n#created by VISION\n%i %i\n255\n", input_image.width, input_image.height);

  for (int i = 0; i < input_image.height; i++)
  {
    for (int j = 0; j < input_image.width; j++)
    {
      unsigned char pixel = (unsigned char)(input_image.pixels_map[i][j]);
      fwrite(&pixel, sizeof(unsigned char), 1, outptr);
    }
  }

	fclose(outptr);
}

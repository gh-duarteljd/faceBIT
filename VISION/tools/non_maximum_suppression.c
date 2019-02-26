 #include "tools.h"

 face*
 non_maximum_suppression(face* faces,
                         int   number_of_faces,
                         int*  final_number_faces)
 {

   if (number_of_faces < 1)
   {
     return;
   }

 	int number_of_cluster = 0;

 	int faces_per_cluster = 0;

  int* suppression_vector = (int*)calloc(number_of_faces, sizeof(int));

 	for (int i = 0; i < number_of_faces; i++)
 	{
    if (faces[i].p == 0)
    {
      continue;
    }
    faces_per_cluster = 1;

 		if (suppression_vector[i] == 0)
 		{
 			suppression_vector[i] = ++number_of_cluster;
 		}

 		for (int j = 0; j < number_of_faces; j++)
 		{

 			if (i == j)
 			{
 				continue;
 			}

      if (faces[j].p == 0)
      {
        continue;
      }

 			int dx = sqrt((faces[i].x - faces[j].x) * (faces[i].x - faces[j].x));
 			int dy = sqrt((faces[i].y - faces[j].y) * (faces[i].y - faces[j].y));

 			if (((dx < (faces[i].w / 2)) && (dy < (faces[i].h / 2))) || ((dx < (faces[j].w / 2)) && (dy < (faces[j].h / 2))))
 			{
 				suppression_vector[j] = suppression_vector[i];
 				faces_per_cluster++;
 			}
 		}

 		if (faces_per_cluster < 2)
 		{
      for (int j = 0; j < number_of_faces; j++)
      {
        if (suppression_vector[j] == suppression_vector[i])
        {
          suppression_vector[j] = 0;
        }
      }
      suppression_vector[i] = 0;
      number_of_cluster = 0;
 		}
 	}
 	face* face_buffer = (face*)malloc(number_of_cluster * sizeof(face));

 	for (int i = 0; i < number_of_cluster; i++)
 	{
 		face_buffer[i].x = 0;
    face_buffer[i].y = 0;
    face_buffer[i].w = 0;
    face_buffer[i].h = 0;
    face_buffer[i].p = 0;

 		for (int j = 0; j < number_of_faces; j++)
 		{
 			if (suppression_vector[j] == (i + 1))
 			{
 				face_buffer[i].x = face_buffer[i].x * face_buffer[i].p / (face_buffer[i].p + faces[j].p) + faces[j].x * faces[j].p / (face_buffer[i].p + faces[j].p);
 				face_buffer[i].y = face_buffer[i].y * face_buffer[i].p / (face_buffer[i].p + faces[j].p) + faces[j].y * faces[j].p / (face_buffer[i].p + faces[j].p);
 				face_buffer[i].w = face_buffer[i].w * face_buffer[i].p / (face_buffer[i].p + faces[j].p) + faces[j].w * faces[j].p / (face_buffer[i].p + faces[j].p);
 				face_buffer[i].h = face_buffer[i].h * face_buffer[i].p / (face_buffer[i].p + faces[j].p) + faces[j].h * faces[j].p / (face_buffer[i].p + faces[j].p);
 				face_buffer[i].p = (face_buffer[i].p + faces[j].p) / 2.0;
 			}
 		}
 	}
  int aux_number_of_cluster = number_of_cluster;
  for(int i = 0; i < number_of_cluster; i++)
	{
		if (face_buffer[i].w <= 0 || face_buffer[i].h <= 0)
		{
			aux_number_of_cluster--;
		}
  }
 	*final_number_faces = aux_number_of_cluster;
 	return face_buffer;
 }

/*
** Copyright (C) 2008 - Hagen Paul Pfeifer <hagen@jauu.net>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "global.h"


#define MAX_VERTICES 16000 // Max number of vertices (for each object)
#define MAX_POLYGONS 16000 // Max number of polygons (for each object)

char load_model_3ds(obj_type_ptr p_object, char *p_filename)
{
	int i; //Index variable
	int ret;

	FILE *l_file; //File pointer

	unsigned short l_chunk_id; //Chunk identifier
	unsigned int l_chunk_lenght; //Chunk lenght

	unsigned char l_char; //Char variable
	unsigned short l_qty; //Number of elements in each chunk

	unsigned short l_face_flags; //Flag that stores some face information

	struct stat statb;

	ret = stat(p_filename, &statb);
	if (ret < 0) {
		fprintf(stderr, "file errror %s:%s!\n", p_filename, strerror(errno));
		exit(1);
	}



	if ((l_file=fopen (p_filename, "rb"))== NULL) return 0; //Open the file

	while (ftell (l_file) < statb.st_size) //Loop to scan the whole file 
	{
		//getche(); //Insert this command for debug (to wait for keypress for each chuck reading)

		fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
		//printf("ChunkID: %x\n",l_chunk_id); 
		fread (&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
		//printf("ChunkLenght: %x\n",l_chunk_lenght);

		switch (l_chunk_id)
		{
			//----------------- MAIN3DS -----------------
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d 
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4d4d: 
				break;    

				//----------------- EDIT3DS -----------------
				// Description: 3D Editor chunk, objects layout info 
				// Chunk ID: 3d3d (hex)
				// Chunk Lenght: 0 + sub chunks
				//-------------------------------------------
			case 0x3d3d:
				break;

				//--------------- EDIT_OBJECT ---------------
				// Description: Object block, info for each object
				// Chunk ID: 4000 (hex)
				// Chunk Lenght: len(object name) + sub chunks
				//-------------------------------------------
			case 0x4000: 
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
					p_object->name[i]=l_char;
					i++;
				}while(l_char != '\0' && i<20);
				break;

				//--------------- OBJ_TRIMESH ---------------
				// Description: Triangular mesh, contains chunks for 3d mesh info
				// Chunk ID: 4100 (hex)
				// Chunk Lenght: 0 + sub chunks
				//-------------------------------------------
			case 0x4100:
				break;

				//--------------- TRI_VERTEXL ---------------
				// Description: Vertices list
				// Chunk ID: 4110 (hex)
				// Chunk Lenght: 1 x unsigned short (number of vertices) 
				//             + 3 x float (vertex coordinates) x (number of vertices)
				//             + sub chunks
				//-------------------------------------------
			case 0x4110: 
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				p_object->vertices_qty = l_qty;
				printf("Number of vertices: %d\n",l_qty);
				for (i=0; i<l_qty; i++)
				{
					fread (&p_object->vertex[i].x, sizeof(float), 1, l_file);
					//printf("Vertices list x: %f\n",p_object->vertex[i].x);
					fread (&p_object->vertex[i].y, sizeof(float), 1, l_file);
					//printf("Vertices list y: %f\n",p_object->vertex[i].y);
					fread (&p_object->vertex[i].z, sizeof(float), 1, l_file);
					//printf("Vertices list z: %f\n",p_object->vertex[i].z);
				}
				break;

				//--------------- TRI_FACEL1 ----------------
				// Description: Polygons (faces) list
				// Chunk ID: 4120 (hex)
				// Chunk Lenght: 1 x unsigned short (number of polygons) 
				//             + 3 x unsigned short (polygon points) x (number of polygons)
				//             + sub chunks
				//-------------------------------------------
			case 0x4120:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				p_object->polygons_qty = l_qty;
				printf("Number of polygons: %d\n",l_qty); 
				for (i=0; i<l_qty; i++)
				{
					fread (&p_object->polygon[i].a, sizeof (unsigned short), 1, l_file);
					//printf("Polygon point a: %d\n",p_object->polygon[i].a);
					fread (&p_object->polygon[i].b, sizeof (unsigned short), 1, l_file);
					//printf("Polygon point b: %d\n",p_object->polygon[i].b);
					fread (&p_object->polygon[i].c, sizeof (unsigned short), 1, l_file);
					//printf("Polygon point c: %d\n",p_object->polygon[i].c);
					fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
					//printf("Face flags: %x\n",l_face_flags);
				}
				break;

				//------------- TRI_MAPPINGCOORS ------------
				// Description: Vertices list
				// Chunk ID: 4140 (hex)
				// Chunk Lenght: 1 x unsigned short (number of mapping points) 
				//             + 2 x float (mapping coordinates) x (number of mapping points)
				//             + sub chunks
				//-------------------------------------------
			case 0x4140:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				for (i=0; i<l_qty; i++)
				{
					fread (&p_object->mapcoord[i].u, sizeof (float), 1, l_file);
					//printf("Mapping list u: %f\n",p_object->mapcoord[i].u);
					fread (&p_object->mapcoord[i].v, sizeof (float), 1, l_file);
					//printf("Mapping list v: %f\n",p_object->mapcoord[i].v);
				}
				break;

				//----------- Skip unknow chunks ------------
				//We need to skip all the chunks that currently we don't use
				//We use the chunk lenght information to set the file pointer
				//to the same level next chunk
				//-------------------------------------------
			default:
				fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
		} 
	}
	fclose (l_file); // Closes the file stream
	return (1); // Returns ok
}



/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */

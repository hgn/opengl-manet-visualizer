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

void free_image(struct image *image)
{
	if (image->data)
		free(image->data);
	free(image);
}

static struct image *load_image_bmp(char *filename)
{
	FILE *file;
	unsigned long size;
	unsigned long i;
	unsigned short int planes;
	unsigned short int bpp;
	char temp;
	struct image *image;

	image = xalloc(sizeof(struct image));

	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		free(image);
		return NULL;
	}

	fseek(file, 18, SEEK_CUR);

	if ((i = fread(&image->size_x, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return NULL;
	}

	if ((i = fread(&image->size_y, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		free(image);
		return NULL;
	}

	size = image->size_x * image->size_y * 3;

	if ((fread(&planes, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		free(image);
		return NULL;
	}
	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		free(image);
		return NULL;
	}

	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);
		free(image);
		return NULL;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		free(image);
		return NULL;
	}

	/* seek past the rest of the bitmap header */
	fseek(file, 24, SEEK_CUR);

	image->data = xalloc(size);

	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		free(image->data); free(image);
		return NULL;
	}

	for (i = 0; i < size; i += 3) {	// reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}

	return image;
}



struct image *load_image(char *filename, enum image_file_type ift)
{
	switch (ift) {
		case BMP_TYPE:
			return load_image_bmp(filename);
			break;
		case TIFF_TYPE:
			return NULL;
			break;
		default:
			die("programmed error - unsupported image file type");
			break;
	}
	return NULL;
}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */

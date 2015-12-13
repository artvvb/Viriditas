#ifndef _BMP_H_
#define _BMP_H_

#include "main.h"
#include "tex.h"

#define HEAD_SIZE 54

GLuint loadBMP_custom(const char * imagepath) {
	// Data read from the header of the BMP file
	unsigned char header[HEAD_SIZE];	// Each BMP file begins by a 54-bytes header
	unsigned int dataPos;		// Position in the file where the actual data begins
	unsigned int width, height;
	unsigned char *data;		// Actual RGB data

	GLuint texture;
	FILE * file;

	fopen_s(&file, imagepath, "rb");	// Open the file

	if (!file) { printf("Image could not be opened\n"); return 0; }

	if (fread(header, 1, HEAD_SIZE, file) != HEAD_SIZE){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M'){
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	cout << "header size = " << *(int*)&(header[0x0E]) << endl;
	dataPos = *(int*)&(header[0x0E]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	cout << "image size = " << *(int*)&(header[0x22]) << endl;

	cout << "width  = " << width << endl;
	cout << "height = " << height << endl;

	// Create a buffer
	data = new unsigned char[width*height * 3];

	// Read the actual data from the file into the buffer
	cout << "read " << (int)fread(data, 1, width*height * 3, file) << " bytes" << endl;
	char *c = new char[1];
	int n = 0;
	while (fread(c, 1, 1, file)) n++;
	cout << "n = " << n << " leftover bytes" << endl;

	//Everything is in memory now, the file can be closed
	fclose(file);

	Tex mytex(data, width, height);

	delete data;

	return mytex.texture;
}
#endif
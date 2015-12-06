#ifndef _TEX_H_
#define _TEX_H_

#include "main.h"

#define FRACTAL 0
#define CHECK 1

#define checkImageWidth		64
#define checkImageHeight	64
#define fractalImageHeight	65
#define fractalImageWidth	fractalImageHeight
#define fractalImageN		8

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];//RGBA
static GLubyte fractalImage[fractalImageHeight][fractalImageWidth][4];//RGBA

static GLuint texName;
void setPointColor(int type, int y, int x, GLubyte c) {
	switch (type) {
	case FRACTAL:
		fractalImage[y][x][0] = c;
		fractalImage[y][x][1] = c;
		fractalImage[y][x][2] = c;
		fractalImage[y][x][3] = (GLubyte)255; break;
	case CHECK:
		checkImage[y][x][0] = c;
		checkImage[y][x][1] = c;
		checkImage[y][x][2] = c;
		checkImage[y][x][3] = (GLubyte)255; break;
	}

}

void makeCheckImage(void)
{
	for (int i = 0; i < checkImageHeight; i++) {
		for (int j = 0; j < checkImageWidth; j++)
			setPointColor(CHECK, i, j, (GLubyte)(((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 0xff);
	}
}

GLubyte diamond_poll(int i, int j, int off) {
	int arr[4];
	arr[0] = fractalImage[i - off][j - off][0];
	arr[1] = fractalImage[i + off][j - off][0];
	arr[2] = fractalImage[i + off][j + off][0];
	arr[3] = fractalImage[i - off][j + off][0];
	int sum = 0;
	for (int i = 0; i < 4; i++)
		sum += arr[i];
	return 0xff;// (GLubyte)(sum / 4);
}
GLubyte square_poll(int i, int j, int off) {
	int arr[4] = { 0, 0, 0, 0 }, count = 0;
	if (j + off <= 64) { arr[0] = fractalImage[i][j + off][0]; count++; }
	if (j - off >= 0 ) { arr[1] = fractalImage[i][j - off][0]; count++; }
	if (i + off <= 64) { arr[2] = fractalImage[i + off][j][0]; count++; }
	if (i - off >= 0 ) { arr[3] = fractalImage[i - off][j][0]; count++; }
	int sum = 0;
	for (int i = 0; i < 4; i++)
		sum += arr[i];
	return (GLubyte)(sum / count);
}

void makeFractalImage(void) {
	srand((unsigned)time(NULL));
	for (int i = 0; i < fractalImageHeight; i += fractalImageHeight - 1) {
		for (int j = 0; j < fractalImageWidth; j += fractalImageWidth - 1)
			setPointColor(FRACTAL, i, j, rand() % 0xff);
	}
	/**/
	for (int off = (fractalImageHeight - 1) / 2; off > 0; off /= 2) {
		//DIAMOND
		for (int i = 0; i < fractalImageHeight; i += off) {
			for (int j = 0; j < fractalImageWidth; j += off) {
				if ((i / off) % 2 == (j / off) % 2)
					setPointColor(FRACTAL, i, j, diamond_poll(i, j, off));
			}
		}
		//SQUARE
		for (int i = off; i < fractalImageHeight; i += 2 * off) {
			for (int j = 0; j < fractalImageWidth; j += 2 * off) {
				setPointColor(FRACTAL, i, j, square_poll(i, j, off));
			}
		}
		for (int i = 0; i < fractalImageHeight; i += 2 * off) {
			for (int j = off; j < fractalImageWidth; j += 2 * off) {
				setPointColor(FRACTAL, i, j, square_poll(i, j, off));
			}
		}
	}
	/**/
}

void makeImage(int type) {
	switch (type) {
	case CHECK: return makeCheckImage();
	case FRACTAL: return makeFractalImage();
	}
}

void tex_init(int type)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	makeImage(type);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	switch (type) {
	case CHECK:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
			checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			checkImage);
	case FRACTAL:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fractalImageWidth,
			fractalImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			fractalImage);
	}
}

#endif
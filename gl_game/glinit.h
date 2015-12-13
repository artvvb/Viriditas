#ifndef _GLINIT_H_
#define _GLINIT_H_

#include "main.h"

void gl_init(int& argc, char *argv[], int width, int height, int x_pos, int y_pos) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);

}

#endif
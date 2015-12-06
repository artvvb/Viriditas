#pragma once
#include "main.h"
#include "coord.h"
#include "tex.h"

float tick = 0.0;

Coord corners[4] = {
	Coord(new float[3]{0.0, 0.0, 0.0}),
	Coord(new float[3]{0.0, 1.0, 0.0}),
	Coord(new float[3]{1.0, 1.0, 0.0}),
	Coord(new float[3]{1.0, 0.0, 0.0}),
};

Coord myorigin(new float[3]{0.0, 0.0, 0.0});

class Color {
public:
	float r, g, b;

	Color(float gray = 0.0f) {
		r = g = b = gray;
	}
	void render() const {
		glColor3f(r, g, b);
	}
};

float normalize(int x, int n) {
	return 2.0f * ((float)x / (float)n) - 1.0f;
}

Coord normalize(vector<int> x, vector<int> n) {
	Coord ret;
	ret['x'] = normalize(x[0], n[0]);
	ret['y'] = normalize(x[1], n[1]);
	ret['z'] = 0.0f;
	return ret;
}

const Color white(1.0f), gray(0.5f), black(0.0f);

class Grid {
public:
	vector<vector<Color>> data;
	Grid() {}
	Grid(int n, int m) {
		for (int i = 0; i < n; i++) {
			data.push_back(vector<Color>());
			for (int j = 0; j < m; j++) {
				data[i].push_back((i % 2 == j % 2) ? white : black);
			}
		}
	}

	void render() {
		static const int i_corners[4][2] = {
			{ 0, 0 },
			{ 0, 1 },
			{ 1, 1 },
			{ 1, 0 }
		};

		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data[i].size(); j++) {
				for (int c = 0; c < 4; c++) {
					data[i][j].render();
					normalize(
						vector<int>{ i + i_corners[c][0], j + i_corners[c][1] }, 
						vector<int>{ (int)data.size(), (int)data[i].size() }
					).render();
				}
			}
		}
	}
};

Grid *mygrid;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);

	mygrid->render();

	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.6f);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void timer(int te)
{
	tick += 0.5;
	while (tick >= 360.0)
		tick -= 360.0;
	glutPostRedisplay(); // Redraw screen with new object data.
	glutTimerFunc(10, timer, 1);//re-call in another 10ms
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	
	mygrid = new Grid(4, 4);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(10, timer, 1);
	glutMainLoop();
	
	delete mygrid;

	return 0;
}
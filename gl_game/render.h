#ifndef _RENDER_H_
#define _RENDER_H_

#include "main.h"
#include "grid.h"
#include "color.h"
#include "coord.h"

Color normalize(float foo) {
	//0.0f <= foo <= 1.0f
	return Color(foo, 0.0f, 0.0f);
}

void render(const Color& mycolor) {
	glColor3f(mycolor.r, mycolor.g, mycolor.b);
}

void render(Coord& mycoord) {
	glVertex2f(mycoord['x'], mycoord['y']);
}

class Square {
public:
	float length;
	Coord center;

	bool within(Coord& point) {
		return (
			point['x'] >= center['x'] - length / 2.0f &&
			point['x'] <= center['x'] + length / 2.0f &&
			point['y'] >= center['y'] - length / 2.0f &&
			point['y'] <= center['y'] + length / 2.0f
		);
	}
};

void render(Square& mysquare, bool textured = false) {
	float mag = mysquare.length / sqrt(2);
	if (textured) glTexCoord2f(0.0f, 0.0f); render(get_vector(mysquare.center,  45.0f, mag));
	if (textured) glTexCoord2f(0.0f, 1.0f); render(get_vector(mysquare.center, 135.0f, mag));
	if (textured) glTexCoord2f(1.0f, 1.0f); render(get_vector(mysquare.center, 225.0f, mag));
	if (textured) glTexCoord2f(1.0f, 0.0f); render(get_vector(mysquare.center, 315.0f, mag));
}

void render(Grid& mygrid, Coord& mouse, GLuint image) {
	for (int i = 0; i < mygrid.m; i++) {
		for (int j = 0; j < mygrid.m; j++) {
			Color mycolor(*(mygrid.root->find(i, j)->data), 0.0f, 0.0f);
			Square mysquare;
			mysquare.center = Coord(new float[2]{//-1.0 -> 1.0
				2.0f * (((float)i + 0.5f) / (float)mygrid.m) - 1.0f,
				-2.0f * (((float)j + 0.5f) / (float)mygrid.m) + 1.0f,
			});
			mysquare.length = 2.0f / (float)mygrid.m;

			bool tex = mysquare.within(mouse);

			if (tex) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, image);
			}
			glBegin(GL_QUADS);
			render(mycolor);
			render(mysquare, tex);
			glEnd();
			if (tex)
				glDisable(GL_TEXTURE_2D);

		}
	}

	glPointSize(4.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(mouse['x'], mouse['y']);
	glEnd();
}

void tex_test(GLuint image) {
	static float theta = 0.0f;
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// setup texture mapping
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, image);

	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(-0.5, -0.5);
	glTexCoord2d(1.0, 0.0); glVertex2d(+0.5, -0.5);
	glTexCoord2d(1.0, 1.0); glVertex2d(+0.5, +0.5);
	glTexCoord2d(0.0, 1.0); glVertex2d(-0.5, +0.5);
	glEnd();
	
	glutSwapBuffers();

	theta += 1.0f;
}

void render(GLuint *font, char *str, Coord& chr_size, Coord& origin) {
	Coord off(origin);
	while (*str) {
		if (*str == '\n') {
			off['x'] = origin['x'];
			off['y'] += chr_size['y'];
		}
		else {
			glEnable(GL_TEXTURE_2D);
			//glBindTexture(GL_TEXTURE_2D, font[*str]);
			glColor3f(1.0f, 1.0f, 1.0f);
			glBindTexture(GL_TEXTURE_2D, *font);
			
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 0.0); glVertex2d(off['x'], off['y']);
			glTexCoord2d(1.0, 0.0); glVertex2d(off['x'] + chr_size['x'], off['y']);
			glTexCoord2d(1.0, 1.0); glVertex2d(off['x'] + chr_size['x'], off['y'] + chr_size['y']);
			glTexCoord2d(0.0, 1.0); glVertex2d(off['x'], off['y'] + chr_size['y']);
			glEnd();

			off['x'] += chr_size['x'];
		}
	}
}

#endif
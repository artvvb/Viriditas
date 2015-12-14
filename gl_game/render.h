#ifndef _RENDER_H_
#define _RENDER_H_

#include "main.h"
#include "grid.h"
#include "color.h"
#include "coord.h"
#include "font.h"
#include "square.h"

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

void render(GLuint tex, Coord& size, Coord& origin) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2d(0.0, 0.0); glVertex2f(origin['x'], origin['y']);
	glTexCoord2d(1.0, 0.0); glVertex2f(origin['x'] + size['x'], origin['y']);
	glTexCoord2d(1.0, 1.0); glVertex2f(origin['x'] + size['x'], origin['y'] - size['y']);
	glTexCoord2d(0.0, 1.0); glVertex2f(origin['x'], origin['y'] - size['y']);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void render(Color& color, Coord& size, Coord& origin) {
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	render(color);
	glVertex2f(origin['x'], origin['y']);
	glVertex2f(origin['x'] + size['x'], origin['y']);
	glVertex2f(origin['x'] + size['x'], origin['y'] - size['y']);
	glVertex2f(origin['x'], origin['y'] - size['y']);
	glEnd();
}

void render(Square& mysquare, bool textured = false) {
	float mag = mysquare.length / sqrt(2);
	Coord origin = get_vector(mysquare.center, 135.0f, mag);
	
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
			if (tex) glDisable(GL_TEXTURE_2D);
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

//renders a rectangle of size size with upper left corner at origin

void render(GLuint texture, char *str, Coord& size, Coord& origin) {
	Coord off(origin);
	while (*str) {
		if (*str == '\n') {
			off['x'] = origin['x'];
			off['y'] -= size['y'];
		}
		else {
			render(texture, size, off);
			off['x'] += size['x'];
		}
		str++;
	}
}

void render(Font& font, char *str, Coord& size, Coord& origin) {
	Coord off(origin);
	while (*str) {
		if (*str == '\n') {
			//should probably process rectangles to fit the size of the character
			off['x'] = origin['x'];
			off['y'] -= size['y'];
		}
		else {
			render(font[*str], size, off);
			off['x'] += size['x'];
		}
		str++;
	}
}

/*/

void render_text(const char *text, float x, float y, float sx, float sy) {
	const char *p;

	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);

		float x2 = x + face->glyph->bitmap_left * sx;
		float y2 = -y - face->glyph->bitmap_top * sy;
		float w = face->glyph->bitmap.width * sx;
		float h = face->glyph->bitmap.rows * sy;

		GLfloat box[4][4] = {//X, Y, TexX, TexY?
			{ x2, -y2, 0.0f, 0.0f },
			{ x2 + w, -y2, 1.0f, 0.0f },
			{ x2, -y2 - h, 0.0f, 1.0f },
			{ x2 + w, -y2 - h, 1.0f, 1.0f },
		};

		{//do nothing brackets for clarity
			//pure gl version
			glBegin(GL_QUADS);
			for (int i = 0; i < 4; i++) {
				glTexCoord2f(box[i][2], box[i][3]);
				glVertex2f(box[i][0], box[i][1]);
			}
			glEnd();

			//shader call version
			//glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
			//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		x += (face->glyph->advance.x >> 6) * sx;
		y += (face->glyph->advance.y >> 6) * sy;
	}
}

void display_text_test() {//display() already exists in main, to run this code, add a call to this function to that display function
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//GLfloat black[4] = { 0, 0, 0, 1 };//rgba
	//glUniform4fv(uniform_color, 1, black);

	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

	render(black);
	render_text("The Quick Brown Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
	render_text("The Misaligned Fox Jumps Over The Lazy Dog", -1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);

	glutSwapBuffers();
}

/**/
#endif
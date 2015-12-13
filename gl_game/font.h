#ifndef _FONT_H_
#define _FONT_H_

#include "main.h"
#include <ft2build.h>
#include FT_FREETYPE_H
//http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
//http://www.freetype.org/freetype2/docs/tutorial/step1.html
void make_tex_from_ttf(GLuint font[256], char *filename) {
	FT_Library  library;
	FT_Face     face;
	int error;
	if (FT_Init_FreeType(&library)) {
		cout << "Freetype library initialization failed" << endl;
	}
	error = FT_New_Face(library, filename, 0, &face);
	if (error == FT_Err_Unknown_File_Format) {
		cout << "Font file format unsupported" << endl;
	}
	else if (error) {
		cout << "Failed to open font file" << endl;
	}

	error = FT_Set_Char_Size(
		face,		/* handle to face object           */
		0,			/* char_width in 1/64th of points  */
		16 * 64,	/* char_height in 1/64th of points */
		800,		/* horizontal device resolution    */
		800			/* vertical device resolution      */
	);

	FT_UInt glyph_index = FT_Get_Char_Index(face, '0');
	FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
	//FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	FT_Bitmap* bitmap = &face->glyph->bitmap;
	Tex zero (
		(unsigned char*)bitmap->buffer,
		(int)bitmap->width,
		(int)bitmap->rows
	);

	cout << "'0': width = " << bitmap->width << endl;
	cout << "'0': rows  = " << bitmap->rows  << endl;

	font['0'] = zero.texture;
}


FT_Library ft;
FT_Face face;
GLuint tex;

GLuint font_init() {
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return 1;
	}

	if (FT_New_Face(ft, "Consolas.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		return 1;
	}

	FT_Set_Pixel_Sizes(face, 0, 12);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		fprintf(stderr, "Could not load character 'X'\n");
		return 1;
	}

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void render_text(const char *text, float x, float y, float sx, float sy) {
	const char *p;

	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
			);

		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{ x2,     -y2    , 0, 0 },
			{ x2 + w, -y2    , 1, 0 },
			{ x2,     -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}
}

void display() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat black[4] = { 0, 0, 0, 1 };
	glUniform4fv(uniform_color, 1, black);

	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

	render_text("The Quick Brown Fox Jumps Over The Lazy Dog",
		-1 + 8 * sx, 1 - 50 * sy, sx, sy);
	render_text("The Misaligned Fox Jumps Over The Lazy Dog",
		-1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);

	glutSwapBuffers();
}

#endif
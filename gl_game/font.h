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


#endif
#ifndef _FONT_H_
#define _FONT_H_

#include "main.h"
#include "tex.h"
#include "fastmath.h"

#include <ft2build.h>
#include FT_FREETYPE_H

//http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
//http://www.freetype.org/freetype2/docs/tutorial/step1.html

//THIS
//https://axfive.wordpress.com/2012/01/17/freetype-texture-fonts-in-opengl/

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
	FT_Set_Pixel_Sizes(
		face,
		64,
		64
		);
	/*/
	FT_Set_Char_Size(
		face,		// handle to face object          
		0,			// char_width in 1/64th of points 
		16 * 64,	// char_height in 1/64th of points
		800,		// horizontal device resolution   
		800			// vertical device resolution     
	);
	/**/
	FT_UInt glyph_index = FT_Get_Char_Index(face, '0');
	FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
	if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);//8 bit greyscale

	//FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	FT_Bitmap* bitmap = &face->glyph->bitmap;
	cout << "'0': width = " << bitmap->width << endl;
	cout << "'0': rows  = " << bitmap->rows  << endl;

	Tex zero (
		(unsigned char*)bitmap->buffer,
		(int)bitmap->width,//
		(int)bitmap->rows,
		1
	);

	font['0'] = zero.texture;
}

class Font {
public:
	FT_Library  library;

	FT_Face     face;
	GLuint *textures;//starting with numeric only
	char *typeface;
	int size;

	bool valid;
	int width, height;

	int freetype_init(char *filename) {
		int error;

		error = FT_Init_FreeType(&library);
		if (error) {
			cout << "Freetype library initialization failed" << endl;
			return error;
		}

		error = FT_New_Face(library, filename, 0, &face);
		if (error == FT_Err_Unknown_File_Format) {
			cout << "Font file format unsupported" << endl;
			return error;
		}
		else if (error) {
			cout << "Failed to open font file" << endl;
			return error;
		}
		
		return 0;
	}

	Font(char *filename, char *_typeface) {
		valid = false;

		if (freetype_init(filename))
			return;
		
		FT_Set_Pixel_Sizes(face, 64, 0);

		size = strlen(_typeface);
		typeface = new char[size];
		for (int i = 0; i < size; i++)
			typeface[i] = _typeface[i];
		textures = new GLuint[size];
		glGenTextures(size, textures);

		for (int i = 0; i < size; i++) {
			char c = typeface[i];
			FT_UInt glyph_index = FT_Get_Char_Index(face, c);
			FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);

			if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
				FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);//8 bit greyscale

			FT_Bitmap* bitmap = &face->glyph->bitmap;
			cout << c << ": width = " << bitmap->width << endl;
			cout << c << ": rows  = " << bitmap->rows << endl;

			width = next_p2(bitmap->width, 8);
			height = next_p2(bitmap->rows,  8);
			
			//PAD THE BUFFER TO FIT 2^N x 2^N
			unsigned char *buffer = new unsigned char[3 * width * height];//default white
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++){
					if (y < bitmap->rows && x < bitmap->width) {
						buffer[3 * (y * width + x) + 0] = 
						buffer[3 * (y * width + x) + 1] = 
						buffer[3 * (y * width + x) + 2] = bitmap->buffer[y * bitmap->width + x];
					}
					else {
						buffer[3 * (y * width + x) + 0] =
						buffer[3 * (y * width + x) + 1] =
						buffer[3 * (y * width + x) + 2] = 0x00;
					}
					/*/
					if (buffer[4 * (y * width + x) + 0] == 0x00) {
						buffer[4 * (y * width + x) + 3] = 0x7f;
					}
					else {
						buffer[4 * (y * width + x) + 3] = 0xff;
					}
					/**/
				}
			}

			cout << "after padding:" << endl;
			cout << c << ": width  = " << width  << endl;
			cout << c << ": height = " << height << endl;

			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//pad buffer to size 2^n?
			
			int error = gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
			cout << gluErrorString(error) << endl;
			cout << textures[i] << endl;
		}
		valid = true;
	}

	~Font() {
		glDeleteTextures(10, textures);
	}

	GLuint& operator[] (char& c) {
		for (int i = 0; i < size; i++) {
			if (c == typeface[i])
				return textures[i];
		}
		return textures[0];
	}
};


/*/
FT_Library ft;
FT_Face face;
GLuint tex;
/**/
/*/
GLuint font_init(char *filename) {
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return 1;
	}

	if (FT_New_Face(ft, filename, 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		return 1;
	}

	FT_Set_Pixel_Sizes(face, 0, 12);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		fprintf(stderr, "Could not load character 'X'\n");
		return 1;
	}
	//Tex mytex(buf, width, height); creates a mipmap(?) texture from a character buffer of RGBRGB... values

	//this implementation might require GLEW
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//

	GLuint vbo;//vertex buffer object is a reference to an input stream to a shader, iirc
	glGenBuffers(1, &vbo);
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
}
/**//**/

//moved text rendering functions to render.h
/**/
#endif
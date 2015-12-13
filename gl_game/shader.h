#ifndef _SHADER_H_
#define _SHADER_H_

#include "main.h"


/*/
void setShaders() {

	char *vs = NULL, *fs = NULL, *fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	f2 = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("minimal.vert");
	fs = textFileRead("minimal.frag");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShader(v);
	glCompileShader(f);

	printShaderInfoLog(v);
	printShaderInfoLog(f);
	printShaderInfoLog(f2);

	p = glCreateProgram();
	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgram(p);
	printProgramInfoLog(p);

	glUseProgram(p);

}
/**/
/*/
static GLuint make_shader(GLenum type, const char *filename)
{
GLint length;
GLchar *source = file_contents(filename, &length);
GLuint shader;
GLint shader_ok;

if (!source)
return 0;

shader = glCreateShader(type);
glShaderSource(shader, 1, (const GLchar**)&source, &length);
free(source);
glCompileShader(shader);

glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
if (!shader_ok) {
cout << "Failed to compile filename" << endl;
//		show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
glDeleteShader(shader);
return 0;
}
return shader;
}
/**/

#endif
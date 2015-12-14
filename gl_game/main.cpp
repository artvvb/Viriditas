#include "main.h"
#include "coord.h"
#include "color.h"
#include "grid.h"
#include "render.h"
#include "shader.h"
#include "bmp.h"
#include "font.h"
#include "glinit.h"

float tick = 0.0f, last_mouse_time = 0.0f;

#define MYN 3

Coord myorigin(new float[3]{0.0, 0.0, 0.0});

Grid *mygrid;

int window[2] = { 800, 800 };//wisth, height
Coord mouse = Coord(new float[2]{0.0f, 0.0f});
const float fov = 90.0;
float aspect = (GLfloat)(window[0]) / (GLfloat)(window[1]);
const float depth = -1.0f;

Coord get_position(int x, int y) {
	//return position of the mouse in world coordinates

	return Coord(new float[2]{
		aspect * (2.0f * (float)x / (float)window[0] - 1.0f),//equivalent of * aspect / width
		-1.0f * (2.0f * (float)y / (float)window[1] - 1.0f)
	});
}

GLuint image;
Font *font;

void display(void)
{
	static unsigned int last_time = (unsigned)clock();
	unsigned int current_time = (unsigned)clock();
	float fps = (float)CLOCKS_PER_SEC / (float)(current_time - last_time);
	last_time = current_time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 1.0);

	char fps_str[7];
	sprintf_s(fps_str, "%.2f", fps);
	//cout << fps << endl;

	render(*mygrid, mouse, image);

	/**/
	if (last_mouse_time + 1000.0f < tick) {
		char *str = "01\n23\n";
		render(*font, str, Coord(0.1f, 0.1f), mouse);
	}
	/**/

	//display_text_test();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	window[0] = w;
	window[1] = h;
	aspect = (GLfloat)(window[0]) / (GLfloat)(window[1]);

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, 0.0, 30.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, depth);
}

void keyboard(unsigned char key, int x, int y)
{
	
	int mod = glutGetModifiers();
	switch (key) {
	case 27:
		exit(0);
		break;
	case ' ':
		if (mod == GLUT_ACTIVE_SHIFT) {
			delete mygrid;
			mygrid = new Grid(MYN);
		}
		else
			mygrid->ds_insert();
		break;
	default:
		break;
	}
}

void timer(int te)
{
	tick += 10.0;//ms
	glutPostRedisplay();//redraw
	glutTimerFunc(10, timer, 1);//re-call in another 10ms
	return;
}

void passive_mouse(int x, int y) {
	mouse = get_position(x, y);
	last_mouse_time = tick;
}

int main(int argc, char** argv)
{
	last_mouse_time = (int)time(NULL);
	srand((unsigned)time(NULL));
	
	gl_init(argc, argv, window[0], window[1], 0, 0);

	cout << "version = " << glGetString(GL_VERSION) << endl;

	image = make_tex_from_bmp("sqr.bmp");
	font = new Font("Consolas.ttf");
	if (font->valid == false) cout << "Font initialized incorrectly" << endl;
	//font_init("Consolas.ttf");
	//make_tex_from_ttf(font, "Consolas.ttf");

	mygrid = new Grid(MYN);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(10, timer, 1);
	glutPassiveMotionFunc(passive_mouse);
	
	glutMainLoop();
	delete mygrid;

	return 0;
}
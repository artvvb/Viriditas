#include "main.h"
#include "coord.h"
#include "color.h"
#include "grid.h"
#include "render.h"
#include "shader.h"
#include "bmp.h"

float tick = 0.0f, last_mouse_time = 0.0f;

#define MYN 3

Coord myorigin(new float[3]{0.0, 0.0, 0.0});

const Color white(1.0f), gray(0.5f), black(0.0f);

Grid *mygrid;

int window_shape[2] = { 800, 800 };
Coord mouse = Coord(new float[2]{0.0f, 0.0f});
const float fov = 90.0;
float aspect = (GLfloat)(window_shape[0]) / (GLfloat)(window_shape[1]);
const float depth = -1.0f;

Coord get_position(int x, int y) {
	//return position of the mouse in world coordinates

	return Coord(new float[2]{
		2.0f * (float)x / (float)window_shape[0] - 1.0f,
		-1.0f * (2.0f * (float)y / (float)window_shape[1] - 1.0f)
	});
}

GLuint image;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 1.0);

	//tex_test(image);
	/**/
	render(*mygrid, mouse, image);
	
	if (last_mouse_time + 1000.0f < tick) {
		glBegin(GL_QUADS);
		//glColor3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);		glVertex2f(mouse['x'],			mouse['y']			);
		glTexCoord2f(0.0f, 1.0f);		glVertex2f(mouse['x'],			mouse['y'] - 0.1f	);
		glTexCoord2f(1.0f, 1.0f);		glVertex2f(mouse['x'] + 0.1f,	mouse['y'] - 0.1f	);
		glTexCoord2f(1.0f, 0.0f);		glVertex2f(mouse['x'] + 0.1f,	mouse['y']			);
		glEnd();
	}
	/**/

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	window_shape[0] = w;
	window_shape[1] = h;
	aspect = (GLfloat)(window_shape[0]) / (GLfloat)(window_shape[1]);

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

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_shape[0], window_shape[1]);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);

	cout << "version = " << glGetString(GL_VERSION) << endl;

	image = loadBMP_custom("sqr.bmp");
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
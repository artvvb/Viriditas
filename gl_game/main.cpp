#include "main.h"
#include "coord.h"
#include "color.h"
#include "grid.h"
#include "render.h"

float tick = 0.0;

#define MYN 3

Coord myorigin(new float[3]{0.0, 0.0, 0.0});

const Color white(1.0f), gray(0.5f), black(0.0f);

Grid *mygrid;

int window_shape[2] = { 800, 800 };
float mouse_pos[2] = { 0.0, 0.0 };
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

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 1.0);

	render(*mygrid, get_position(mouse_pos[0], mouse_pos[1]));

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
	tick += 0.5;
	while (tick >= 360.0)
		tick -= 360.0;
	glutPostRedisplay(); // Redraw screen with new object data.
	glutTimerFunc(10, timer, 1);//re-call in another 10ms
}

void passive_mouse(int x, int y) {
	mouse_pos[0] = x;
	mouse_pos[1] = y;
}

int main(int argc, char** argv)
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_shape[0], window_shape[1]);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);

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
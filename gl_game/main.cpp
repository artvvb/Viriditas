#include "main.h"
#include "coord.h"
#include "color.h"
#include "link2.h"

float tick = 0.0;

Coord corners[4] = {
	Coord(new float[3]{0.0, 0.0, 0.0}),
	Coord(new float[3]{0.0, 1.0, 0.0}),
	Coord(new float[3]{1.0, 1.0, 0.0}),
	Coord(new float[3]{1.0, 0.0, 0.0}),
};

Coord myorigin(new float[3]{0.0, 0.0, 0.0});

const Color white(1.0f), gray(0.5f), black(0.0f);

class Grid {
public:
	Link2<Color> *root;
	int n;

	Grid() {
		n = 2;

		root = new Link2<Color>(white);

		root->adj[L2_RIGHT] = new Link2<Color>(black);
		root->adj[L2_RIGHT]->adj[L2_LEFT] = root;
		root->adj[L2_RIGHT]->adj[L2_DOWN] = new Link2<Color>(white);
		root->adj[L2_RIGHT]->adj[L2_DOWN]->adj[L2_UP] = root->adj[L2_RIGHT];

		root->adj[L2_DOWN] = new Link2<Color>(black);
		root->adj[L2_DOWN]->adj[L2_UP] = root;
		root->adj[L2_DOWN]->adj[L2_RIGHT] = root->adj[L2_RIGHT]->adj[L2_DOWN];
		
		root->adj[L2_RIGHT]->adj[L2_DOWN] = root->adj[L2_DOWN]->adj[L2_RIGHT];
	}
	void render() {
		static const int i_corners[4][2] = {
			{ 0, 0 },
			{ 0, 1 },
			{ 1, 1 },
			{ 1, 0 }
		};
		glBegin(GL_QUADS);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				for (int c = 0; c < 4; c++) {
					root->find(i, j).render();
					normalize(
						vector<int>{ i + i_corners[c][0], j + i_corners[c][1] }, 
						vector<int>{ n, n }
					).render();
				}
			}
		}
		glEnd();
	}

};

Grid *mygrid;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 1.0);

	mygrid->render();
	
	glutSwapBuffers();
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
	
	mygrid = new Grid();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(10, timer, 1);
	glutMainLoop();
	
	delete mygrid;

	return 0;
}
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
	int n, m;

	Grid() {
		n = 2;
		m = 2;

		root = new Link2<Color>(white);

		root->insert(L2_RIGHT, white);
		root->insert(L2_DOWN, white);
		root->adj[L2_RIGHT]->insert(L2_DOWN, white);
		root->adj[L2_DOWN]->connect(L2_RIGHT, root->adj[L2_RIGHT]->adj[L2_DOWN]);
	}

	void insert_horizontal() {
		Link2<Color> *trace = root;
		while (trace != NULL && trace->adj[L2_RIGHT] != NULL) {
			cout << "check" << endl;
			trace->insert_col(black);
			cout << "check" << endl;
			trace = trace->adj[L2_RIGHT]->adj[L2_RIGHT];
		}
	}

	void insert_vertical() {
		cout << "WIDTH: " << root->get_width() << endl;
		for (int i = 0; i < 2 * (n - 1) + 1; i++) {
			for (int j = 0; j < n - 1; j += 1) {
				root->find(i, 0)->insert(L2_DOWN, black);
			}
		}
	}

	void ds_insert() {
		root->insert_row(black);
		root->insert_col(black);
		m++; n++;
		//insert_horizontal();
		//n += n - 1;
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
			for (int j = 0; j < m; j++) {
				for (int c = 0; c < 4; c++) {
					root->find(i, j)->data->render();
					normalize(
						vector<int>{ i + i_corners[c][0], j + i_corners[c][1] }, 
						vector<int>{ n, m }
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
	case ' ':
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
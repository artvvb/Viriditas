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

Color normalize(float foo) {
	//0.0f <= foo <= 1.0f
	return Color(foo, 0.0f, 0.0f);
	//return Color(foo);
}

class Grid {
public:
	Link2<float> *root;
	int n;

	//corners are initialized to 0.0f <= data < 1.0f randomly
	Grid() {
		n = 2;
		root = new Link2<float>((float)(rand() % 256) / 256.0f);
		root->insert(L2_RIGHT, (float)(rand() % 256) / 256.0f);
		root->insert(L2_DOWN,  (float)(rand() % 256) / 256.0f);
		root->adj[L2_RIGHT]->insert(L2_DOWN, (float)(rand() % 256) / 256.0f);
		root->adj[L2_DOWN]->connect(L2_RIGHT, root->adj[L2_RIGHT]->adj[L2_DOWN]);
		grid_normalize();
	}

	void insert_horizontal() {
		Link2<float> *trace = root->adj[L2_DOWN];
		while (trace != NULL) {
			trace->adj[L2_UP]->insert_row(0.0f);
			trace = trace->adj[L2_DOWN];
		}
	}

	void insert_vertical() {
		Link2<float> *trace = root->adj[L2_RIGHT];
		while (trace != NULL) {
			trace->adj[L2_LEFT]->insert_col(0.0f);
			trace = trace->adj[L2_RIGHT];
		}
	}

	//accounts for accidental overshoot of the 0.0 -> 1.0 range by addition of the random poll element
	void grid_normalize() {
		float max, min;
		Link2<float> *trace[2] = { root, NULL };

		max = min = *root->data;
		while (trace[0] != NULL) {
			trace[1] = trace[0];
			while (trace[1] != NULL) {
				if (max < *trace[1]->data) max = *trace[1]->data;
				if (min > *trace[1]->data) min = *trace[1]->data;
				trace[1] = trace[1]->adj[L2_DOWN];
			}
			trace[0] = trace[0]->adj[L2_RIGHT];
		}
		trace[0] = root;
		trace[1] = NULL;
		while (trace[0] != NULL) {
			trace[1] = trace[0];
			while (trace[1] != NULL) {
				*trace[1]->data -= min;
				*trace[1]->data /= max - min;
				trace[1] = trace[1]->adj[L2_DOWN];
			}
			trace[0] = trace[0]->adj[L2_RIGHT];
		}
	}

	void diamond_poll() {
		Link2<float> *trace[2] = { root->adj[L2_RIGHT]->adj[L2_DOWN], NULL };
		while (trace[0] != NULL) {
			trace[1] = trace[0];
			while (trace[1] != NULL) {
				trace[1]->diamond_poll(256 - n);
				trace[1] = trace[1]->adj[L2_DOWN]->adj[L2_DOWN];
			}
			trace[0] = trace[0]->adj[L2_RIGHT]->adj[L2_RIGHT];
		}
	}
	void hor_square_poll() {
		Link2<float> *trace[2] = { root->adj[L2_RIGHT], NULL };
		while (trace[0] != NULL) {
			trace[1] = trace[0];
			while (trace[1] != NULL) {
				trace[1]->square_poll(256 - n);
				if (trace[1]->adj[L2_DOWN] != NULL)
					trace[1] = trace[1]->adj[L2_DOWN]->adj[L2_DOWN];
				else
					trace[1] = NULL;
			}
			trace[0] = trace[0]->adj[L2_RIGHT]->adj[L2_RIGHT];
		}
	}
	void ver_square_poll() {
		Link2<float> *trace[2] = { root->adj[L2_DOWN], NULL };
		while (trace[0] != NULL) {
			trace[1] = trace[0];
			while (trace[1] != NULL) {
				trace[1]->square_poll(256 - n);
				trace[1] = trace[1]->adj[L2_DOWN]->adj[L2_DOWN];
			}
			if (trace[0]->adj[L2_RIGHT] != NULL)
				trace[0] = trace[0]->adj[L2_RIGHT]->adj[L2_RIGHT];
			else
				trace[0] = NULL;
		}
	}

	void ds_insert() {
		//root->insert_row(0.0f);
		//root->insert_col(0.0f);
		if (n < 65) {
			insert_horizontal();
			insert_vertical();
			diamond_poll();
			hor_square_poll();
			ver_square_poll();
			grid_normalize();
			n = 2 * n - 1;
		}
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
			for (int j = 0; j < n; j++) {
				for (int c = 0; c < 4; c++) {
					normalize(*(root->find(i, j)->data)).render();
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
	
	int mod = glutGetModifiers();
	switch (key) {
	case 27:
		exit(0);
		break;
	case ' ':
		if (mod == GLUT_ACTIVE_SHIFT) {
			delete mygrid;
			mygrid = new Grid();
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

int main(int argc, char** argv)
{
	srand((unsigned)time(NULL));

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
#ifndef _GRID_H_
#define _GRID_H_

#include "main.h"
#include "link2.h"

class Grid {
public:
	Link2<float> *root;
	int n, m;

	//corners are initialized to 0.0f <= data < 1.0f randomly
	Grid(int _n) {
		root = new Link2<float>(0.0f);
		root->insert(L2_RIGHT, 0.0f);
		root->insert(L2_DOWN,  0.0f);
		root->adj[L2_RIGHT]->insert(L2_DOWN, 0.0f);
		root->adj[L2_DOWN]->connect(L2_RIGHT, root->adj[L2_RIGHT]->adj[L2_DOWN]);

		m = 2;
		for (n = 0; n < _n; )
			insert();
		randomize();
	}

	void randomize() {
		Link2<float> *trace[2] = { root, NULL };
		while (trace[0] != NULL) {
			trace[1] = trace[0];
			while (trace[1] != NULL) {
				*trace[1]->data = (float)(rand() % 1024);
				trace[1] = trace[1]->adj[L2_DOWN];
			}
			trace[0] = trace[0]->adj[L2_RIGHT];
		}
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
	void insert() {
		if (n < 6) {//severe performance issues after around this point
			insert_horizontal();
			insert_vertical();
			cout << m << " : ";
			m = 2 * m - 1;
			cout << m << " (" << n << " : ";
			n++;
			cout << n << ")" << endl;
		}
	}
	void ds_insert() {
		if (n < 6) {//severe performance issues after around this point
//			insert_horizontal();
//			insert_vertical();
			insert();
			diamond_poll();
			hor_square_poll();
			ver_square_poll();
			grid_normalize();
//			m = 2 * m - 1;
//			n++;
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
				trace[1]->diamond_poll(256 - m);
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
				trace[1]->square_poll(256 - m);
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
				trace[1]->square_poll(256 - m);
				trace[1] = trace[1]->adj[L2_DOWN]->adj[L2_DOWN];
			}
			if (trace[0]->adj[L2_RIGHT] != NULL)
				trace[0] = trace[0]->adj[L2_RIGHT]->adj[L2_RIGHT];
			else
				trace[0] = NULL;
		}
	}
};


#endif
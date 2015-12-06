#ifndef _RENDER_H_
#define _RENDER_H_

#include "main.h"
#include "grid.h"
#include "color.h"
#include "coord.h"

/**/
Coord corners[4] = {
	Coord(new float[3]{0.0, 0.0, 0.0}),
	Coord(new float[3]{0.0, 1.0, 0.0}),
	Coord(new float[3]{1.0, 1.0, 0.0}),
	Coord(new float[3]{1.0, 0.0, 0.0}),
};
/**/

Color normalize(float foo) {
	//0.0f <= foo <= 1.0f
	return Color(foo, 0.0f, 0.0f);
	//return Color(foo);
}

void render(const Color& mycolor) {
	glColor3f(mycolor.r, mycolor.g, mycolor.b);
}
void render(Grid& mygrid) {
	static const int i_corners[4][2] = {
		{ 0, 0 },
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 }
	};



	glBegin(GL_QUADS);
	for (int i = 0; i < mygrid.m; i++) {
		for (int j = 0; j < mygrid.m; j++) {
			for (int c = 0; c < 4; c++) {
				render(normalize(*(mygrid.root->find(i, j)->data)));
				normalize(
					vector<int>{ i + i_corners[c][0], j + i_corners[c][1] },
					vector<int>{ mygrid.m, mygrid.m }
				).render();
			}
		}
	}
	glEnd();
}
#endif
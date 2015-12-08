#ifndef _RENDER_H_
#define _RENDER_H_

#include "main.h"
#include "grid.h"
#include "color.h"
#include "coord.h"

Color normalize(float foo) {
	//0.0f <= foo <= 1.0f
	return Color(foo, 0.0f, 0.0f);
}

void render(const Color& mycolor) {
	glColor3f(mycolor.r, mycolor.g, mycolor.b);
}

void render(Coord& mycoord) {
	glVertex2f(mycoord['x'], mycoord['y']);
}

class Square {
public:
	float length;
	Coord center;

	bool within(Coord& point) {
		return (
			point['x'] >= center['x'] - length / 2.0f &&
			point['x'] <= center['x'] + length / 2.0f &&
			point['y'] >= center['y'] - length / 2.0f &&
			point['y'] <= center['y'] + length / 2.0f
			);
	}
};

void render(Square& mysquare) {
	for (int i = 0; i < 4; i++)
		render(get_vector(mysquare.center, 45.0f + 90.0f * i, mysquare.length / sqrt(2)));
}

void render(Grid& mygrid, Coord& mouse) {
	static const int corners[4][2] = {
		{ 0, 0 },
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 }
	};

	glBegin(GL_QUADS);
	for (int i = 0; i < mygrid.m; i++) {
		for (int j = 0; j < mygrid.m; j++) {
			Color mycolor(*(mygrid.root->find(i, j)->data), 0.0f, 0.0f);
			Square mysquare;
			mysquare.center = Coord(new float[2]{//-1.0 -> 1.0
				2.0f * (((float)i + 0.5f) / (float)mygrid.m) - 1.0f,
				2.0f * (((float)j + 0.5f) / (float)mygrid.m) - 1.0f,
			});
			mysquare.length = 2.0f / (float)mygrid.m;
			if (mysquare.within(mouse))
				render(Color(0.0f, 0.0f, 1.0f));
			else
				render(mycolor);
			render(mysquare);
		}
	}
	glEnd();

	glPointSize(4.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(mouse['x'], mouse['y']);
	glEnd();
}

#endif
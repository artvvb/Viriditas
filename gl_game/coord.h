#ifndef _COORD_H_
#define _COORD_H_
#include "main.h"

class Coord {
protected:
	float x, y, z;
public:
	Coord() {}
	Coord(float arr[]) : x(arr[0]), y(arr[1]), z(arr[2]) {}

	float& operator[] (char index) {
		switch (index) {
		case 'x':
		case 'X': return x;
		case 'y':
		case 'Y': return y;
		case 'z':
		case 'Z': return z;
		}
	}
	void render() {
		glVertex3f(x, y, z);
	}
};

Coord get_vector(Coord& origin, const float& angle, const float& magnitude) {
	Coord dest;
	dest['x'] = (magnitude * cos(angle * PI / 180.0f) + origin['x']);
	dest['y'] = (magnitude * sin(angle * PI / 180.0f) + origin['y']);
	dest['z'] = 0.0f;
	return dest;
}

#endif
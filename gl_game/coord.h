#ifndef _COORD_H_
#define _COORD_H_
#include "main.h"

class Coord {
protected:
	float x, y;
public:
	Coord() {}
	Coord(float arr[]) : x(arr[0]), y(arr[1]) {}

	float& operator[] (char index) {
		switch (index) {
		case 'x':
		case 'X': return x;
		case 'y':
		case 'Y': return y;
		}
	}
};

Coord get_vector(Coord& origin, const float& angle, const float& magnitude) {
	Coord dest;
	dest['x'] = (magnitude * cos(angle * PI / 180.0f) + origin['x']);
	dest['y'] = (magnitude * sin(angle * PI / 180.0f) + origin['y']);
	return dest;
}

float normalize(const float& x, const float& n) {
	return 2.0f * (x / n) - 1.0f;
}

Coord normalize(Coord& x, Coord& n) {
	Coord ret;
	ret['x'] = normalize(x['x'], n['x']);
	ret['y'] = normalize(x['y'], n['y']);
	return ret;
}

#endif
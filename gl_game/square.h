#ifndef _SQUARE_H_
#define _SQUARE_H_

#include "coord.h"

class Square {
public:
	float length;
	Coord center;

	Square() {

	}

	bool within(Coord& point) {
		return (
			point['x'] >= center['x'] - length / 2.0f &&
			point['x'] <= center['x'] + length / 2.0f &&
			point['y'] >= center['y'] - length / 2.0f &&
			point['y'] <= center['y'] + length / 2.0f
			);
	}
};

#endif
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

class Color {
public:
	float r, g, b;

	Color(const float gray = 0.0f) {
		r = g = b = gray;
	}
	Color(const Color& other) {
		r = other.r;
		g = other.g;
		b = other.b;
	}
	void render() const {
		glColor3f(r, g, b);
	}
};

#endif
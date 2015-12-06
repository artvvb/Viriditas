#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

class Color {
public:
	float r, g, b;

	Color(const float gray = 0.0f) :
		r(gray),
		g(gray),
		b(gray)
	{
	
	}
	Color(const float _r, const float _g, const float _b) : 
		r(_r),
		g(_g),
		b(_b)
	{
		
	}
	Color(const Color& other) {
		r = other.r;
		g = other.g;
		b = other.b;
	}
	
};

#endif
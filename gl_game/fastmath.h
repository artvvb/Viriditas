#ifndef _FASTMATH_H_
#define _FASTMATH_H_

//returns the next greatest power of two after foo
//pow(2, ceil(log2(foo)))
int next_p2(int foo, int n) {
	int p2 = 1;
	while (p2 < foo)
		p2 <<= 1;
	return p2;
}

#endif
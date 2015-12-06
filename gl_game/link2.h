#ifndef _LINK2_H_
#define _LINK2_H_

#include "main.h"

#define L2_LEFT 0
#define L2_RIGHT 1
#define L2_UP 2
#define L2_DOWN 3

template<class T>
class Link2 {
public:
	T *data;
	Link2<T> *adj[4];//LRUD

	Link2<T>() {
		for (int i = 0; i < 4; i++)
			adj[i] = NULL;
		data = NULL;
	}

	Link2<T>(const T& _data) : Link2<T>() {
		data = new T(_data);
	}

	~Link2<T>() {
		if (data != NULL) delete data;
		for (int i = 0; i < 4; i++) {
			if (adj[i] != NULL)
				adj[i]->adj[(i/2)*2 + (i%2 == 0 ? 1 : 0)];
				delete adj[i];
		}
	}

	T find(int i, int j) {
		if (i == 0) {
			if (j == 0)
				return *data;
			else
				return adj[L2_DOWN]->find(i, j - 1);
		}
		else
			return adj[L2_RIGHT]->find(i - 1, j);
	}
};

#endif
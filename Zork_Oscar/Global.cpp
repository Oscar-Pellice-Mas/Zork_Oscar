#include "Global.h"

int Roll(int min, int max) {
	return (max > 0) ? min + (rand() % (max - min)) : 0;
}
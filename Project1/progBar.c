#include <stdio.h>
#include "progBar.h"

static void progBar(long long goal, long long progress, int width) {
	char* full = "=";
	char* empty = "-";
	printf("\r[");
	for (int i = 0; i < width; i++) {
		if ((progress * progress * (long long) width) / (goal * goal) > i) {
			printf(full);
		}else {
			printf(empty);
		}
	}
	printf("] %lld/%lld",progress,goal);
}
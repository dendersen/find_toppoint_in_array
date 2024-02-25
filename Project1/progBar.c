#include <stdio.h>
#include "progBar.h"

static void progBar(long long goal, long long progress, int width) {
	char* full = "=";
	char* empty = "-";
	printf("\r[");
	int filled = progress % (goal/3);
	filled *= filled;
	filled += (progress / (goal / 3)) * (progress / (goal / 3));
	filled *= width;
	filled /= goal * goal * goal;
	for(int i = 0; i < filled; i++) {
		printf (full);
	}
	for(int i = filled; i < width; i++) {
		printf (empty);
	}
	printf("] %lld/%lld, %.2f%%",progress,goal, ((float)progress / (float)goal) * 100.0f);
}
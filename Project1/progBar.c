#include <stdio.h>
#include "progBar.h"

static void progBar(long long goal, long long progress, int width) {
	char* full = "=";
	char* empty = "-";
	printf("\r[");
	int filled = (progress * width) / goal;
	for(int i = 0; i < filled; i++) {
		printf (full);
	}
	for(int i = filled; i < width; i++) {
		printf (empty);
	}
	printf("] %lld/%lld, %.2f%%",progress,goal, ((float)progress / (float)goal) * 100.0f);
}
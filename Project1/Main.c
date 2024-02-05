#include <stdio.h>
#include <time.h>
#include "Test.h"
#include "progBar.h"

long long splitSearch(long long* arr, long long length);
long long maxValue(long long* arr, long long length);

#define tests 2
long long (*test[tests]) (long long*, long long) = { splitSearch,maxValue };
char* testFile[tests] = { "splitSearch_result.csv","maxValue_result.csv" };
int main() {
    long long dataPoints = 20;
    long long maxLength = 1000ll * 1000ll * 1000ll;
    for (int i = 0; i < tests; i++) {
        printf("\ntesting funktion: %d\n", i);
        allTest(test[i]);
        printf("\ntesting speed\n");
        long long* speed = speedTest(test[i], maxLength, dataPoints);
        if (speed == NULL) {
            printf("failed to alocate memory\n ending test early!");
            return 1;
        }
        printf("\n\nwriting result to file\n");
        FILE* file = fopen(testFile[i], "w");
        fprintf(file, "time, indices\n");
        for (long long j = 0; j < dataPoints; j++) {
            fprintf(file, "%lld, ", speed[j * 2]);
            fprintf(file, "%lld\n", speed[j * 2 + 1]);
        }
        free(speed);
        fclose(file);
    }
}

long long maxValue(long long* arr, long long length) {
    long long max = 0;
    for (long long i = 1; i < length; i++){
        if (arr[max] < arr[i]) max = i;
    }
    return max;
}

long long splitSearch(long long* arr, long long length) {
    long long width = 1, start = 0;
    while (width < length) {
        width = width << 1;
    }
    while (width > 1) {
        width /= 2;
        if (/*0+*/
            width + start + 1 < length &&
            arr[width + start] < 
            arr[width + start + 1]
        ) {
            start += width;
        }
    }
    return 
        arr[start] >=
        arr[start + 1] ? 
        start : start+1;
}
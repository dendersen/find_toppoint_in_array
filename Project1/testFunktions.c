#include "testFunktions.h"

long long (*toBeTested [tests]) (long long*, long long) = {splitSearch , linearSearch, maxValue};
char* testFile [tests] = {"splitSearch_result.csv" , "linear_result.csv", "maxValue_result.csv"};

static long long maxValue (long long* arr, long long length) {
    long long max = 0;
    for(long long i = 1; i < length; i++) {
        if(arr [max] < arr [i]) max = i;
    }
    return max;
}

static long long splitSearch (long long* arr, long long length) {
    long long width = 1, start = 0;
    while(width < length) {
        width = width << 1;
    }
    while(width > 1) {
        width /= 2;
        if(
            width + start + 1 < length &&
            arr [width + start] <
            arr [width + start + 1]
            ) {
            start += width;
        }
    }
    return
        arr [start] >=
        arr [start + 1] ?
        start : start + 1;
}

static long long linearSearch (long long* arr, long long length) {
    long long max = 0;
    for(long long i = 1; i < length; i++) {
        if(arr [max] < arr [i]) max = i;
        else return max;
    }
    return max;
}
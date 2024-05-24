#include "testFunktions.h"
#include <stdlib.h>

long long (*toBeTested [tests]) (long long*, long long) = {splitSearch , boolSplit,ifMax, boolMax};
char* testFile [tests] = {"splitSearch_result.csv" ,"boolSplit_result.csv", "ifStatement_result.csv", "boolMath_result.csv"};

static long long ifMax (long long* arr, long long length) {
    long long top = 0;
    for(long long i = 1; i < length; i++) {
        if(arr [i] > arr [top]) {
            top = i;
        }
    }
    return top;
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

static long long boolSplit (long long* arr, long long length) {
    long long width = 1, start = 0;
    while(width < length) {
        width = width << 1;
    }
    while(width > 1) {
        width /= 2;
        start += width * (long long)((
                width + start + 1 < length
            )&&(
                arr [width + start] <
                arr [width + start + 1]
            ));
    }
    return
        start +
    (long long)(arr [start] < arr [start + 1]);
}

static long long boolMax (long long* arr, long long length) {
    long long top = 0;
    for(long long i = 1; i < length; i++) {
        top = 
            i * (arr [i] > arr [top]) + 
            top * (arr [i] <= arr [top]);
    }
    return top;
}


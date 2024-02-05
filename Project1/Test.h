#pragma once
int allTest(long long (*f) (long long*, long long));
struct testResult FrontToBackTest(long long (*findTop) (long long*, long long));
struct testResult BackToFrontTest(long long (*findTop) (long long*, long long));
struct testResult RandomPlaceTest(long long (*findTop) (long long*, long long));
/**
	* @param maxLength: dictates the maximum size of array to be speed tested.
	* @param WARNING, this will allocate two pieces of memory according to
	* @param malloc(maxLength * sizeof(long long)) and
	* @param malloc(dataPoints * sizeof(long long))
	* @returns long long* {runtime,arraylength,runtime,arraylength...}
	* @returns lenght = dataPoints * 2
*/
long long* speedTest(long long (*findTop) (long long*, long long), long long maxLength, long long dataPoints);
static long long FrontToBack(long long (*findTop) (long long*, long long), long long* arr, long long currentLength);
static long long BackToFront(long long (*findTop) (long long*, long long), long long* arr, long long currentLength);
static long long RandomArray(long long (*findTop) (long long*, long long), long long* arr, long long currentLength);
static long long linearInterpolation(long long max, long long index, long long totalIndexes);

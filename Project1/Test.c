#define RAND_MAX 2147483647
#include <stdio.h>
#include "Test.h"
#include "stdlib.h"
#include <time.h>
#include "progBar.c"
#define testLength 1000
#define repeat 10
#define consoleWidth 50

long long* testARR;
//				   ESC  [    3    8    ;    5    ;    <n>  m
char red [10] =   {0x1b,0x5B,0x33,0x38,0x3b,0x35,0x3b,0x31,0x6d,0};
char green [10] = {0x1b,0x5B,0x33,0x38,0x3b,0x35,0x3b,0x32,0x6d,0};
char white [19] = {0x1b,0x5B,0x33,0x38,0x3b,0x35,0x3b,0x30,0x6d,
				   0x1b,0x5B,0x34,0x38,0x3b,0x35,0x3b,0x37,0x6d,0};
// ^			   ESC  [    4    8   ;    5    ;    <n>  m

//				   ESC  [    0    m
char reset [5] =  {0x1b,0x5B,0x30,0x6d,0};
//                 ESC  [    1    A 
char up [5]    =  {0x1b,0x5B,0x31,0x41,0};
//                 ESC  [    0    K	
char clear [5] =  {0x1b,0x5B,0x30,0x4B,0};
struct testResult {
	int correct;
	long long expected;
	long long result;

};

static void printResults(char* functionName, struct testResult results) {
	printf("Test: %s\n", functionName);
	printf(
		"\tresult: %s%s%s\n",
		(results.correct ? green : red),
		(results.correct ? "SUCCES" : "FAILED"),
		reset
	);
	printf("\treceived: %lld\n", results.result);
	printf("\texpected: %lld\n", results.expected);
}

int allTest(long long (*f) (long long*, long long)) {
	testARR = malloc(sizeof(long long) * testLength);
	if(testARR == NULL) {
		printf ("malloc failed: %lld", sizeof (long long) * testLength);
		return -1;
	}
	printf("Running test's, this might take a while!\n0/3");
	struct testResult front  = FrontToBackTest(f);
	
	printf("\r1");
	struct testResult back   = BackToFrontTest(f);
	
	printf("\r2");
	struct testResult random = RandomPlaceTest(f);
	
	printf("\rTest's completed!\n");
	printResults("front" , front);
	printResults("back"  , back);
	printResults("random", random);

	int result = front.correct && back.correct && random.correct;

	printf(
		"\nFinal result: %s%s%s\n",
		(result ? green : red),
		result ?
		"SUCCES!" : 
		"FAILED!", 
		reset
	);
	free(testARR);
	return result;
}

struct testResult FrontToBackTest(long long (*findTop) (long long*, long long)) {
	for (long long i = 0; i < testLength; i++) {
		testARR[i] = i;
	}
	long long top = findTop(testARR, testLength);
	struct testResult output = {
		.correct = top == (testLength - 1),
		.expected = testLength - 1,
		.result = top,
	};
	return output;
}

struct testResult BackToFrontTest(long long (*findTop) (long long*, long long)) {
	for (long long i = 0; i < testLength; i++) {
		testARR[i] = testLength - i - 1;
	}
	long long top = findTop(testARR, testLength);
	struct testResult output = {
		.correct = top == 0,
		.expected = 0,
		.result = top,
	};
	return output;
}

struct testResult RandomPlaceTest(long long (*findTop) (long long*, long long)) {
	for (long long i = testLength - 1; i >= 0; i--) {
		testARR[i] = i;
	}
	for (long long i = 0; i < testLength; i++) {
		long long temp = testARR[i];
		int random = rand();
		testARR[i] = testARR[random % testLength];
		testARR[random % testLength] = temp;
	}
	long long top = findTop(testARR, testLength);
	struct testResult output = {
		.correct = 
		((top == 0) ? 1 : testARR[top - 1] < testARR[top]) && 
		((top == testLength-1) ? 1 : testARR[top + 1] < testARR[top]),
		.expected = -1,
		.result = top,
	};
	return output;
}

#define arrayRising 0
#define arrayFalling 1
#define arrayRandom 2

static int ArrayFill(long long* arr, long long length, int type) {
	switch(type) {
	case arrayRising:
		for (long long i = 0; i < length; i++) {
			arr[i] = i;
		}
	break;
	case arrayFalling:
		for (long long i = 0; i < length; i++) {
			arr[i] = length - i - 1;
		}
	break;
	case arrayRandom:
		ArrayFill(arr, length, arrayRising);
		for (long long i = 0; i < length; i++) {
			int random = rand();
			long long temp = arr[random];
			arr[random] = arr[i];
			arr[i] = temp;
		}
	break;
	default:
		printf("\narray filling error, invalid fill direction");
		return 1;
	}
}

long long* speedTest(long long (*findTop) (long long*, long long), long long maxLength, long long dataPoints) {
	long long* arr = malloc (maxLength * ((int) sizeof (long long)));
	long long* runTime = malloc(dataPoints * ((int) sizeof(long long)));
	long long currentLength;
	if(arr == NULL) {
		printf ("malloc failed: %lldB\ndata array\n\n", maxLength * ((int) sizeof (long long)));
		return NULL;
	}
	if(runTime == NULL) {
		printf ("malloc failed: %lldB\ntime array\n\n", dataPoints * ((int) sizeof (long long)));
		return NULL;
	}
	printf ("\n");
	long long progMax = dataPoints * repeat * 3;
	progBar (progMax, 0, 50);
	printf (up);
	
	printf ("\r%sfilling array please wait%s\r",white,reset);
	ArrayFill(arr, maxLength, arrayRising);
	for(long long i = 0; i < dataPoints; i++) {
		long long current = 0;
		long long temp = 0;
		currentLength = linearInterpolation (maxLength, i, dataPoints);
		runTime [i] = 0;
		for(long long j = 0; j < repeat; j++) {
			//linear test 0->length
			current = FrontToBack (findTop, arr, currentLength);
			temp += current;
			printf ("\r%siteration time: %lldns\n",clear, temp);
			progBar (progMax, 1 + i * repeat + j + dataPoints, consoleWidth);
			printf ("%s\r", up);
		}
		runTime [i] = currentLength;
	}
	
	printf ("\r%sfilling array please wait%s\r", white, reset);
	ArrayFill (arr, maxLength, arrayFalling);
	for(long long i = 0; i < dataPoints; i++) {
		long long current = 0;
		long long temp = 0;
		currentLength = linearInterpolation (maxLength, i, dataPoints);
		runTime [i] = 0;
		for(long long j = 0; j < repeat; j++) {
			//linear test length->0
			current = BackToFront (findTop, arr, currentLength);
			temp += current;
			printf ("\r%siteration time: %lldns\n",clear, current);
			progBar (progMax, 1 + i * repeat + j + dataPoints * repeat, consoleWidth);
			printf ("%s\r", up);
		}
		runTime [i] = max (currentLength, runTime [i]);
	}
	
	printf ("\r%sfilling array please wait%s\r", white, reset);
	ArrayFill (arr, maxLength, arrayRandom);
	for(long long i = 0; i < dataPoints; i++) {
		long long current = 0;
		long long temp = 0;
		currentLength = linearInterpolation (maxLength, i, dataPoints);
		runTime [i] = 0;
		for(long long j = 0; j < repeat; j++) {
			//random Test 
			current = RandomArray (findTop, arr, currentLength);
			temp += current;
			printf ("\r%siteration time: %lldns\n",clear, current);
			progBar (progMax, 1+ i * repeat + j + dataPoints * repeat * 2, consoleWidth);
			printf ("%s\r", up);
		}
		runTime [i] = max (currentLength, runTime [i]);
	}
	
	free(arr);
	arr = malloc(dataPoints * sizeof(long long));
	if (arr != NULL) {
		for (long long i = 0; i < dataPoints; i++) {
			arr[i] = linearInterpolation(maxLength, i, dataPoints);
		}
	}

	long long* out = malloc(dataPoints * 2 * sizeof(long long));
	if (out == NULL) return NULL;
	for (long long i = 0; i < dataPoints; i++) {
		out[i * 2] = runTime[i];
		if (arr != NULL)
			out[i * 2 + 1] = arr[i];
		else
			out[i * 2 + 1] = -1;
	}
	free(arr);
	return out;
}

static long long FrontToBack(long long (*findTop) (long long*, long long), long long* arr, long long currentLength) {
	struct timespec startTime, stopTime;
	long long top;
	int ignore = timespec_get(&startTime, 1);
	top = findTop(arr, currentLength);
	ignore = timespec_get(&stopTime,1);
	if (top >= currentLength || top < 0) return -1ll;
	if (!(
		(top == 0) ? 1 : arr[top - 1] < arr[top] &&
		(top == currentLength - 1) ? 1 : arr[top + 1] < arr[top]
		)) return -1ll;

	return
		((long long)stopTime.tv_sec - (long long)startTime.tv_sec) * 1000ll * 1000ll * 1000 +
		((long long)stopTime.tv_nsec - (long long)startTime.tv_nsec);
}

static long long BackToFront(long long (*findTop) (long long*, long long), long long* arr, long long currentLength) {
	struct timespec startTime, stopTime;
	long long top;
	int ignore = timespec_get(&startTime, 0);
	top = findTop(arr, currentLength);
	ignore = timespec_get(&stopTime, 0);
	if (top >= currentLength || top < 0) return -1ll;
	if (!(
		(top == 0) ? 1 : arr[top - 1] < arr[top] &&
		(top == currentLength - 1) ? 1 : arr[top + 1] < arr[top]
		)) return -1ll;
	return
		((long long)stopTime.tv_sec - (long long)startTime.tv_sec) * 1000ll * 1000ll * 1000ll +
		((long long)stopTime.tv_nsec - (long long)startTime.tv_nsec);
}

static long long RandomArray(long long (*findTop) (long long*, long long), long long* arr, long long currentLength) {
	struct timespec startTime, stopTime;
	long long top;
	int  ignore = timespec_get(&startTime, 0);
	top = findTop(arr, currentLength);
	ignore = timespec_get(&stopTime, 0);
	if (top >= currentLength || top < 0) return -1ll;
	if (!(
		(top == 0) ? 1 : arr[top - 1] < arr[top] &&
		(top == currentLength - 1) ? 1 : arr[top + 1] < arr[top]
		)) return -1ll;
	return
		((long long)stopTime.tv_sec - (long long)startTime.tv_sec) * 1000ll * 1000ll * 1000ll +
		((long long)stopTime.tv_nsec - (long long)startTime.tv_nsec);
}

/**
    * @param max: the highes value that wil be reached
	* @param index: the current point allong the line,
	* @param totalIndexes: the total number of indexes
	* @param
	* @param if (index == totalIndexes - 1) return max
	* @param if (index == 0) return max / totalIndexes
*/
long long linearInterpolation(long long max, long long index, long long totalIndexes) {
	return max / totalIndexes * (index + 1ll);
}
#include <stdio.h>
#include "Test.h"
#include "stdlib.h"
#include <time.h>
#include "progBar.c"

long long* testARR;
long long testLength = 1000;
//				   ESC  [    3    8    ;    5    ;    <n>  m
char red[10]   = { 0x1b,0x5B,0x33,0x38,0x3b,0x35,0x3b,0x31,0x6d,0 };
char green[10] = { 0x1b,0x5B,0x33,0x38,0x3b,0x35,0x3b,0x32,0x6d,0 };
//				  ESC  [    0    m
char reset[5] = { 0x1b,0x5B,0x30,0x6d,0 };
//                ESC  [    1    A 
char up[5] =    { 0x1b,0x5B,0x31,0x41,0 };
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
	printf("Running test's, this might take a while!\n0/3");
	struct testResult front  = FrontToBackTest(f);
	
	printf("\r1");
	struct testResult back   = BackToFrontTest(f);
	
	printf("\r2");
	struct testResult random = RandomPlaceTest(f);
	
	printf("\rTest's completed!\n");
	printResults("front", front);
	printResults("back", back);
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

long long* speedTest(long long (*findTop) (long long*, long long), long long maxLength, long long dataPoints) {
	long long* arr = malloc(maxLength * sizeof(long long));
	long long* runTime = malloc(dataPoints * sizeof(long long));
	long long currentLength;
	long long current;
	if (arr == NULL || runTime == NULL) return NULL;
	for (long long i = 0; i < dataPoints; i++){
		currentLength = linearInterpolation(maxLength, i, dataPoints);
		runTime[i] = 0;
		//linear test 0->length
		current = FrontToBack(findTop, arr, currentLength);
		runTime[i] = max(current, runTime[i]);
		//linear test length->0
		current = BackToFront(findTop, arr, currentLength);
		runTime[i] = max(current, runTime[i]);
		//random Test 
		current = RandomArray(findTop, arr, currentLength);
		runTime[i] = max(current, runTime[i]);
		
		printf("\riteration time: %lldns\n", runTime[i]);
		progBar(dataPoints, i, 20);
		printf(up);
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
	for (long long j = 0; j < currentLength; j++) {
		arr[j] = j;
	}
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
	for (long long j = 0; j < currentLength; j++) {
		arr[j] = j;
	}
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
	for (long long j = 0; j < currentLength; j++) {
		arr[j] = j;
	}
	for (long long j = 0; j < currentLength; j++) {
		int random = rand() % currentLength;
		long long temp = arr[j];
		arr[j] = arr[random];
		arr[random] = temp;
	}
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
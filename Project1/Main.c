#include <Windows.h>
#include <winbase.h>
#include "testFunktions.h"
#include "stdlib.h"
#include <stdio.h>
#include <time.h>
#include "Test.h"

int main() {
    SetThreadExecutionState (ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
    long long dataPoints = 20;
    long long maxLength = 100ll * 1000ll * 1000ll;
    for (int i = 0; i < tests; i++) {
        printf("\n\n\ntesting funktion: %d\n", i);
        int succes = allTest (toBeTested [i]);
        if(succes == -1) {
            printf ("failed to alocate memory\n ending test early!");
            return 1;
        } else if(succes == 0) {
            printf ("algorithm %d failed general testing\nskipping algorithm", i);
            continue;
        }
        printf("\ntesting speed\n");
        long long* speed = speedTest(toBeTested[i], maxLength, dataPoints);
        if (speed == NULL) {
            printf("failed to alocate memory\n ending test early!");
            return 1;
        }
        printf("\n\nwriting result to file: \"%s\"\n",testFile[i]);
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
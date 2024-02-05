#pragma once
static long long splitSearch (long long* arr, long long length);
static long long linearSearch (long long* arr, long long length);
static long long maxValue (long long* arr, long long length);
#define tests 1
long long (*toBeTested [tests]) (long long*, long long);
char* testFile [tests];
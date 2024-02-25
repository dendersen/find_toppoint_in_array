#pragma once
static long long ifMax (long long* arr, long long length);
static long long splitSearch (long long* arr, long long length);
static long long boolSplit (long long* arr, long long length);
static long long boolMax (long long* arr, long long length);
static long long splitOptimize (long long* arr, long long length);
#define tests 4
long long (*toBeTested [tests]) (long long*, long long);
char* testFile [tests];
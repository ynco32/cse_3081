#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <iostream>

/******************************************************************************************************/
#include <Windows.h>
#define CHECK_TIME_START(start,freq) QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(start,end,freq,time) QueryPerformanceCounter((LARGE_INTEGER*)&end); time = (float)((float)(end - start) / (freq * 1.0e-3f))

__int64 _start, _freq, _end;
float compute_time;
/******************************************************************************************************/


#define CHECK_GENERATED_DATA
#undef CHECK_GENERATED_DATA

#define N_ELEMENTS 2097152	// 2^21
//#define N_ELEMENTS 1048576	// 2^20
//#define N_ELEMENTS 16384
//#define N_ELEMENTS 1024		// 2^10
//#define N_ELEMENTS 32		// 2^5

//#define FILE_NAME_PREFIX  "input_data_rd" // Random
//#define FILE_NAME_PREFIX  "input_data_as" // Ascending
//#define FILE_NAME_PREFIX  "input_data_de" // Descending
//#define FILE_NAME_PREFIX  "input_data_fw" // Few swaps

#define INPUT_FILE_NAME "input_data_rd_1048576.bin"
#define OUTPUT_FILE_NAME "output_data_rd_1048576.bin"

using namespace std;

void merge(unsigned int* arr, int first, int mid, int last)
{
	unsigned int* sorted = new unsigned int[last - first + 1];
	int i, j, k;
	i = first;		// First arr idx
	j = mid + 1;	// Second arr idx
	k = 0;			// Sorted arr idx

	while (i <= mid && j <= last)
	{
		if (arr[i] <= arr[j]) sorted[k++] = arr[i++];
		else sorted[k++] = arr[j++];
	}

	if (i > mid)
		while (j <= last) sorted[k++] = arr[j++];
	else
		while (i <= mid) sorted[k++] = arr[i++];

	for (i = first, k = 0; i <= last; i++, k++) arr[i] = sorted[k];

	delete[] sorted;
}

int Merge_Sort(unsigned int* data, int left, int right) {
	
	if (right < left) {
		fprintf(stderr, "ERROR: INPUT ERROR\n");
		return -1;
	}

	else if (right == left) {
		return 0;
	}

	else {
		int mid = (left + right) / 2;
		Merge_Sort(data, left, mid);
		Merge_Sort(data, mid + 1, right);
		merge(data, left, mid, right);
	}
	return 0;

}







int main(int argc, char** arvgv) {

	unsigned int* array = (unsigned int*)malloc(sizeof(int) * N_ELEMENTS);
	unsigned scale_factor_uint = UINT_MAX >> 2;
	char file_name[128];

	FILE* fp = fopen(INPUT_FILE_NAME, "r");

	int n = N_ELEMENTS; // n is the number of data elements to be written


	/*sprintf(file_name, "%s_%d.bin", FILE_NAME_PREFIX, n);

	fprintf(stdout, "^^^ UNIT_MAX = %u\n", UINT_MAX);
	fprintf(stdout, "^^^ RAND_MAX = %u\n\n", RAND_MAX);*/

	if (fp == NULL) {
		fprintf(stderr, "Error: cannot open the binary file %s for Read...\n", INPUT_FILE_NAME);
		exit(-1);
	}

	//init_array(array, n);
	fread(&n, sizeof(int), 1, fp);
	fprintf(stdout, "*** Number of elements = %d\n", n);
	fprintf(stdout, "*** Data Type = %s\n", INPUT_FILE_NAME);

	for (int i = 0; i < n; i++) {
		//unsigned int key;
		fread(&array[i], sizeof(unsigned int), 1, fp);
		//fprintf(stdout, " [%d] %11u\n", i, array[i]);
		//array[i] = key;
	}

	/*fwrite(&n, sizeof(unsigned int), 1, fp);
	srand((unsigned int)time(NULL));*/

	CHECK_TIME_START(_start, _freq);

	Merge_Sort(array, 0, n);

	CHECK_TIME_END(_start, _end, _freq, compute_time);
	fprintf(stdout, "\n^^^ Time for Insertion Sorting %d elements = %.3fms\n\n", n, compute_time);


	fclose(fp);




	//output file
	FILE* out_fp = fopen(OUTPUT_FILE_NAME, "wb");
	if (out_fp == NULL) {
		fprintf(stderr, "Error: cannot open the binary file %s for writing...\n", OUTPUT_FILE_NAME);
		exit(-1);
	}

	fwrite(&n, sizeof(unsigned int), 1, out_fp);
	for (int i = 0; i < n; i++) {
		//
		// For more efficient read/write operations, use a buffer of proper size!!!
		//
		/*unsigned int key = (unsigned int)((((float)rand()) / RAND_MAX) * (((float)rand()) / RAND_MAX) * (((float)rand()) / RAND_MAX) *
			(((float)rand()) / RAND_MAX) * (float)scale_factor_uint);*/
		fwrite(&array[i], sizeof(unsigned int), 1, out_fp);
	}

	fclose(out_fp);


	//check
	//fprintf(stdout, "\n\n\n-----sorted-----\n");

	/*
	FILE* out_fp2 = fopen(OUTPUT_FILE_NAME, "wb");
	if (out_fp == NULL) {
		fprintf(stderr, "Error: cannot open the binary file %s for writing...\n", OUTPUT_FILE_NAME);
		exit(-1);
	}
	for (int i = 0; i < n; i++) {
		//unsigned int key;
		fread(&array[i], sizeof(unsigned int), 1, out_fp2);
		fprintf(stdout, " [%d] %11u\n", i, array[i]);
		//array[i] = key;
	}
	

	*/


#ifdef CHECK_GENERATED_DATA
	if ((fp = fopen(file_name, "rb")) == NULL) {
		fprintf(stderr, "Error: can not open the binary file %s for reading...\n", file_name);
		exit(-1);
	}
	fread(&n, sizeof(int), 1, fp);
	fprintf(stdout, "*** Number of elements = %d\n", n);

	for (int i = 0; i < n; i++) {
		unsigned int key;
		fread(&key, sizeof(unsigned int), 1, fp);
		fprintf(stdout, " [%d] %11u\n", i, key);
	}
	fclose(fp);
#endif
}

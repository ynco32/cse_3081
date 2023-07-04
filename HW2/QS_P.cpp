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


//#define N_ELEMENTS 2097152	// 2^21
//#define N_ELEMENTS 1048576	// 2^20
//#define N_ELEMENTS 16384
//#define N_ELEMENTS 1024		// 2^10
#define N_ELEMENTS 4096		// 2^5
//#define N_ELEMENTS 131072

//#define FILE_NAME_PREFIX  "input_data_rd" // Random
//#define FILE_NAME_PREFIX  "input_data_as" // Ascending
//#define FILE_NAME_PREFIX  "input_data_de" // Descending
//#define FILE_NAME_PREFIX  "input_data_fw" // Few swaps

#define INPUT_FILE_NAME "input_data_rd_4096.bin"
#define OUTPUT_FILE_NAME "output_data_rd_4096.bin"

using namespace std;



void swap(unsigned int* data, int a, int b) {
	int tmp = data[a];
	data[a] = data[b];
	data[b] = tmp;
}

void Median_of_Three(unsigned int* data, int left, int mid, int right) {
	if (data[left] > data[mid]) swap(data, left, mid);
	if (data[mid] > data[right]) swap(data, mid, right);
	if (data[left] > data[mid]) swap(data, left, mid);
}

void Median_of_Five(unsigned int* data, int f, int s, int th, int fourth, int fifth ) {
	
	if (data[f] > data[s]) swap(data, f, s);
	if (data[f] > data[th]) swap(data, f, th);
	if (data[f] > data[fourth]) swap(data, f, fourth);
	if (data[f] > data[fifth]) swap(data, f, fifth);

	if (data[s] > data[th]) swap(data, s, th);
	if (data[s] > data[fourth]) swap(data, s, fourth);
	if (data[s] > data[fifth]) swap(data, s, fifth);

	if (data[th] > data[fourth]) swap(data, th, fourth);
	if (data[th] > data[fifth]) swap(data, th, fifth);

	if (data[fourth] > data[fifth]) swap(data, fourth, fifth);
}



//int pivotSelect(unsigned int* data, int left, int right) {
//	int pivot = left;
//	int temp = 0;
//
//
//	int size = right - left + 1;
//
//
//	for (int i = 0; i < M_PIVOT; i++) {
//		int idx1 = i * step;
//		temp += data[idx1];
//	}
//	
//
//
//	return temp;
//}

int Quick_Sort_P(unsigned int* data, int left, int right) {
	int n = right - left + 1;
	int pivot;

	if (n < 0) {
		fprintf(stderr, "Error: Input Error\n");
		return -1;
	}

	else if (n < 4) {
		pivot = left;
		int i = left + 1;
		int j = right;
		int temp;

		while (i < j + 1) {
			while (i < right + 1 && data[i] <= data[pivot]) {
				i++;
			}
			while (j > left && data[j] >= data[pivot]) {
				j--;
			}

			if (i > j) {
				temp = data[j];
				data[j] = data[pivot];
				data[pivot] = temp;
			}
			else {
				temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		}
	}

	

	//int pivot = left; //pivot_Naive
	else {

		Median_of_Three(data, left, right, n/2);
		
		//Median_of_Five(data, left, left + n/4-1, left + n/2-1, left + n/4*3-1, right);

		pivot = n/2;
		swap(data, n / 2, right - 1);
		
		int i = left;
		int j = right - 1;
		int temp;



		while (i <= j) {
			while (i <= right && data[i] <= data[pivot]) {
				i++;
			}
			while (j > left && data[j] >= data[pivot]) {
				j--;
			}

			if (i > j) {
				temp = data[j];
				data[j] = data[pivot];
				data[pivot] = temp;
			}
			else {
				temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		}

		Quick_Sort_P(data, left, j - 1);
		Quick_Sort_P(data, j + 1, right);
	}

	return 0;
}
//
//int main() {
//
//	unsigned int testarr[10] = { 1, 5, 315, 7, 2, 4, 9, 0, 2, 10 };
//
//	Quick_Sort_P(testarr, 0, 10);
//	cout << "\n";
//	for (int i = 0; i < 10; i++) {
//		cout << testarr[i] << '\n';
//	}
//
//
//	return 0;
//}


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

	Quick_Sort_P(array, 0, n);

	CHECK_TIME_END(_start, _end, _freq, compute_time);
	fprintf(stdout, "\n^^^ Time for Pivot Selected Quick Sorting %d elements = %.3fms\n\n", n, compute_time);


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

	/*
	//check
	fprintf(stdout, "\n\n\n-----sorted-----\n");


	FILE* out_fp2 = fopen(OUTPUT_FILE_NAME, "wb");
	if (out_fp2 == NULL) {
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

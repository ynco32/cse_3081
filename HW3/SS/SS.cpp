#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cstring>

using namespace std;

#define FILE_NAME_LEN 128
#define TESTCASE 10
#define N_ELEMENTS 200

int main(int argc, const char* argv[]) {
    //    char input_file[TESTCASE][FILE_NAME_LEN];
    //    char output_file[TESTCASE][FILE_NAME_LEN];

    int N;
    FILE* fp;
    fp = fopen("SS_test_command.txt", "r");


    if (fp == NULL) {
        fprintf(stderr, "ERROR: cannot open the file");
    }

    char ch;
    char numArr[100];
    int idx_c = 0;
    while (1) {
        ch = fgetc(fp);
        if (ch == '\n') {
            numArr[idx_c] = '\0';
            idx_c = 0;
            break;
        }
        numArr[idx_c++] = ch;
    }
    N = atoi(numArr);

    //fscanf(fp, "%d", &N);
    char* input_file;
    char* output_file;
    for (int i = 0; i < N; i++) {
        char input_file[FILE_NAME_LEN];
        char output_file[FILE_NAME_LEN];

        //input_file = (char*)malloc(sizeof(char) * 100);
        //output_file = (char*)malloc(sizeof(char) * 100);
        int num;
        int set[N_ELEMENTS];
        int result[N_ELEMENTS];
        bool dp[N_ELEMENTS][N_ELEMENTS];
        int L;
        int index = 0;

        //file 읽고 data 저장
//        fscanf(fp, "%s", input_file);
//        fscanf(fp, "%s", output_file);
        fscanf(fp, "%s%s\n", input_file, output_file);

        /*

        printf(" -- %d --- \n",i);
        printf("%s\n", input_file);
        printf("%s\n", output_file);
        */


        FILE* input_fp = fopen(input_file, "r");
        FILE* output_fp = fopen(output_file, "w");

        if (input_fp == NULL) {
            fprintf(stderr, "ERROR: cannot open the input file %s", input_file);
        }

        if (output_fp == NULL) {
            fprintf(stderr, "ERROR: cannot open the input file %s", output_file);
        }
        //free(input_file);
        //free(output_file);

        //fread(&num, sizeof(int), 1, input_fp);
        fscanf(input_fp, "%d\n", &num);



        for (int j = 0; j < num; j++) {
            fscanf(input_fp, "%d", &set[j]);
        }
        /*
        for (int j = 0; j < num; j++){
            printf("%d ", set[j]);
        }
       */

       //        L = set[num];
       //        set[num] = 0;
        fscanf(input_fp, "%d", &L);
        //printf("\nL: %d\n", L);
        //init dp table
        for (int k = 0; k < num; k++) {
            for (int l = 0; l <= L; l++) {
                dp[k][l] = false;
            }
        }

        //fill dp table
        for (int k = 0; k < num; k++) {
            dp[k][0] = true;
            dp[k][set[k]] = true;

            for (int l = 1; l <= L; l++) {
                dp[k][l] = dp[k - 1][l];
                if (l >= set[k]) {
                    dp[k][l] = dp[k][l] || dp[k - 1][l - set[k]];
                }
            }
        }

        //file Write
        /*
        printf("\ndp\n");

        for (int x = 0; x < num; x++){
            for (int y = 0; y <= L; y++){
                printf("%d ", dp[x][y]);
            }
            printf("\n");
        }
        printf("\n");
        */
        int dp_result[N_ELEMENTS];

        if (dp[num - 1][L] == false) {
            //존재하지 않을 경우
            fprintf(output_fp, "0\n");
        }
        else {

            fprintf(output_fp, "1\n");

            if (dp[num - 1][L] == true) {
                for (int k = num; k--; ) {
                    if (k && dp[k - 1][L]) continue;
                    if (!k && !L) continue;

                    L = L - set[k];
                    dp_result[index++] = k;
                }

                fprintf(output_fp, "%d\n", index);

                //                for (int m = index; m >= 0; m--){
                int m = index;

                while (m--) {
                    fprintf(output_fp, "%d\n", dp_result[m]);
                }
            }
        }

        /*
        printf("\n");
        printf("--------------\n\n");
        printf("\n");
        */
        fclose(input_fp);
        fclose(output_fp);


    }
    fclose(fp);
    return 0;
}

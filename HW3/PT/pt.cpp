#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <float.h>

using namespace std;

#define FILE_NAME_LEN 128
#define TESTCASE 10
#define N_ELEMENTS 200

struct _vertex {
    int first;
    int second;
} res[N_ELEMENTS];

double x[N_ELEMENTS];
double y[N_ELEMENTS];
double dp[N_ELEMENTS][N_ELEMENTS];
int pair_vp[N_ELEMENTS][N_ELEMENTS];
int top = 0;

//함수 선언
int cmp(const void* a, const void* b);
double set_dp(int i, int s, int vertex_num);
double cal_dist(double x1, double y1, double x2, double y2);
double get_vertex_dist(int v1, int v2, int num_vertex);
void set_index(int i, int s, int vertex_num);



int main(int argc, const char* argv[]) {
    //    char input_file[TESTCASE][FILE_NAME_LEN];
    //    char output_file[TESTCASE][FILE_NAME_LEN];

    int N;
    FILE* fp;
    fp = fopen("PT_test_command.txt", "r");


    if (fp == NULL) {
        fprintf(stderr, "ERROR: cannot open the file");
        return -1;
    }

    fscanf(fp, "%d", &N);


    for (int i = 0; i < N; i++) {
        char input_file[FILE_NAME_LEN];
        char output_file[FILE_NAME_LEN];
        int num;

        //file 읽기
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
            return -1;
        }
        if (output_fp == NULL) {
            fprintf(stderr, "ERROR: cannot open the output file %s", output_file);
            return -1;
        }


        //fread(&num, sizeof(int), 1, input_fp);
        fscanf(input_fp, "%d\n", &num);

        for (int k = 0; k < num; k++) {
            //            for (int l = 0; l < num; l++){
            //                dp[k][l] = 0.0;
            //                pair_vp[k][l] = 0;
            //
            //            }
            x[k] = 0.0;
            y[k] = 0.0;
        }
        top = 0;
        memset(dp, 0.0, sizeof(dp));
        memset(pair_vp, 0, sizeof(pair_vp));

        // printf("%d\n", num);

        for (int j = 0; j < num; j++) {
            fscanf(input_fp, "%lf %lf", &x[j], &y[j]);
        }

        /*
        for (int j = 0; j < num; j++){
            printf("%lf %lf\n", x[j], y[j]);
        }
       */


        double a = set_dp(0, num, num);
        set_index(0, num, num);
        qsort(res, top, sizeof(struct _vertex), cmp);

        /*
        printf("\nres:\n");
        for (int j = 0; j < top; j++){
            printf("%d %d\n", res[j].first, res[j].second);
        }
         printf("\ntop: %d\n", top);
        */


        fprintf(output_fp, "%lf\n", a);
        for (int j = 0; j < top; j++) {
            if (res[j].second - res[j].first == num - 1 || res[j].second - res[j].first == 1) {
                continue;
            }
            if (res[j].first == res[j + 1].first && res[j].second == res[j + 1].second) {
                continue;
            }
            else {
                fprintf(output_fp, "%d %d\n", res[j].first, res[j].second);
            }
        }

        //        printf("\n");
        //        printf("--------------\n\n");
        //        printf("\n");

        fclose(input_fp);
        fclose(output_fp);


    }
    fclose(fp);
    return 0;
}

int cmp(const void* a, const void* b) {
    _vertex* pa = (_vertex*)a;
    _vertex* pb = (_vertex*)b;

    if (pa->first > pb->first) return 1;
    else if (pa->first < pb->first) return -1;
    else {
        if (pa->second > pb->second) return 1;
        else if (pa->second < pb->second) return -1;
    }
    return 0;

}

double cal_dist(double x1, double y1, double x2, double y2) {
    double result = 0;

    result = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    result = sqrt(result);
    //printf("car_dist: %lf\n\n", result);
    return result;
}



double set_dp(int i, int s, int vertex_num) {
    double result = DBL_MAX;
    if (s < 4) {
        return 0;
        //printf("s<4\n");
    }

    if (dp[i][s]) {
        return dp[i][s];
        //printf("true\n");
    }


    for (int k = 1; k < s - 1; k++) {
        double num1 = set_dp(i, k + 1, vertex_num);
        double num2 = set_dp(i + k, s - k, vertex_num);
        double num3 = get_vertex_dist(i, i + k, vertex_num) + get_vertex_dist(i + k, i + s - 1, vertex_num);
        double sum = num1 + num2 + num3;
        //        printf("\nsetdp::\n")
        //        printf("num1: %lf   \n", num1);
        //        printf("num2: %lf   \n", num2);
        //        printf("num3: %lf   \n", num3);
        //        printf("sum: %lf   \n", sum);
        //        printf("\n::");

        if (result > sum) {
            result = sum;
            pair_vp[i][s] = k;
        }
    }

    dp[i][s] = result;
    return result;
}

double get_vertex_dist(int v1, int v2, int num_vertex) {
    double result = 0;
    if (v1 - v2 == 1 || v1 - v2 == -1) {
        result = 0;
    }
    else if (v1 - v2 == num_vertex - 1 || v1 - v2 == -1 + num_vertex) {
        result = 0;
    }
    else {
        result = cal_dist(x[v1], y[v1], x[v2], y[v2]);
    }
    return result;
}


void set_index(int i, int s, int vertex_num) {
    if (s < 4) {
        return;
    }

    else {
        int current = pair_vp[i][s];
        int par = i + s - 1;
        int p = i + current;

        if (current > 1) {
            res[top].first = i;
            res[top++].second = p;
        }

        if (s - current > 2) {
            res[top].first = p;
            res[top++].second = par;
        }

        if (s < vertex_num) {
            res[top].first = i;
            res[top++].second = par;
        }

        set_index(i, current + 1, vertex_num);
        set_index(p, s - current, vertex_num);
    }
}

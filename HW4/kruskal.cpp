#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <stdint.h>
#include <time.h>
//#include <Windows.h>

using namespace std;

FILE *fp_in, *fp_out;

typedef struct Edge {
    int src;
    int dest;
    int weight;
} Edge;

typedef struct Graph {
    int V;
    int E;
    
    Edge * edge;
} Graph;

Graph * createGraph (int v, int e){
    Graph * graph = (Graph *)malloc(sizeof(Graph));
    graph -> V = v;
    graph -> E = e;
    graph -> edge = (Edge *) malloc(sizeof(Edge)*e);
    return graph;
}

typedef struct Subset {
    int parent;
    int rank;
    //추가구현
    int64_t weight_sum;
    int cnt_vertices;
} Subset;

int find(Subset subsets[], int i){
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    
    return subsets[i].parent;
}

int myCmp(const void* a, const void* b) {
    Edge* temp1 = (Edge*)a;
    Edge* temp2 = (Edge*)b;
    return temp1->weight - temp2->weight;
}



void Union(struct Subset subsets[], int x, int y, int weight) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank > subsets[yroot].rank) {
        subsets[yroot].parent = xroot;
        subsets[xroot].weight_sum += weight;
        subsets[xroot].weight_sum += subsets[yroot].weight_sum;
        subsets[xroot].cnt_vertices += subsets[yroot].cnt_vertices;
    }
    else if (subsets[xroot].rank < subsets[yroot].rank) {
        subsets[xroot].parent = yroot;
        subsets[yroot].weight_sum += weight;
        subsets[yroot].weight_sum += subsets[xroot].weight_sum;
        subsets[yroot].cnt_vertices += subsets[xroot].cnt_vertices;
    }
    else{
        subsets[xroot].parent = yroot;
        subsets[yroot].rank++;
        subsets[yroot].weight_sum += weight;
        subsets[yroot].weight_sum += subsets[xroot].weight_sum;
        subsets[yroot].cnt_vertices += subsets[xroot].cnt_vertices;
    }
}

void adjust( Edge* heapEdge, int root, int n) {
    int child = 2*root;
    
    int root_w =heapEdge[root].weight;
    Edge temp = heapEdge[root];

    
    
    while (child <= n) {
        if ((child < n) && (heapEdge[child].weight < heapEdge[child + 1].weight)) {
            child++;
        }
        if (root_w > heapEdge[child].weight)
            break;
        else {
            heapEdge[child / 2] = heapEdge[child];
            child *= 2;
        }
    }
    heapEdge[child / 2] = temp;
}

void heapsort(struct Edge* heapEdge, int n) {
    Edge temp;
    for (int i = n/2; i > 0; i--) {
        adjust(heapEdge, i, n);
    }
    for (int i = n-1; i > 0; i--) {
        temp = heapEdge[1];
        heapEdge[1] = heapEdge[i + 1];
        heapEdge[i + 1] = temp;
        adjust(heapEdge, 1, i);
    }
}





void kruskalMST(Graph * graph, Edge * heap);


int main(int argc, const char * argv[]) {
    char input_file_name[1024];
    char output_file_name[1024];
    char path[1024];
    char full_in_fp[1024];
    char full_out_fp[1024];
    

    
    
//    FILE *fp_in, *fp_out;
    int n_vertices, n_edges, v_from, v_to;
    int weight;
    int max_weight;
    //int64_t total_weight;
    FILE *fp_cmd;
    
    fp_cmd = fopen("commands.txt", "r");
    
    if (fp_cmd == NULL){
        fprintf(stderr, "File Open error : commands.txt");
        exit(-1);
    }
    
    fscanf(fp_cmd, "%s", path);
    fscanf(fp_cmd, "%s", input_file_name);
    fscanf(fp_cmd, "%s", output_file_name);

    fclose(fp_cmd);
    //cout << path<<endl;
    strcpy(full_in_fp, path);
    strcpy(full_out_fp, path);
    
    
    
    strcat(full_in_fp, input_file_name);
    strcat(full_out_fp, output_file_name);
    
    cout << "input fp name: " << full_in_fp <<endl;
    cout << "output fp name: " << full_out_fp <<endl;
    
//    cout << "input fp name: " << full_in_fp <<endl;
//    cout << "output fp name: " << full_out_fp <<endl;
    
    fp_in = fopen(full_in_fp, "r");
    if (fp_in == NULL){
        fprintf(stderr, "File Open error : input file");
        exit(-1);
    }
    
    fp_out = fopen(full_out_fp, "w");
    if (fp_out == NULL){
        fprintf(stderr, "File Open error : input file");
        exit(-1);
    }
    
    fscanf(fp_in, "%d %d %d", &n_vertices, &n_edges, &max_weight);
    fprintf(stdout, "\v = %d, e = %d, w = %d\n", n_vertices, n_edges, max_weight);
    
    Graph* graph = createGraph(n_vertices, n_edges);
    Edge* heapEdge = (Edge*)malloc(sizeof(Edge) * (n_edges + 1));
    
    
    for (int i = 0; i < n_edges; i++){
        fscanf(fp_in, "%d %d %d", &v_from, &v_to, &weight);
        //fprintf(stdout, "%d %d %d\n", v_from, v_to, weight);
//        connect_edge(node[v_from].head, make_edge(v_to, weight));
//        connect_edge(node[v_to].head, make_edge(v_from, weight));
        
//        if (v_from > v_to){
//            int tmp = v_to;
//            v_to = v_from;
//            v_from = tmp;
//        }
        
        graph->edge[i].src = v_from;
        graph->edge[i].dest = v_to;
        graph->edge[i].weight = weight;
        
        heapEdge[i + 1] = graph->edge[i];
    }
//    for (int i = 0; i < 3; i++){
//
//
//        fprintf(stdout, "%d %d %d\n", graph->edge[i].src, graph->edge[i].dest, graph->edge[i].weight);
////        connect_edge(node[v_from].head, make_edge(v_to, weight));
////        connect_edge(node[v_to].head, make_edge(v_from, weight));
//
//
//    }
    
    kruskalMST(graph, heapEdge);
    free(graph->edge);
    free(graph);
    fclose(fp_in);
    fclose(fp_out);
    
    
    return 0;
}


void kruskalMST(Graph * graph, Edge * heap){
    
    clock_t start, end;
    start = clock();
    
    int v_g = graph -> V;
    int v_e = graph -> E;
    Edge * result = (Edge *)malloc(sizeof(Edge) * (v_g-1));
    int* connected = (int*)malloc(sizeof(int) *v_g);
    
//    for (int i = 0; i < 5; i++){
//
//        fprintf(stdout, "\t::%d %d %d\n", heap[i + 1].src, heap[i + 1].dest, heap[i + 1].weight);
//    }
    
    int e = 0;
    int idx = 0;
    
//    for (int i = 0; i < 3; i++){
//
//
//        fprintf(stdout, "%d %d %d\n", graph->edge[i].src, graph->edge[i].dest, graph->edge[i].weight);
////        connect_edge(node[v_from].head, make_edge(v_to, weight));
////        connect_edge(node[v_to].head, make_edge(v_from, weight));
//
//
//    }
    
    //printf("\n------\n");
    
    //qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myCmp);
    //cout << "qsort completed\n";
    
    heapsort(heap, graph->E);
    for (int j = 0; j < v_e; j++) {
        graph->edge[j] = heap[j + 1];
        //printf("\t%d %d %d\n", heap[j + 1].weight, heap[j + 1].src, heap[j + 1].dest);
    }
    cout<<("----Heap sort completes----\n");
    
//    for (int i = 0; i < 3; i++){
//        fprintf(stdout, "%d %d %d\n", graph->edge[i].src, graph->edge[i].dest, graph->edge[i].weight);
//
//    }
    
    Subset *subsets = (Subset *) malloc(sizeof(Subset)*v_g);
    
    for (int j = 0; j < v_g; j++){
        connected[j] = 0;
        subsets[j].parent = j;
        subsets[j].rank = 0;
        subsets[j].weight_sum = 0;
        subsets[j].cnt_vertices = 1;
    }
    
    while (e < (v_g -1) && idx < v_e){
        //step2: pick samallest edge;
        
        Edge next_edge = graph->edge[idx++];
        //idx++;
        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);
        
        if (x != y){
            result[e++] = next_edge;
            connected[next_edge.src] = 1;
            connected[next_edge.dest] = 1;
            //Union(subsets, x, y);
            Union(subsets, x, y, next_edge.weight);
        }
        
        
    }
    
    
    ///////// 여기까지 강의자료 참고 kruscalMST
    ///
    ///
    ///count connected
    /////c추가구현
    
    
    end = clock();
    double time = (double)(end-start);
    
    cout << "time: " << time<< endl;
    cout << "k_scanned: " << idx <<endl;
    
    int cnt_not_connexted;
    int cnt_connected;
    cnt_not_connexted = cnt_connected = 0;
    
    for (int j = 0; j < v_g; j++){
        if (connected[j] == 0) cnt_not_connexted+=1;
        if (subsets[j].parent == j) cnt_connected += 1;
    }
    cout << "\nconnected: " << cnt_connected << endl;
    cout << "not connected: " << cnt_not_connexted << endl;
    
    fprintf(fp_out, "%d\n",cnt_connected);
    
    for (int j = 0; j < v_g; j++) {
        if (subsets[j].parent == j) {
            fprintf(fp_out, "%d %lld\n", subsets[j].cnt_vertices, subsets[j].weight_sum);
        }
    }
        
        free(subsets);
        free(connected);
        free(result);
    
}

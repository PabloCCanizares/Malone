/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testVec.c
 * Author: Pablo C. Cañizares
 *
 * Created on August 28, 2018, 12:57 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kvec.h"
#include "Options.h"
#include "printers.h"
int test_vector() {
    int M = 3, N = 1000000, i, j;
    clock_t t;
    t = clock();
    for (i = 0; i < M; ++i) {
        int *array = (int*) malloc(N * sizeof (int));
        for (j = 0; j < N; ++j) array[j] = j;
        free(array);
    }
    printf("C array, preallocated: %.3f sec\n", (float) (clock() - t) / CLOCKS_PER_SEC);
    t = clock();
    for (i = 0; i < M; ++i) {
        int *array = 0, max = 0;
        for (j = 0; j < N; ++j) {
            if (j == max) {
                max = !max ? 1 : max << 1;
                array = (int*) realloc(array, sizeof (int)*max);
            }
            array[j] = j;
        }
        free(array);
    }
    printf("C array, dynamic: %.3f sec\n", (float) (clock() - t) / CLOCKS_PER_SEC);

    for (i = 0; i < M; ++i) {
        T_stExecutionStructure *array = (T_stExecutionStructure*) malloc(N * sizeof (T_stExecutionStructure));
        T_stExecutionStructure oStExec;
        for (j = 0; j < N; ++j) 
        {
            
            oStExec.nExecutionMode=j;
            oStExec.nMutantEnd=j;
            oStExec.nMutantInit=j;
            oStExec.nTestEnd=j;
            oStExec.nTestInit=j;
            oStExec.nWorker=j;
            array[j] = oStExec;
        }
        for(j = 0; j < N; ++j)
        {
            oStExec = array[j] ;
            printDeployStruct(&oStExec);
        }
        
        free(array);
    }
    printf("sizeof(T_stExecutionStructure): %d\n", (int) sizeof (T_stExecutionStructure));
    printf("sizeof(T_stMutant): %d\n", (int) sizeof (T_stMutant));
    printf("sizeof(T_stTestList): %d\n", (int) sizeof (T_stTestList));
    printf("sizeof(T_stTestInfo): %d\n", (int) sizeof (T_stTestInfo));
    printf("\n");
    printf("sizeof(T_stExecutionMap): %d\n", (int) sizeof (T_stExecutionMap));
    
    printf("C array with T_stExecutionStructure, preallocated: %.3f sec\n", (float) (clock() - t) / CLOCKS_PER_SEC);

    /*
    t = clock();
    for (i = 0; i < M; ++i) {
        kvec_t(int) array;
        kv_init(array);
        kv_resize(int, array, N);
        for (j = 0; j < N; ++j) 
            kv_a(int, array, j) = j;
        kv_destroy(array);
    }
    printf("C vector, dynamic (kv_a): %.3f sec\n", (float) (clock() - t) / CLOCKS_PER_SEC);
    t = clock();
    for (i = 0; i < M; ++i) {
        kvec_t(int) array;
        kv_init(array);
        for (j = 0; j < N; ++j) 
            kv_push(int, array, j);
        kv_destroy(array);
    }
    printf("C vector, dynamic (kv_push): %.3f sec\n", (float) (clock() - t) / CLOCKS_PER_SEC);
     */
    int nRet;
        kvec_t(int) array;
	kv_init(array);
	kv_push(int, array, 10); // append
	nRet = kv_a(int, array, 20); // dynamic
	kv_A(array, 20) = 4; // static
	kv_destroy(array);
        
    return 0;
}


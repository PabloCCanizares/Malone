/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   autotest.c
 * Author: Pablo C. Canizares
 *
 * Created on April 24, 2020, 8:58 PM
 */
#include "autotest.h"
#include "Auxiliars.h"
#include "MPI_Operations.h"
void autotest();

static
void mySignalCatcher(int n) {
    fprintf(stderr, "<%d> Control-C or similar caught, quitting.\n", m_nRank);

    printf("<%d>  SIGNAL (%d) catched during the execution of the program", m_nRank, n);
    MPI_Abort(MPI_COMM_WORLD,33);    
}
int main(int argc, char** argv)
{
    printf("Auto test v0.0 starting\n");
    
    //Configure signal catcher
    signal(SIGINT, mySignalCatcher);
    signal(SIGTERM, mySignalCatcher);
    signal(SIGHUP, mySignalCatcher);
    signal(SIGSEGV, mySignalCatcher);
    
    autotest();

    printf("End\n");
}

void autotest()
{
    //The main idea is to load a test file, and configure it
    //Depending on the size of the MPI environment, several tests will be conducted.
    initialize_auxiliars();
    
    //1 - Single process
    //  - Environment file load.
    //testEnvFile();
    
    //  - Configuration load.
    //testConfFile();
    
    //Testing command execution
    testCommand();
    
    //  - Auxiliar functions 
    
    //2 - Multiprocess
    initialize_auxiliars();
    initializeMPI();
    loadConfig();
    
    //  - Deploys
    testDeploy();
    
    // Tests 
    testTestcases();
    
    //Mutants
    testMutants();
           
    //3 - Test full executions
    //Distribution algorithms
    testDistributionAlgorithms();
    
    MPI_Finalize();
    
    //Lo mas importante y donde mas flaquea el proyecto es en las estructuras estaticas
    //Algun dia habra que echarle webs
}


/*
struct a_tag {
   char c;
   int i;
   char *w;
};
// return_ptr: return a pointer to a struct 
struct a_tag *return_ptr(void) {
   struct a_tag *p;
   p = (struct a_tag *) malloc(sizeof(struct a_tag));
   p -> c = 'Z'; // same as (*p).c = 'Z' 
   p -> i = 77;  // same as (*p).i = 77 
   p -> w = "Yours does too";
   return p;
}
struct TestInfo* createTestST99(int nIndexTest, char* strResult, double dTime, int nKill) {
    struct TestInfo* pTest;    

    pTest = (struct TestInfo*) malloc(sizeof (struct TestInfo));
    pTest->dTime = dTime;
    pTest->nKill = nKill;
    pTest->nTest = nIndexTest;
    bzero(pTest->res, MAX_RESULT_SIZE);

    if (strResult != NULL)
        strcpy(pTest->res, strResult);
    
    return pTest;
}

// print_a: print a struct where pointer is passed 
void print_p(int test_num, struct a_tag *p) {
   printf("Test %i, ", test_num);
   printf("Passing pointer, fields: c:%c, i:%3i, w:\"%s\"\n",
      p -> c, p -> i, p -> w);   p -> c same as (*p).c 
    increment below changes struct back in main 
   p->i++; /* same as (p->i)++ 
}
    struct a_tag *p3;
    p3 = return_ptr();
    print_p(8, p3);
 */

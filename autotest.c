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
void autotest_initialise();
void autotest();
void autotest_print_results();

void autotest_initialise()
{
    m_oAutoTests.nCategories = m_oAutoTests.nFail = m_oAutoTests.nPass = m_oAutoTests.nTotalTests = 0;
}
void autotest()
{
    printf("Auto test v1.0 starting\n");
   
    autotest_initialise();
    
    //The main idea is to load a test file, and configure it
    //Depending on the size of the MPI environment, several tests will be conducted.
    initialize_auxiliars();
    
    //1 - Single process
    //  - Environment file load.
    //testEnvFile();
    
    //  - Configuration load.
    //testConfFile();
    
    //Testing command execution
    //testCommand();
    
    //  - Auxiliar functions 
    
    //2 - Multiprocess
    //initialize_auxiliars();
    initializeMPI();
    loadConfig();
    
    //  - Deploys
    //testDeploy();
    
    // Tests 
    //testTestcases();
    
    //Mutants
    //testMutants();
           
    //3 - Test full executions
    //Distribution algorithms
    free_auxiliars();
    //initialize_auxiliars();
    //loadConfig();
    testDistributionAlgorithms();
    
    MPI_Finalize();
    
    //Lo mas importante y donde mas flaquea el proyecto es en las estructuras estaticas
    //Algun dia habra que echarle webs
    
    autotest_print_results();
    
    printf("End\n");
}

void autotest_print_results()
{
    if(m_nRank == 0)
    {
        printf("================================\n");
        printf("MALONE autotest v1.0\n");
        printf("Total categories: %d\n", m_oAutoTests.nCategories);
        printf("Total tests: %d\n", m_oAutoTests.nTotalTests);
        printf("Total pass: %d\n", m_oAutoTests.nPass);
        printf("Total fail: %d\n", m_oAutoTests.nFail);
        printf("================================\n");
    }
    
}

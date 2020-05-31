/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_autotest.c
 *     Source file of the MALONE autotest.
 * @par Purpose:
 *     Testing the different functionalities of MALONE. The test suite
 * starts with basic components, such as, just o name a few, command executor, datatypes, configuration,
 * and finishing with a combination of up to 60 distribution algorithms.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  24 Apr 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "Malone_autotest.h"
#include "Auxiliars.h"
#include "Malone_MPI_Operations.h"
void autotest_initialise();
void autotest();
void autotest_print_results();
#define CONF_ALLOC_FREE 100

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
    
    //TODO: otra categoria de tests. De estres o lo que sea
    for(int i=0;i<CONF_ALLOC_FREE;i++)
    {
        printf("Testing init/free method - %d\n", i);
        initialize_auxiliars();
        free_auxiliars();
    }
    
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
    testMutants();
           
    //3 - Test full executions
    //Distribution algorithms
    free_auxiliars();
    //initialize_auxiliars();
    //loadConfig();
    testDistributionAlgorithms();
    
    MPI_Finalize();
    
    //Lo mas importante y donde mas flaquea el proyecto es en las estructuras estaticas
    //Algun dia habra que echarle webs. Llego el dia y se lo echamos! Quitamos el limite! 24/05/2020
    
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

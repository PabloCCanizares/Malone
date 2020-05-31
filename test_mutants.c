/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file test_mutants.c
 *     Source file of the mutants test suite.
 * @par Purpose:
 *     Testing the mutants functionality.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  10 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "Malone_printers.h"
#include "randomElements.h"
#include "Malone_MPI_Operations.h"
#include "test_mutants.h"
#include "Auxiliars.h"

void testMutants()
{
    printf("Test [testMutants] - Testing Mutants start\n");
    test_mutants_1();
    //TODO: test_mutants_2() enviando VARIOS mutantes, la idea es comprobar EL INTERIOR de los mutantes, si esta vivo etc.
    m_oAutoTests.nCategories++;
    printf("Test [testMutants] - End\n");
}
void test_mutants_1()
{    
    int nRemainingBlocks, nWorkerSource;
    T_stMutant* pMutant;
    T_stExecutionStructure exeVector[MAX_WORKERS];
    
    printf("Test [test_mutants_1] - Init\n");
    MPI_Barrier(MPI_COMM_WORLD);
    if(m_nRank == 0)
    {
        nRemainingBlocks = m_nSize -1;
        for(int i=0;i<MAX_WORKERS;i++)
        {
            exeVector[i].nMutantInit = 0;
            exeVector[i].nMutantEnd = 33;
            exeVector[i].nTestInit = 0;
            exeVector[i].nTestEnd = 33;
        }
        
        for(int i=1;i<m_nSize;i++)
        {
            sendDeployMode(&exeVector[0], i);
        }      
        
        do
        {        
            pMutant = receiveMutantAnySource(&nWorkerSource);
            
            assert(pMutant != NULL);
            
            if(pMutant != NULL)
                free_mutant(pMutant);
            
            nRemainingBlocks--;
            printf("%d\n", nRemainingBlocks);
        }while(nRemainingBlocks);
    }
    else
    {
        T_stMutant* pMutant;
        T_stExecutionStructure* pExeRetMode;
        
        pExeRetMode = receiveDeployMode();
         
        if(pExeRetMode != NULL)
        {
            pMutant = generateRandomMutant(MAX_TESTS);
            printMutant((T_stMutant*) pMutant);
            sendMutant(pMutant, MALONE_MASTER);
            
            if(pMutant != NULL)
                free_mutant(pMutant);            
        }
        else
        {
            printf("Test [test_mutants_1] - ERROR receiving deploymode\n");
        }        
    }
    m_oAutoTests.nPass++;
    m_oAutoTests.nTotalTests++;
    printf("Test [test_mutants_1] - End\n");
}

//Probar sendmutants (en plural)
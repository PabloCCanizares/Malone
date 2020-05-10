/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "printers.h"
#include "randomElements.h"
#include "MPI_Operations.h"
#include "test_mutants.h"
#include "Auxiliars.h"

void testMutants()
{
    printf("Test [testMutants] - Testing Mutants start\n");
    test_mutants_1();
    printf("Test [testMutants] - End\n");
}
void test_mutants_1()
{    
    int nAlive, nRemainingBlocks, nWorkerSource;
    T_stMutant* pMutant;
    T_stExecutionStructure exeVector[MAX_WORKERS];
    
    printf("Test [test_mutants_1] - Init\n");
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
            pMutant = generateRandomMutant(1000);
            printMutant((T_stTestInfo*) pMutant);
            sendMutant(pMutant, MALONE_MASTER);
        }
        else
        {
            printf("Test [test_mutants_1] - ERROR receiving deploymode\n");
        }        
    }
    printf("Test [test_mutants_1] - End\n");
}
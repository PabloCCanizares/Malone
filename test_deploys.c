/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file test_deploys.h
 *     Source code of the test suite for checking the deployment of an execution 
 * structure used in the distributed algorithms in MALONE.
 * @par Purpose:
 *     Testing the execution structures.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  3 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "Malone_autotest.h"
#include "test_deploys.h"
#include "Malone_MPI_Operations.h"
#include "Malone_printers.h"

void testDeploy()
{
    printf("Test [testDeploy] - Init\n");
    test_deploy_1();
    test_deploy_2();
    test_deploy_3();
    test_deploy_4();
    
    m_oAutoTests.nCategories++;
    printf("Test [testDeploy] - Init\n");
}

void test_deploy_1()
{
    T_stExecutionStructure* pExeRetMode;
    T_stExecutionStructure* pExeVector;    
    
    printf("Test [test_deploy_1] - Init\n");
    
    if(m_nRank == 0)
    {       
        //Wait for receiving values from all workers and filling final result
        pExeVector = (T_stExecutionStructure*)  malloc(sizeof(T_stExecutionStructure));
        
            
        for(int i=1;i<m_nSize;i++)
        {
            pExeVector->nExecutionMode = 1;
            pExeVector->nMutantInit = i;
            pExeVector->nMutantEnd = i+1;
            pExeVector->nTestInit = 0;
            pExeVector->nTestEnd = 10;
            sendDeployMode(pExeVector, i);                            
        }
        free(pExeVector);
    }
    else
    {
        int exeParams[NUM_DEPLOY_PARAMS];

        MPI_Status status;                    
    
        printf("<%d> Worker, waiting for receiving a deploy", m_nRank);
        
        MPI_Recv(&exeParams, 1, m_DeployType, 0, 0, MPI_COMM_WORLD, &status);
        
        pExeRetMode = (T_stExecutionStructure*) malloc(sizeof (T_stExecutionStructure));
        pExeRetMode->nExecutionMode = exeParams[0];
        pExeRetMode->nMutantInit = exeParams[1];
        pExeRetMode->nMutantEnd = exeParams[2];
        pExeRetMode->nTestInit = exeParams[3];
        pExeRetMode->nTestEnd = exeParams[4];
        
        
        assert(pExeRetMode != NULL);        
        printf(" DP %d, %d\n", pExeRetMode->nExecutionMode, pExeRetMode->nMutantInit);
    }
    m_oAutoTests.nPass++;
    m_oAutoTests.nTotalTests++;
    printf("T1 [test_deploy_1] - End\n");    
}

void test_deploy_2()
{
    T_stExecutionStructure* pExeVector;    
    
    printf("Test [test_deploy_2] - Init\n");
    if(m_nRank == 0)
    {
       
        //Wait for receiving values from all workers and filling final result
        pExeVector = (T_stExecutionStructure*)  malloc(sizeof(T_stExecutionStructure));

        for(int i=1;i<m_nSize;i++)
        {
            pExeVector->nExecutionMode = 1;
            pExeVector->nMutantInit = i;
            pExeVector->nMutantEnd = i+1;
            pExeVector->nTestInit = 0;
            pExeVector->nTestEnd = 10;
            sendDeployMode(pExeVector, i);                            
        }
        free(pExeVector);
        
    }
    else
    {            
        printf("<%d> Worker, waiting for receiving a deploy", m_nRank);               
        T_stExecutionStructure oExeRetMode;
        receiveDeployModeRef(&oExeRetMode);
                
        printf("Test [test_deploy_2] - T2 DP %d, %d\n", oExeRetMode.nExecutionMode, oExeRetMode.nMutantInit);
    }
    m_oAutoTests.nPass++;
    m_oAutoTests.nTotalTests++;
    printf("Test [test_deploy_2] - End\n"); 
}
void test_deploy_3()
{    
    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;
    T_stExecutionStructure* pExeVector;    
    
    printf("Test [test_deploy_3] - Init\n");
    
    if(m_nRank == 0)
    {
        //Wait for receiving values from all workers and filling final result
        pExeVector = (T_stExecutionStructure*)  malloc(sizeof(T_stExecutionStructure));
        
        for(int i=1;i<m_nSize;i++)
        {
            pExeVector->nExecutionMode = 1;
            pExeVector->nMutantInit = i;
            pExeVector->nMutantEnd = i+1;
            pExeVector->nTestInit = 0;
            pExeVector->nTestEnd = 10;
            sendDeployMode(pExeVector, i);                            
        }
        free(pExeVector);               
    }
    else
    {
        printf("<%d> Worker, waiting for receiving a deploy", m_nRank);        
               
        receiveDeployModeRefP(&pExeRetMode);
        
        assert(pExeRetMode != NULL);
        printf("T3 DP %d, %d\n", pExeRetMode->nExecutionMode, pExeRetMode->nMutantInit);
    }
    m_oAutoTests.nPass++;
    m_oAutoTests.nTotalTests++;
    printf("Test [test_deploy_3] - End\n"); 
}
void test_deploy_4()
{
    int oIndex[MAX_WORKERS];    
    int nTotalRemaining, nRemWork, nIndex, nInit, nEnd, nNumReceives, nTotalSent;
    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;
    T_stExecutionStructure* pExeVector;    
    
    printf("Test [test_deploy_4] - Init\n");
    if(m_nRank == 0)
    {
        nRemWork = m_nSize-1;
        nTotalRemaining = 20;
        nIndex = 0;
        nEnd = nNumReceives = nTotalSent = 0;
        
        while (nTotalRemaining > 0 && nRemWork>0) {
            oIndex[nIndex] = ceil(nTotalRemaining / nRemWork);
            nTotalRemaining -= ceil(nTotalRemaining / nRemWork);
            nIndex++;
            nRemWork--;
        }
        printDeployArray(oIndex, m_nSize);
        
        printf("Test [test_deploy_4] - 1\n");
        
        //Fill the env values to send it to workers
        for (int i = 1; i < m_nSize; i++) {
            nEnd += oIndex[i];
            exeVector[i].nMutantInit = nInit;
            exeVector[i].nMutantEnd = nEnd;
            exeVector[i].nTestInit = 0;
            
            if(m_stEnvValues != NULL)
                exeVector[i].nTestEnd = m_stEnvValues->nTotalTests - 1;
            else
                exeVector[i].nTestEnd = 10;

            printf("Test [test_deploy_4] - 1.5\n");
            pExeVector = (T_stExecutionStructure*)  malloc(sizeof(T_stExecutionStructure));
            pExeVector->nMutantInit = nInit;
            pExeVector->nMutantEnd = nEnd;
            pExeVector->nTestInit = nEnd;
            
             if(m_stEnvValues != NULL)
                exeVector[i].nTestEnd = m_stEnvValues->nTotalTests - 1;
            else
                exeVector[i].nTestEnd = 10;
            
            sendDeployMode(pExeVector, i);                    
            nInit = nEnd;
        }        
       printf("Test [test_deploy_4] - 2\n");
    }
    else
    {
        printf("<%d> Worker, waiting for receiving a deploy", m_nRank);
         
        receiveDeployModeRefP(&pExeRetMode);

        
        assert(pExeRetMode != NULL);
        printf("Test [test_deploy_4] - T4 DP %d, %d\n", pExeRetMode->nExecutionMode, pExeRetMode->nMutantInit);
    }
    m_oAutoTests.nPass++;
    m_oAutoTests.nTotalTests++;
    printf("Test [test_deploy_4] - End\n"); 
}
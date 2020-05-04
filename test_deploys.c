/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "autotest.h"
#include "test_deploys.h"
#include "MPI_Operations.h"
#include "printers.h"

void testDeploy()
{
    printf("Test [testDeploy] - Init\n");
    test_deploy_1();
    test_deploy_2();
    test_deploy_3();
    test_deploy_4();
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
        pExeVector->nExecutionMode = 1;
        pExeVector->nMutantInit = 0;
        pExeVector->nMutantEnd = 1;
        pExeVector->nTestInit = 0;
        pExeVector->nTestEnd = 10;
            
        sendDeployMode(pExeVector, 1);                            
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
        pExeVector->nExecutionMode = 1;
        pExeVector->nMutantInit = 0;
        pExeVector->nMutantEnd = 1;
        pExeVector->nTestInit = 0;
        pExeVector->nTestEnd = 10;
            
        sendDeployMode(pExeVector, 1);                            
        
    }
    else
    {            
        printf("<%d> Worker, waiting for receiving a deploy", m_nRank);               
        T_stExecutionStructure oExeRetMode;
        receiveDeployModeRef(&oExeRetMode);
                
        printf("T2 DP %d, %d\n", oExeRetMode.nExecutionMode, oExeRetMode.nMutantInit);
    }
    printf("Test [test_deploy_2] - End\n"); 
}
void test_deploy_3()
{
    int oIndex[MAX_WORKERS];    
    int nTotalRemaining, nRemWork, nIndex, nInit, nEnd, nNumReceives, nTotalSent;
    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;
    T_stExecutionStructure* pExeVector;    
    
    printf("Test [test_deploy_3] - Init\n");
    
    if(m_nRank == 0)
    {
        //Wait for receiving values from all workers and filling final result
        pExeVector = (T_stExecutionStructure*)  malloc(sizeof(T_stExecutionStructure));
        pExeVector->nExecutionMode = 1;
        pExeVector->nMutantInit = 0;
        pExeVector->nMutantEnd = 1;
        pExeVector->nTestInit = 0;
        pExeVector->nTestEnd = 10;
            
        sendDeployMode(pExeVector, 1);                
    }
    else
    {
        printf("<%d> Worker, waiting for receiving a deploy", m_nRank);        
               
        receiveDeployModeRefP(&pExeRetMode);
        
        assert(pExeRetMode != NULL);
        printf("T3 DP %d, %d\n", pExeRetMode->nExecutionMode, pExeRetMode->nMutantInit);
    }
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
        printDeployArray(&oIndex, m_nSize);
        
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
       /* do {
            receiveMd5Mutants(&exeVector);
            int nMutants;
            MPI_Status status;
            
             MPI_Recv(&nMutants, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            nNumReceives++;
        } while (nNumReceives < (m_nSize - 1)); // Decreasing in 1, due to the master*/
        
    }
    else
    {
        printf("<%d> Worker, waiting for receiving a deploy", m_nRank);
         
        receiveDeployModeRefP(&pExeRetMode);

        
        assert(pExeRetMode != NULL);
        printf("T4 DP %d, %d\n", pExeRetMode->nExecutionMode, pExeRetMode->nMutantInit);
    }
    printf("Test [test_deploy_4] - End\n"); 
}
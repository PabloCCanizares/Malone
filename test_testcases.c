/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "printers.h"
#include "randomElements.h"
#include "MPI_Operations.h"
#include "test_testcases.h"
#include "Auxiliars.h"

void testTestcases()
{
    printf("Test [testTestcases] - Testing TCs start\n");
    test_testcase_0();
    test_testcase_1();
    test_testcase_2();
    printf("Test [testTestcases] - End\n");
}

void test_testcase_0()
{
    struct TestInfo* pTest;
    
    printf("Test [test_testcase_0] - Init\n");
    
    if(m_nRank == 0)
    {
        pTest = (struct TestInfo*) createTestST(0, "Test BRO!\n", 0.1,1);
        printTest(pTest);
        sendTest(pTest,1);
    }
    else
    {
        receiveTestRefP(0, &pTest);
        
        printTest(pTest);
    }
    printf("Test [test_testcase_0] - End\n");
}

void test_testcase_1()
{
    T_stTestInfo* pTest;
    
    printf("Test [test_testcase_1] - Init\n");
    
    if(m_nRank == 0)
    {
        createTestRefP(0, "Test BRO!\n", 0.1,1, &pTest);
        printTest(pTest);
        sendTest(pTest,1);
    }
    else
    {
        receiveTestRefP(0, &pTest);
        
        printTest(pTest);
    }
    printf("Test [test_testcase_1] - End\n");
}

void test_testcase_2()
{    
    int nAlive, nWorkerSource;
    T_stExecutionStructure exeVector[MAX_WORKERS];
    
    printf("Test [test_testcase_2] - Init\n");
    if(m_nRank == 0)
    {
        
        for(int i=0;i<MAX_WORKERS;i++)
        {
            exeVector[i].nMutantInit = 0;
            exeVector[i].nMutantEnd = 33;
            exeVector[i].nTestInit = 0;
            exeVector[i].nTestEnd = 33;
        }
        
                
        sendDeployMode(&exeVector[0], 1);
        
        nAlive = receiveSingleTestAndCheck(&exeVector, &nWorkerSource);
                
    }
    else
    {
        T_stTestInfo*pTest;
        T_stExecutionStructure* pExeRetMode;
        
        pExeRetMode = receiveDeployMode();
         
        if(pExeRetMode != NULL)
        {
            pTest = generateRandomTest(0);
            sendTest(pTest, MALONE_MASTER);
        }
        else
        {
            printf("Test [test_testcase_2] - ERROR receiving deploymode\n");
        }
        
    }
    printf("Test [test_testcase_2] - End\n");
}
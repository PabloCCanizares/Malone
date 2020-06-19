/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file test_testcases.c
 *     Source file of the test suite tests.
 * @par Purpose:
 *     Testing the test case functionality.
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

#include "test_testcases.h"


void testTestcases()
{
    printf("Test [testTestcases] - Testing TCs start\n");
    test_testcase_0();
    test_testcase_1();
    test_testcase_2();
    
    m_oAutoTests.nCategories++;
    printf("Test [testTestcases] - End\n");
}

void test_testcase_0()
{
    T_stTestInfo* pTest;
    
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Test [test_testcase_0] - Init\n");
    
    if(m_nRank == 0)
    {
        createTestRefP(0, "Test BRO!\n", 0.1,1, &pTest);
        printTest((T_stTestInfo*)pTest);
        
        for(int i=1;i<m_nSize;i++)
        {
            sendTest((T_stTestInfo*)pTest,i);
        }
    }
    else
    {
        receiveTestRefP(0, (T_stTestInfo**)&pTest);
        
        printTest((T_stTestInfo*) pTest);
    }
    m_oAutoTests.nPass++;
    m_oAutoTests.nTotalTests++;
    printf("Test [test_testcase_0] - End\n");
}

void test_testcase_1()
{
    T_stTestInfo* pTest;
    
    printf("Test [test_testcase_1] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    if(m_nRank == 0)
    {
        createTestRefP(0, "Test BRO!\n", 0.1,1, &pTest);
        printTest(pTest);
        
        for(int i=1;i<m_nSize;i++)
        {
            sendTest(pTest,i);
        }
        
    }
    else
    {
        receiveTestRefP(0, &pTest);
        
        printTest(pTest);
    }
    m_oAutoTests.nPass++;
    m_oAutoTests.nTotalTests++;
    printf("Test [test_testcase_1] - End\n");
}

void test_testcase_2()
{    
    int nAlive, nRemainingBlocks, nWorkerSource;
    T_stExecutionStructure exeVector[MAX_WORKERS];
    
    printf("Test [test_testcase_2] - Init\n");
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
            nAlive = receiveSingleTestAndCheck(exeVector, &nWorkerSource);
            nRemainingBlocks--;
            printf("%d\n", nRemainingBlocks);
        }while(nRemainingBlocks);
    }
    else
    {
        T_stTestInfo*pTest;
        T_stExecutionStructure* pExeRetMode;
        
        pExeRetMode = receiveDeployMode();
         
        if(pExeRetMode != NULL)
        {
            pTest = generateRandomTest(0);
            printTest((T_stTestInfo*) pTest);
            sendTest(pTest, MALONE_MASTER);
        }
        else
        {
            printf("Test [test_testcase_2] - ERROR receiving deploymode\n");
        }        
    }
    m_oAutoTests.nPass++;
    m_oAutoTests.nTotalTests++;
    printf("Test [test_testcase_2] - End\n");
}
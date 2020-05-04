#include "Options.h"
#include "printers.h"
#include "MPI_Operations.h"
#include "randomElements.h"
void testDataType(int choice)
{
    int i;
    int nValue;
    printf("<%d>testDataType - Init\n", m_nRank);        
    MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    //
    
    nValue =33;
    MPI_Send(&nValue, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    
    //    
    T_stTestInfo oTest;
    oTest.dTime = 66.6;
    oTest.nKill = 1;
        
    printf("<%d>testDataType - Testing single send...\n", m_nRank);
    
    strcpy(oTest.res, "EHH!");
    sendTest(&oTest, 1);
    printf("<%d>testDataType - sent...\n", m_nRank);
    
    printf("<%d>testDataType - Waiting barrier!!\n", m_nRank);
    MPI_Barrier(MPI_COMM_WORLD);
    
    printf("<%d>testDataType - Test multiple send\n", m_nRank);
    T_stTestList* pList = generateRandomTestList_r(3);            
    printTestResults(pList);
    sendOriginalTestResults(pList);
    
    printf("<%d>testDataType - Testing the single mode\n", m_nRank);
    for(i = 0;i<2;i++)
    {
        printf("<%d>testDataType - Sending a random test case: %d\n", i, m_nRank);
        T_stTestInfo* pTest = generateRandomTest(i);
        sendTest(pTest,1);
    }
    
    //
    nValue =33;
    MPI_Send(&nValue, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("<%d>testDataType - End\n", m_nRank);
}

void testDataType_w()
{
    printf("<%d>testDataType_w - Init!! \n", m_nRank);
    T_stTestInfo* pTest;     
    T_stTestList* ptList;
    T_stExecutionStructure oExecution;
    int i, nWorker;
    int nRecvCode;
    MPI_Status status;  
    int nValue;
    
    nRecvCode = MPI_Recv(&nValue, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&status);     
    printf("<%d>testDataType - Receiving a test from %d with value: %d| code: %d | error: %d\n", m_nRank, status.MPI_SOURCE, nValue, nRecvCode, status.MPI_ERROR);
    
   
    printf("<%d>testDataType_w - Test single receive\n", m_nRank);
    pTest = receiveTest(0);
    printf("<%d>testDataType_w - Moreover!!\n", m_nRank);
    sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);
    
    ptList =  (T_stTestList*)receiveOriginalTestResults_r(); 
    printTestResults(ptList);
    
    
    printf("<%d>testDataType - Testing the single mode\n", m_nRank);
    for(i = 0;i<2;i++)
    {
        oExecution.nExecutionMode = 4;
        oExecution.nTestInit = i;
        oExecution.nTestEnd = i;        
        oExecution.nMutantInit = 1;
        oExecution.nMutantEnd = 1;
    
        printf("<%d>testDataType - Receiving a random test case: %d\n", i, m_nRank);
        receiveSingleTestAndCheck(&oExecution, &nWorker);  
        printf("<%d>testDataType - Received from <%d>\n", nWorker, m_nRank);
        printTest(pTest);
    }
    

    nRecvCode = MPI_Recv(&nValue, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);     
    printf("<%d>testDataType - Receiving a test from %d with value: %d| code: %d | error: %d\n", m_nRank, status.MPI_SOURCE, nValue, nRecvCode, status.MPI_ERROR);
    
    printf("testDataType_w - End!! \n");
}        
void mutantDataType(int choice)
{
    printf("MUTANT MPI_DataTypes!! \n");
    MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
    T_stMutant* pMut = generateRandomMutant(10);
    printfMutantFull(pMut);
    sendMutant(pMut,1);
}
void mutantDataType_w()
{
    printf("MUTANT MPI_DataTypes!! \n");
    T_stMutant* pMutant;                
    pMutant = receiveMutant(0);
    printfMutantFull(pMutant);
}

void testSendRecv()
{
    MPI_Status status;
    int number, numberRcv;
    if (m_nRank == 0) {
        printf("------------------------------------------\n");
        printf("TestId: SencRcv\n");
        number = 20994;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else{
        MPI_Recv(&numberRcv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 &status);
        printf("Process %d received number %d from process %d\n",m_nRank, numberRcv, status.MPI_SOURCE);
        
        if(numberRcv == 20994)
            printf("Test OK\n");
        else
            printf("Test FAIL!\n");
        
        printf("------------------------------------------\n");
    }
}
void createIndexVector(T_stTestList** pResList, int  vector[])
{
    T_stTestList* pList = *pResList;
    T_stTestInfo* pTest;
    int nTest, nIndex, i;
    if(pList)
    {
        for(i=0;i<pList->nElems;i++)
        {
            pTest = pList->tests[i];
            nTest = pTest->nTest;
            vector[nTest] =i;        
        }               
    }

}
void createSortIndexVector(T_stTestList** pResList, int  vector[])
{
    T_stTestList* pList = *pResList;
    T_stTestInfo* pTest;
    int nTest, nIndex, i;
    if(pList)
    {
        for(i=0;i<pList->nElems;i++)
        {
            pTest = pList->tests[i];
            nTest = pTest->nTest;
            vector[i] =nTest;        
        }               
    }

}
//TODO: Lo suyo es hacer un mergesort.
void reorderTestSuite(T_stTestList** pResList)
{
    int nCont, nIndex,i,j;
    T_stTestList* pAuxList= malloc(sizeof(T_stTestList));

    nIndex = 0;
    nCont = 1;
    pAuxList->nElems=0;
    pAuxList->nMax = MAX_MUTANTS;
    
    for(i=0;i<(*pResList)->nElems;i++)
    {
        T_stTestInfo* pTest=(*pResList)->tests[i];
        if(i==0)
        {
            pAuxList->tests[i]=pTest;
            pAuxList->nElems++;
        }
        else
        {
            nCont=1;
            nIndex=0;
            while(nIndex<pAuxList->nElems && nCont)
            {
                T_stTestInfo* pTestIndex = pAuxList->tests[nIndex];
                
                if(pTest->dTime<pTestIndex->dTime)
                    nCont = 0;
                else
                    nIndex++;
            }

            if(nCont == 0)
            {
                //Reorder
                for(j=pAuxList->nElems; j>nIndex;j--)
                {
                    pAuxList->tests[j]=pAuxList->tests[j-1];
                }
        
                //printf("<%d> nElems: %d | nIndex: %d\n", i, pAuxList->nElems, nIndex);                
            }
            else
            {
                //pAuxList->tests[nIndex+1]=pTest;
                //printf("·<%d> nElems: %d | nIndex: %d\n", i, pAuxList->nElems, nIndex);
            }
            
            pAuxList->tests[nIndex]=pTest;
            pAuxList->nElems++;            
        }
    }
    //printTestResultsSort(pAuxList);
    *pResList = pAuxList;
    
}

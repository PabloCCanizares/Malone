#include "Options.h"
#include "Auxiliars.h"
#include <time.h>
#include <stdlib.h>

T_stTestInfo* generateRandomTest(int nTest)
{
    T_stTestInfo* pRet;
    char buffer [50];
    srand(time(NULL));
    
    pRet = malloc(sizeof(T_stTestInfo));
    pRet->nTest = nTest;
    pRet->dTime = rand();
    pRet->nKill = rand();
    bzero(&buffer,50);
    sprintf (buffer, "data_%d", rand());
    strcpy(pRet->res, buffer);
            
    return pRet;
}
T_stTestInfo* createTestTrap(int nIndexTest, char* strResult, double dTime, int nKill)
{
    T_stTestInfo* pTest;   
    
    pTest = (T_stTestInfo*)malloc(sizeof(T_stTestInfo));
    pTest->dTime = dTime;
    pTest->nKill = nKill;
    pTest->nTest = nIndexTest;
    bzero(pTest->res, MAX_RESULT_SIZE);
    
    if(strResult != NULL)
        strcpy(pTest->res, strResult);
    
    return pTest;
}
T_stTestInfo* generateTestTrap(int nTest, char* strResult)
{
    T_stTestInfo* pRet;
    char buffer [50];
    srand(time(NULL));
    
    pRet = malloc(sizeof(T_stTestInfo));
    pRet->nTest = nTest;
    pRet->dTime = rand();
    pRet->nKill = rand();
    //bzero(&buffer,50);
    //sprintf (buffer, "data_%d", rand());
    bzero(pRet->res,50);
    strcpy(pRet->res, strResult);
            
    return pRet;
}
void doingTraps()
{
    T_stTestInfo* pTest;
    int nSize,i;
    char* strResult;
    
    nSize = 100000;
    for(i=0;i<nSize;i++)
    {
        strResult = malloc(sizeof(char)*5);
        strcpy(strResult, "#__#"); 
        //pTest = generateRandomTest(i);//createTest(i, strResult, 0.0, 0);
        pTest = createTest(i, strResult, 0.0, 0);
        //pTest = createTestTrap(i, strResult, 0.0, 0);
        //pTest = generateTestTrap(i, "#__#");
        
        if(pTest)
        {
               if(pTest != NULL)
                   printf("Test - N:%d | Res:%s | Time: %lf | Killed<%d>\n",pTest->nTest, pTest->res, pTest->dTime, pTest->nKill);
               else
                   printf("Test - N:%d | Res:%s | Time: %lf | Killed<%d>\n",pTest->nTest, "NULL", pTest->dTime, pTest->nKill);
        }
        free(pTest);
    }
    
    printf("Dio!!\n");
    printf("Total Tests processed: %d", nSize);
}
T_stTestList* generateRandomTestList_r(int nTests)
{
    T_stTestList* pTestList;
    int i;
    
    pTestList = (T_stTestList*)malloc(sizeof(T_stTestList));
    pTestList->nElems = nTests;
    for(i=0;i<nTests;i++)
    {
        pTestList->tests[i] = generateRandomTest(i);
    }
    
    return pTestList;
}
void generateRandomTestList(T_stTestList* pTestList, int nTests)
{
    int i;
    pTestList->nElems = nTests;
    for(i=0;i<nTests;i++)
    {
        pTestList->tests[i] = generateRandomTest(i);
    }
}
T_stMutant* generateRandomMutant(int nTests)
{
    T_stMutant* pMutant;
    
    pMutant = malloc(sizeof(T_stMutant));
    pMutant->nNumber = 1;
    pMutant->nState=2;
    generateRandomTestList(&pMutant->oTestList,nTests);
    printf("generateRandomMutant done!");
    
    return pMutant;
}


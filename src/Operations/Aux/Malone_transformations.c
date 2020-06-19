/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_transformations.h
 *     Source file of transformation operations.
 *     MALONE 
 * @par Purpose:
 *     Provides the data structure and operations needed to perform the transformations
 * between tests/mutants and its reduced versions.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  27 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#include "Malone_transformations.h"

#ifdef DISABLELOGS
#define DEBUG_TRANS 0
#else
#define DEBUG_TRANS isEnabledTransLog()
#endif

T_stMutant* redMut2Mut(T_stM* pReducedMut)
{
    T_stMutant* pRet;
    int i;
    
    if(DEBUG_TRANS) printf("<%d>redMut2Mut - Init\n", m_nRank);
    
    pRet = malloc(sizeof(T_stMutant));
    pRet->nNumber = pReducedMut->nNumber;
    pRet->nState = pReducedMut->nState;
    pRet->nTestKiller =  pReducedMut->nTestKiller;    
    pRet->oTestList.nElems = pReducedMut->nTests;            
    pRet->strDescription = NULL;                    //the reduced mutant does not have description
    
    for(i = 0;i<pRet->oTestList.nElems;i++)
    {        
        pRet->oTestList.tests[i] = malloc(sizeof(T_stTestInfo));
        pRet->oTestList.tests[i]->nKill = pReducedMut->oKill[i]; 
        pRet->oTestList.tests[i]->nTest = pReducedMut->oTest[i]; 
        pRet->oTestList.tests[i]->dTime = pReducedMut->oTime[i];                
        
        //Reset
        pReducedMut->oKill[i] = -1;
        pReducedMut->oTest[i] = -1;
        pReducedMut->oTime[i] = -1;
        pReducedMut->nTests = 0;
        pReducedMut->nState = 1;
    }     
    if(DEBUG_TRANS) printMutant(pRet);
        
    if(DEBUG_TRANS) printf("<%d>redMut2Mut - End\n", m_nRank);
    
    return pRet;
}
void testList2redTestList(T_stTI* oTest, T_stTestList* pTestList)
{
    if(DEBUG_TRANS) printf("testList2redTestList - Init\n");
    int i;
    
    if(pTestList)
    {
        for(i=0;i<pTestList->nElems;i++)
        {
            test2redTest(pTestList->tests[i], oTest+i);
        }
    }
    if(DEBUG_TRANS) printf("testList2redTestList - End\n");
}
void test2redTest(T_stTestInfo* pTest, T_stTI* pTestRed)
{
    if(DEBUG_TRANS) printf("<%d>test2redTest - Init\n", m_nRank);
    if(pTest && pTestRed)
    {        
        //copy the values!!
        bzero(pTestRed->res, MAX_RESULT_SIZE);
        strcpy(pTestRed->res, pTest->res);   
        
        pTestRed->dTime = pTest->dTime;
        pTestRed->nKill = pTest->nKill;                     
        pTestRed->nTest = pTest->nTest;
    }
    if(DEBUG_TRANS) printf("<%d>test2redTest - End\n", m_nRank);
}
void redtestList2TestList(T_stTI oTest[],  T_stTestList* pList, int nItems)
{
    int i;
    if(DEBUG_TRANS) printf("redtestList2TestList - Init\n");
        
    if(oTest && pList)   
    {
        pList->nElems = nItems;
        for(i=0;i<pList->nElems;i++)
        {
            pList->tests[i] = redtest2Test(&oTest[i]);
        }
    }
    printTestResults(pList);
    if(DEBUG_TRANS) printf("redtestList2TestList - End\n");
}

T_stTestList* redtestList2TestList_r(T_stTI oTest[],  int nItems)
{
    int i;
    T_stTestList* pTestList;
    if(DEBUG_TRANS) printf("redtestList2TestList_r - Init\n");
        
    if(oTest)   
    {
        pTestList = malloc(sizeof(T_stTestList));
        pTestList->nElems = nItems;
        
        for(i=0;i<pTestList->nElems;i++)
        {
            pTestList->tests[i] = redtest2Test(&oTest[i]);    //TODO: \pabloSays{Por dios!!!!!!!!}
        }
    }
    if(DEBUG_TRANS) printf("redtestList2TestList_r - End\n");
    if(DEBUG_TRANS) printTestResults(pTestList);
    
    return pTestList;
}
T_stTestInfo*  redtest2Test(T_stTI* pTestRed)
{
    T_stTestInfo* pTest;
    
    if(DEBUG_TRANS) printf("redtest2Test - Init\n");
    
    if(pTestRed)
    {
        pTest = malloc(sizeof(T_stTestInfo));
        //copy the values!!
        bzero(pTest->res, MAX_RESULT_SIZE);
        strcpy(pTest->res, pTestRed->res);

        pTest->dTime = pTestRed->dTime;
        pTest->nKill = pTestRed->nKill;                     
        pTest->nTest = pTestRed->nTest;
    }
    else
        printf("testred2Test - Empty pointer\n!!");
    
    if(DEBUG_TRANS) printf("testred2Test - End\n");
    
    return pTest;
}
int isEnabledTransLog()
{   
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugTrans : 0;
}
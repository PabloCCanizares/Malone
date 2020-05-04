#include "printers.h"


#ifdef DISABLELOGS
#define PRINTER_ON 0
#define PRINTER_RES_FINAL 0
#define PRINTER_RES_INITIAL 0
#else
#define PRINTER_RES_FINAL 1
#define PRINTER_RES_INITIAL 1
#define PRINTER_ON isEnabledPrinterLog()
#endif


void printTestResults(T_stTestList* pList) 
{
    if(PRINTER_ON || PRINTER_RES_INITIAL)
    {
        int i;
        printf("<%d>printTestResults - Init\n", m_nRank);
        if(pList != NULL)
        {
            for(i=0;i<pList->nElems;i++)
            {
                T_stTestInfo* pTest = pList->tests[i];

                if(pTest != NULL)
                {
                    printf("[%d] %s : %lf <%d>\n",i,pTest->res, pTest->dTime, pTest->nKill);
                }
                else
                {
                    printf("[%d] WARING NULL test!!!\n",i);
                }
            }
        }
        else
            printf("<%d>printTestResults - WARNING!! Empty list!\n", m_nRank);    
        
        printf("<%d>printTestResults - End\n", m_nRank);
    }
}
void printTestResultsSort(T_stTestList* pList) 
{
    if(PRINTER_ON || PRINTER_RES_INITIAL)
    {
        int i;
        printf("printTestResults - Init\n");
        if(pList != NULL)
        {
            for(i=0;i<pList->nElems;i++)
            {
                T_stTestInfo* pTest = pList->tests[i];

                if(pTest != NULL)
                {
                    printf("[%d - %d] %s : %lf <%d>\n",i,pTest->nTest,pTest->res, pTest->dTime, pTest->nKill);
                }
                else
                {
                    printf("[%d] WARING NULL test!!!\n",i);
                }

            }
        }
        else
            printf("printTestResults - WARNING!! Empty list!\n");    
        printf("printTestResults - End\n");
    }

}
void printIndexVector(int  vector[], int nElems)
 {
     int i;
     if(PRINTER_RES_INITIAL)
     {
         for(i=0;i<nElems;i++)
        {
            printf("%d <-> %d\n", i,vector[i]);
        }
     }
 }
void printMutant(T_stMutant* pMutant)
{
    if(PRINTER_ON)
    {
        printf("printMutant  Init\n");
        if (pMutant) {
            if (pMutant->nState == ALIVE)            
                printf(">[%d] Alive! - %d\n", pMutant->nNumber, pMutant->oTestList.nElems);
            else
                printf(">[%d] Dead! killed by test %d - %d\n", pMutant->nNumber, pMutant->nTestKiller, pMutant->oTestList.nElems);
        }
        printf("printMutant  End\n");
    }
}
void printMutantsInfo() 
{
    if(PRINTER_ON || PRINTER_RES_FINAL)
    {
        int i, nMutants;
        
        i= nMutants = 0;
        nMutants = m_oTestExecMap.nMutants;
        printf("-------------------------------------------\n");
        printf("Mutants Info %d\n", nMutants);
        if(m_stEnvValues->nClusterMutants == 0)
        {
            for (i = 1; i <= nMutants; i++) {

                T_stMutant* pMutant = m_oMutantList.array[i];

                if (pMutant) {
                    if (pMutant->nState == ALIVE)                
                        printf(">s[%d] Alive!\n", pMutant->nNumber);
                    else
                        printf(">s[%d] Dead! killed by test %d\n", pMutant->nNumber, pMutant->nTestKiller);
                }
                else
                {
                        printf(">s[%d] Null mutant!\n", i);
                        
                }
            }
        }
        else
        {
            for (i = 1; i <= nMutants; i++) {

                T_stMutant* pMutant = m_oMutantList.array[i];
                T_stEquivalentInfo* pEquiv = m_oTestExecMap.oEquivalentInfo[i];
                T_stMutantExecution* pMutantExec = m_oTestExecMap.oMutantMap[i];
                if (pMutant && pEquiv && pMutantExec) {
                    if (pMutant->nState == ALIVE)                
                    {
                        if(pMutantExec->nEquivalent)
                            printf(">s[%d] Equivalent!\n", pMutant->nNumber);
                        else if(pMutantExec->nIsClustered && pEquiv->nIsClusterHead)
                            printf(">s[%d] Alive! (CH)\n", pMutant->nNumber);
                        else if(pMutantExec->nIsClustered)
                            printf(">s[%d] Dupped (%d)!\n", pMutant->nNumber, pEquiv->nMutantHeader);
                        else
                            printf(">s[%d] Alive!\n", pMutant->nNumber);
                    }
                    else
                    {
                        printf(">s[%d] Dead! killed by test %d\n", pMutant->nNumber, pMutant->nTestKiller);
                    }
                }
                else
                {
                    if(pMutantExec->nEquivalent)
                        printf(">s[%d] Equivalent!\n", i);
                    else if(pMutantExec->nIsClustered)
                        printf(">s[%d] Dupped (%d)!\n", i, pEquiv->nMutantHeader);
                    else
                        printf(">s[%d] Null mutant!\n", i);                        
                }
            }            
        }
        printf("-------------------------------------------\n");
    }
}
void printfMutantFull(T_stMutant* pMutant)
{
    if(PRINTER_ON)
    {
        int i;
        if(pMutant)
        {
            if (pMutant->nState == ALIVE)
                printf(">[%d] Alive! | [%d - ", pMutant->nNumber, pMutant->oTestList.nElems);
            else
                printf(">[%d] Dead! killed by test %d | [ %d ", pMutant->nNumber, pMutant->nTestKiller, pMutant->oTestList.nElems);

            for(i=0;i<pMutant->oTestList.nElems;i++)
            {
                T_stTestInfo* pTest = pMutant->oTestList.tests[i];
                
                if(pTest != NULL);
                {
                    printf("<%d,%lf,%d,%s>",pTest->nTest, pTest->dTime,pTest->nKill,pTest->res);
                }

            }
            printf("]\n");
        }
        else
            printf("printfMutantFull - WARNING!! Null mutant.\n");
    }
}
void printTest(T_stTestInfo* pTest)
{
    if(PRINTER_ON)
    {
        if(pTest)
        {
            if(strlen(pTest->res) >0)
                printf("<%d>Test - N:%d | Res:%s | Time: %lf | Killed<%d>\n",m_nRank, pTest->nTest, pTest->res, pTest->dTime, pTest->nKill);
            else
                printf("<%d>Test - N:%d | Res:%s | Time: %lf | Killed<%d>\n",m_nRank, pTest->nTest, "(empty)", pTest->dTime, pTest->nKill);
        }
        else
            printf("WARNING! The test is null\n");
    }   
}
void printTestRed(T_stTI* pTest)
{
    if(PRINTER_ON)
    {
        if(pTest)
            printf("<%d>printTestRed- <%d>Test: %s : %lf <%d>\n",m_nRank, pTest->nMutant,pTest->res, pTest->dTime, pTest->nKill);
    }
}
void printResults(int nTotalMutants, int nDeadMutants, int nEquivalents, int nDupped, int nTotalTests, long int dCompTime, long int dOriginalTime, long int dTime)
{    
    float fMS;
    int nFinalMutants;
    if(PRINTER_ON || PRINTER_RES_FINAL)
    {
        fMS = 0.0;
        

        if(m_stEnvValues && m_stEnvValues->nClusterMutants == 0)
        {
            fMS = (float) nDeadMutants / (float) nTotalMutants;
            printf("---------------------------------\n");
            printf("MALONE says:\n");
            printf("Compilation time: %ld\n", dCompTime);
            printf("Original application time: %ld\n", dOriginalTime);
            printf("Total elapsed time: %ld\n", dTime);
            printf("\n");
            printf("Total mutants: %d\n", nTotalMutants);
            printf("Alive mutants: %d\n", nTotalMutants - nDeadMutants);
            printf("Dead mutants: %d\n", nDeadMutants);
            printf("Mutation Score: %f\n", fMS);

            printf("\n");    
            printMutantsInfo();    
            printf("--------------\n");
        }
        else
        {
            nFinalMutants = nTotalMutants-(nEquivalents+nDupped);
            
            if(nFinalMutants>0)
                fMS = (float) nDeadMutants / (float) nFinalMutants;
            else
                fMS = (float) nDeadMutants / (float) nTotalMutants;
            
            printf("---------------------------------\n");
            printf("MALONE says:\n");
            printf("Compilation time: %ld\n", dCompTime);
            printf("Original application time: %ld\n", dOriginalTime);
            printf("Total elapsed time: %ld\n", dTime);
            printf("\n");
            printf("Total mutants: %d\n", nTotalMutants);
            printf("Alive mutants: %d\n", nTotalMutants - nDeadMutants);
            printf("Dead mutants: %d\n", nDeadMutants);
            printf("Equivalent mutants: %d\n", nEquivalents);
            printf("Dupped mutants: %d\n", nDupped);
            printf("Mutation Score: %f\n", fMS);

            printf("\n");    
            printMutantsInfo();    
            printf("--------------\n");        
        }

    }
}
void printResultsPartial()
{
    if(PRINTER_ON)
    {
        printf("Printing with rank: %d \n", m_nRank);
        printResults(m_oMutantList.nElems, m_oMutantList.nDead, m_oTestExecMap.nEquivalents,m_oTestExecMap.nDupped, 0,0.0,0.0, 0.0);
    }
}
void printDeployArray(int pIndex[], int nSize)
{
    if(PRINTER_ON)
    {
        int i;
        printf("<");
        for(i=0;i<nSize;i++)
        {
            printf("%d, ", pIndex[i]);
        }
        printf(">\n");
    }
}
void printDeployStruct(T_stExecutionStructure* pDeploy)
{
    if(PRINTER_ON)
    {
        if(pDeploy)
        {
            printf("<%d>printDeployStruct (%d) - Deploy structure: !\n", m_nRank, PRINTER_ON);
            printf("<%d>Execution mode: %d\n", m_nRank, pDeploy->nExecutionMode);
            printf("<%d>Init mutant: %d\n", m_nRank, pDeploy->nMutantInit);
            printf("<%d>End mutant: %d\n", m_nRank, pDeploy->nMutantEnd);
            printf("<%d>Init test: %d\n", m_nRank, pDeploy->nTestInit);
            printf("<%d>End test: %d\n", m_nRank, pDeploy->nTestEnd);
            printf("<%d>printDeployStruct - End\n", m_nRank);
        }
    }
}
//This function prints the values contained in an environment object
void printEnvFile(T_stIniValues* pValues)
{
    if(PRINTER_ON)
    {
        printf("<%d>Printing environment file!!\n", m_nRank);
        if(pValues)
        {
            printf("----------\n");
            printf("Environment object:\n");
            printf("Framework: %s\n", pValues->strFramework);
            printf("AppPath: %s\n",pValues->strAppPath);
            printf("MutantPath: %s\n",pValues->strMutantsPath);
            printf("AppName: %s\n", pValues->strAppName);
            printf("Compilation line original: %s\n", pValues->strCompLineOriginal);
            printf("Compilation line mutants: %s\n", pValues->strCompLineMutants);
            printf("Execution line original: %s\n", pValues->strExecLineOriginal);        
            printf("Execution line mutants: %s\n", pValues->strExecLineMutants);        
            printf("Total tests: %d\n", pValues->nTotalTests);
            printf("Total mutants: %d\n", pValues->nTotalMutants);
            printf("Starting mutant: %d\n", pValues->nStartingMutant);
            printf("DistributeOriginal: %d\n", pValues->nDistributeOriginal);
            printf("Incomplete ...\n");
            printf("----------\n");
        }
        else
        {
            printf("<%d>WARNING, Empty environment file!!\n", m_nRank);
        }
    }    
}
int isEnabledPrinterLog()
{   
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugPrinters : 0;
}

void printEquivInfo()
{
    int i;
    for(i=m_stEnvValues->nStartingMutant; i<m_stEnvValues->nTotalMutants;i++)
    {
        T_stMutantExecution* pExecutionBase = m_oTestExecMap.oMutantMap[i];
        T_stEquivalentInfo* pEquivalentBase = m_oTestExecMap.oEquivalentInfo[i];
        
        printf(">Mutant %d - [E:%d, C: %d, ICH: %d, CH:%d, NC: %d ]\n", i, pExecutionBase->nEquivalent, pExecutionBase->nIsClustered, pEquivalentBase->nIsClusterHead, pEquivalentBase->nMutantHeader, pEquivalentBase->nClones);
    }
}
void printMutantEquiv(int nIndex)
{
    
    if(nIndex<m_oTestExecMap.nMutants)
    {
        T_stMutantExecution* pExecutionBase = m_oTestExecMap.oMutantMap[nIndex];
        T_stEquivalentInfo* pEquivalentBase = m_oTestExecMap.oEquivalentInfo[nIndex];

        printf(">Mutant %d - [E:%d, C: %d, ICH: %d, CH:%d, NC: %d ]\n", nIndex, pExecutionBase->nEquivalent, pExecutionBase->nIsClustered, pEquivalentBase->nIsClusterHead, pEquivalentBase->nMutantHeader, pEquivalentBase->nClones);  
    }

}
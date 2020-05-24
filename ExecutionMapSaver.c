#include "ExecutionMapSaver.h"

#define SAVING_PRINT 1
void saveExecutionMap()
{
    int i, j, nMutants, nTests, nLen;
    char* strFile;    
    FILE *file;
    
    if(SAVING_PRINT) printf("saveExecutionMap - Init\n");

    //Create the string for saving the file
    nLen = strlen(m_strResFolder) + strlen(EXECUTION_MAP_FILE) + 1;
    strFile = malloc((nLen + 1) * sizeof (char));
    sprintf(strFile, "%s%s", m_strResFolder, EXECUTION_MAP_FILE);
    if(SAVING_PRINT) printf("saveExecutionMap - File to be saved: %s\n", strFile);
    
    nMutants = m_oTestExecMap.nMutants;
    nTests = m_oTestExecMap.nTests;
    
    if(SAVING_PRINT) printf("saveExecutionMap - Mutants: %d | Tests: %d\n", nMutants, nTests);
    file = fopen(strFile, "w");
    
    if (file != NULL && m_oTestExecMap.pMap != NULL)
    {
        for(i=1;i<nMutants;i++)
        {
            if(m_oTestExecMap.pMap[i] != NULL)
            {
                for(j=0;j<nTests;j++)
                {
                    fprintf(file, "%d ", m_oTestExecMap.pMap[i][j]);
                }          
                fprintf(file, "\n");            
            }
        }
        fflush(file);
        fclose(file);
    }
    else
    {
        printf("Error opening file!\n");
    }
        
    if(strFile != NULL)
        free(strFile);
    if(SAVING_PRINT) printf("saveExecutionMap - End\n");
}
//Save the execution time of the whole process.

//Save the execution time of the 
void saveOriginalProgramExecutionTimes()
{
    char* strFile;
    int i, nMutants, nTests, nLen;
    long lValue;
    FILE *f;
     
    if(SAVING_PRINT) printf("saveOriginalProgramExecutionTimes - Init\n");
    
    strFile = NULL;
    nMutants = m_oTestExecMap.nMutants;
    nTests = m_oTestExecMap.nTests;
    
    //Create the string for saving the file
    nLen = strlen(m_strResFolder) + strlen(EXECUTION_TIMES_MAP_FILE) + 1;
    strFile = malloc((nLen + 1) * sizeof (char));
    sprintf(strFile, "%s%s", m_strResFolder, EXECUTION_TIMES_MAP_FILE);
    if(SAVING_PRINT) printf("saveOriginalProgramExecutionTimes - File to be saved: %s\n", strFile);
    
    if(SAVING_PRINT) printf("saveOriginalProgramExecutionTimes - Saving execution map!!\n");
    if(SAVING_PRINT) printf("saveOriginalProgramExecutionTimes - Tests: %d\n", nTests);
    f = fopen(strFile, "w");
    
    if (f != NULL)
    {
        for(i=0;i<nTests; i++)
        {
            T_stTestInfo* pTest = m_pResList->tests[i];
            if(pTest)
            {
                lValue = (long)pTest->dTime;
                fprintf(f, "%ld ", lValue);
            }
            else
            {
                fprintf(f, "-1 ");
            }            
        }
        fflush(f);
        fclose(f);
    }
    else
    {
        printf("Error opening file!\n");
    }
    if(strFile != NULL)
        free(strFile);

    
    if(SAVING_PRINT) printf("saveOriginalProgramExecutionTimes - End\n");    
}
void saveOriginalProgramResults()
{
    char* strFile;
    int i, j, nMutants, nTests, nLen;
    long lValue;
    FILE *f;
     
    if(SAVING_PRINT) printf("saveOriginalProgramResults - Init\n");
    
    strFile = NULL;
    nMutants = m_oTestExecMap.nMutants;
    nTests = m_oTestExecMap.nTests;
    
    //Create the string for saving the file
    nLen = strlen(m_strResFolder) + strlen(EXECUTION_RESULTS_MAP_FILE) + 1;
    strFile = malloc((nLen + 1) * sizeof (char));
    sprintf(strFile, "%s%s", m_strResFolder, EXECUTION_RESULTS_MAP_FILE);
    if(SAVING_PRINT) printf("saveOriginalProgramResults - File to be saved: %s\n", strFile);
    
    if(SAVING_PRINT) printf("saveOriginalProgramResults - Saving execution map!!\n");
    if(SAVING_PRINT) printf("saveOriginalProgramResults - Tests: %d\n", nTests);
    f = fopen(strFile, "w");
    
    if (f != NULL)
    {
        for(i=0;i<nTests; i++)
        {
            T_stTestInfo* pTest = m_pResList->tests[i];
            if(pTest)
            {
                fprintf(f, "%d - %s ", i, pTest->res);
            }
            else
            {
                fprintf(f, "-1 ");
            }            
        }
        fflush(f);
        fclose(f);
    }
    else
    {
        printf("saveOriginalProgramResults - Error opening file!\n");
    }
    if(strFile != NULL)
        free(strFile);

    
    if(SAVING_PRINT) printf("saveOriginalProgramExecutionTimes - End\n");    
}
void saveWholeExecutionTimesMap()
{
    T_stTestInfo * pTest;
    T_stMutant * pMutant;
    FILE *file;
    char* strFile;
    int i, j, nMutants, nTests, nLen;
    long lValue, lTickInit, lTickEnd;
    
    nMutants = m_oTestExecMap.nMutants;
    nTests = m_oTestExecMap.nTests;
    
    //Create the string for saving the file
    nLen = strlen(m_strResFolder) + strlen(EXECUTION_WHOLE_TIMES_MAP_FILE) + 1;
    strFile = malloc((nLen + 1) * sizeof (char));
    sprintf(strFile, "%s%s", m_strResFolder, EXECUTION_WHOLE_TIMES_MAP_FILE);
    if(SAVING_PRINT) printf("saveWholeExecutionTimesMap - File to be saved: %s\n", strFile);
    
    if(SAVING_PRINT) printf("saveWholeExecutionTimesMap - Saving execution map!!\n");
    if(SAVING_PRINT) printf("saveWholeExecutionTimesMap - Mutants: %d | Tests: %d\n", nMutants, nTests);
    file = fopen(strFile, "w");
    if (file != NULL)
    {

        //Mutants
        for(i=1;i<=nMutants;i++)
        {
            pMutant = m_oMutantList.array[i];
            if(pMutant!= NULL)
            {                
                for(j=0;j<nTests;j++)
                {
                    if(j<MAX_TESTS)
                    {
                        if(j<pMutant->oTestList.nElems)
                        {    pTest = pMutant->oTestList.tests[j];
                            if(pTest != NULL)
                            {
                                lValue = (long)pTest->dTime;
                                fprintf(file, "%ld ", lValue);
                            }   
                            /*else Deleted to reduce the total size of the results
                            {
                                fprintf(file, "-1 ");
                            }*/
                        }
                        /*else
                        {
                            fprintf(file, "-1 ");
                        }*/
                    }
                    else
                        printf("saveWholeExecutionTimesMap - Test (id:%d) overflow \n", j);
                    
                }              
            }        
            fprintf(file, "\n");
        }
        fflush(file);
        fclose(file);
    }
    else
    {
        printf("Error opening file!\n");
    }
    if(strFile != NULL)
        free(strFile);
    
    if(SAVING_PRINT) printf("saveWholeExecutionTimesMap - End\n");  
}

void saveMutationTestingTickMap()
{
    T_stTestInfo * pTest;
    T_stMutant * pMutant;
    FILE *file;
    char* strFile;
    int i, j, nMutants, nTests, nLen;
    long lValue, lTickInit, lTickEnd;
    
    nMutants = m_oTestExecMap.nMutants;
    nTests = m_oTestExecMap.nTests;
    
    //Create the string for saving the file
    nLen = strlen(m_strResFolder) + strlen(MUTATION_TESTING_TIMES_MAP_FILE) + 1;
    strFile = malloc((nLen + 1) * sizeof (char));
    sprintf(strFile, "%s%s", m_strResFolder, MUTATION_TESTING_TIMES_MAP_FILE);
    if(SAVING_PRINT) printf("saveMutationTestingTickMap - File to be saved: %s\n", strFile);
    
    if(SAVING_PRINT) printf("saveMutationTestingTickMap - Saving execution map!!\n");
    if(SAVING_PRINT) printf("saveMutationTestingTickMap - Mutants: %d | Tests: %d\n", nMutants, nTests);
    
    printf("<%d> saveMutationTestingTickMap() - Duration of mutation phases -  [Init: %ld | Gen:%ld | Comp: %ld | Orig: %ld | Mut: %ld | Total: %ld]\n", 
            m_nRank, m_lInitializationTime, m_lGenerationTime, m_lCompTime, m_lOriginalTime, m_lMutantsTime, m_lTotalTime);
    
    file = fopen(strFile, "w");
    if (file != NULL)
    {
        fprintf(file, "Workers: %d\n", m_nSize-1);
        fprintf(file, "Mutants: %d\n", nMutants);
        fprintf(file, "Tests: %d\n", nTests);
        fprintf(file, "InitTick: %ld\n", m_lInitTick);
        fprintf(file, "EndTick: %ld\n", m_lEndTick);
        fprintf(file, "InitializationInitTick: %ld\n", m_lInitializeInitTick);
        fprintf(file, "InitializationEndTick: %ld\n", m_lInitializeEndTick);        
        fprintf(file, "GenInitTick: %ld\n", m_lGenEndTick);
        fprintf(file, "GenEndTick: %ld\n", m_lGenInitTick);       
        fprintf(file, "CompilationInitTick: %ld\n", m_lCompEndTick);
        fprintf(file, "CompilationEndTick: %ld\n", m_lCompInitTick);       
        fprintf(file, "OriginalInitTick: %ld\n", m_lOriginalInitTick);
        fprintf(file, "OriginalEndTick: %ld\n", m_lOriginalEndTick);     
        fprintf(file, "MutantsInitTick: %ld\n", m_lMutantInitTick);
        fprintf(file, "MutantsEndTick: %ld\n", m_lMutantEndTick); 
        
        //Original
        fprintf(file, "Original #tests: %d\n", nTests);
        for(j=0;j<nTests; j++)
        {
            T_stTestInfo* pTest = m_pResList->tests[j];
            if(pTest)
            {
                lValue = (long)pTest->dTime;
                lTickInit = (long)pTest->dInitTick;
                lTickEnd = (long)pTest->dEndTick;
                fprintf(file, "[#%d|%ld|%ld,%ld]\n", j, lValue, lTickInit, lTickEnd);
            }
            else
            {
                fprintf(file, "[#%d|-1|-1,-1|]\n", j);
            }            
        }
        
        //Completar: añadir los ticks del original y dar un formato algo más legible
        fprintf(file, "#Mutants: %d\n", nMutants);
        for(i=1;i<=nMutants;i++)
        {
            fprintf(file, "M%d { ", i);
            pMutant = m_oMutantList.array[i];
            if(pMutant!= NULL)
            {                
                for(j=0;j<nTests;j++)
                {
                    if(j<MAX_TESTS)
                    {
                        if(j<pMutant->oTestList.nElems)
                        {    pTest = pMutant->oTestList.tests[j];
                            if(pTest != NULL)
                            {
                                lValue = (long)pTest->dTime;
                                lTickInit = (long)pTest->dInitTick;
                                lTickEnd = (long)pTest->dEndTick;
                                fprintf(file, "[#%d|%ld|%ld,%ld] ", j, lValue, lTickInit, lTickEnd);
                            }   
                            else
                            {
                                fprintf(file, "[#%d|-1|-1,-1] ", j);
                            }
                        }
                        else
                        {
                            fprintf(file, "[#%d|-1|-1,-1] ",j);
                        }
                    }
                    else
                        printf("saveWholeExecutionTimesMap - Test (id:%d) overflow \n", j);
                    
                }              
            }        
            fprintf(file, "}\n");
        }
        fflush(file);
        fclose(file);
    }
    else
    {
        printf("Error opening file!\n");
    }
    if(strFile != NULL)
        free(strFile);
    
    if(SAVING_PRINT) printf("saveWholeExecutionTimesMap - End\n");  
}
void saveOverview()
{
    char* strFile;
    int i, j, nMutants, nTests, nLen, nAliveMutants, nDeadMutants, nFinalMutants;
    long lValue;
    float fMS;
    FILE *f;
     
    if(SAVING_PRINT) printf("saveOverview - Init\n");
    
    strFile = NULL;
    nMutants = m_oTestExecMap.nMutants;
    nTests = m_oTestExecMap.nTests;
    nDeadMutants = m_oMutantList.nDead;
    nAliveMutants = nMutants - nDeadMutants;
    fMS = (float) nDeadMutants / (float) nMutants;
    
    //Create the string for saving the file
    nLen = strlen(m_strResFolder) + strlen(MUTATION_TESTING_OVERVIEW) + 1;
    strFile = malloc((nLen + 1) * sizeof (char));
    sprintf(strFile, "%s%s", m_strResFolder, MUTATION_TESTING_OVERVIEW);
    if(SAVING_PRINT) printf("saveOverview - File to be saved: %s\n", strFile);
    
    if(SAVING_PRINT) printf("saveOverview - Saving execution map!!\n");
    if(SAVING_PRINT) printf("saveOverview - Tests: %d\n", nTests);
    f = fopen(strFile, "w");
    
    if (f != NULL)
    {
       // fprintf(f,"<%d> Duration of mutation phases -  [Init: %ld | Gen:%ld | Comp: %ld | Orig: %ld | Mut: %ld | Total: %ld]\n",
       //  m_nRank, m_lInitializationTime, m_lGenerationTime, m_lCompTime, m_lOriginalTime, m_lMutantsTime, m_lTotalTime);
        fprintf(f,"<%d> Duration of mutation phases -  [Init: %ld | Gen:%ld | Comp: %ld | Equiv: %ld | Sort: %ld | Orig: %ld | Mut: %ld | Total: %ld]\n",
            m_nRank, m_lInitializationTime, m_lGenerationTime, m_lCompTime, m_lEquivTime, m_lSortTime, m_lOriginalTime, m_lMutantsTime, m_lTotalTime);        
        //Print the results!            
        
        fprintf(f,"<%d> Total worker processes: %d\n", m_nRank, m_nSize);
        fprintf(f,"<%d> Check if the res folder [%s] exists\n", m_nRank, PATH_RES_DIR);
        
        nFinalMutants = nMutants-(m_oTestExecMap.nEquivalents+m_oTestExecMap.nDupped);
        if(nFinalMutants>0)
            fMS = (float) nDeadMutants / (float) nFinalMutants;
        else if(nMutants != 0)
            fMS = (float) nDeadMutants / (float) nMutants;
        else
            fMS = 0.0;
        
        fprintf(f, "----------------\n");
        fprintf(f, "MALONE says: \n");
        fprintf(f, "Initialization time: %ld\n", m_lInitializationTime);
        fprintf(f, "Generation time: %ld\n", m_lGenerationTime);
        fprintf(f, "Compilation time: %ld\n", m_lCompTime);
        fprintf(f, "Original time: %ld\n", m_lOriginalTime);
        fprintf(f, "Mutants time: %ld\n", m_lMutantsTime);
        fprintf(f, "Total time: %ld\n", m_lTotalTime);
        fprintf(f, "----------------\n");
        fprintf(f, "Total mutants: %d\n", nMutants);
        fprintf(f, "Total tests: %d\n", nTests);
        fprintf(f, "Alive mutants: %d\n", nAliveMutants);
        fprintf(f, "Dead mutants: %d\n", nDeadMutants);
        fprintf(f, "Equivalent mutants: %d\n", m_oTestExecMap.nEquivalents);
        fprintf(f, "Dupped mutants: %d\n", m_oTestExecMap.nDupped);
        fprintf(f, "Mutation score: %f\n", fMS);
        fprintf(f, "----------------\n");
        
        if(m_stEnvValues->nClusterMutants == 0)
        {
            for (i = 1; i <= nMutants; i++) {

                T_stMutant* pMutant = m_oMutantList.array[i];

                if (pMutant) {
                    if (pMutant->nState == ALIVE)                
                        fprintf(f,">s[%d] Alive!\n", pMutant->nNumber);
                    else
                        fprintf(f,">s[%d] Dead! killed by test %d\n", pMutant->nNumber, pMutant->nTestKiller);
                }
                else
                {
                    fprintf(f,">s[%d] Null mutant!\n", i);                        
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
                            fprintf(f,">s[%d] Equivalent!\n", pMutant->nNumber);
                        else if(pMutantExec->nIsClustered && pEquiv->nIsClusterHead)
                            fprintf(f,">s[%d] Alive! (CH)\n", pMutant->nNumber);
                        else if(pMutantExec->nIsClustered)
                            fprintf(f,">s[%d] Dupped (%d)!\n", pMutant->nNumber, pEquiv->nMutantHeader);
                        else
                            fprintf(f,">s[%d] Alive!\n", pMutant->nNumber);
                    }
                    else
                    {
                        fprintf(f,">s[%d] Dead! killed by test %d\n", pMutant->nNumber, pMutant->nTestKiller);
                    }
                }
                else
                {
                    if(pMutantExec->nEquivalent)
                        fprintf(f,">s[%d] Equivalent!\n", i);
                    else if(pMutantExec->nIsClustered)
                        fprintf(f,">s[%d] Dupped (%d)!\n", i, pEquiv->nMutantHeader);
                    else
                        fprintf(f,">s[%d] Null mutant!\n", i);                        
                }
            }            
        }
        fprintf(f, "----------------\n");
        fflush(f);
        fclose(f);
    }
    else
    {
        printf("saveOverview - Error opening file!\n");
    }
    if(strFile != NULL)
        free(strFile);

    
    if(SAVING_PRINT) printf("saveOverview - End\n"); 
    
}
void saveEquivalenceList()
{
    char* strFile, *strMd5Hash;
    int i, j, nMutants, nTests, nLen, nAliveMutants, nDeadMutants;
    long lValue;
    float fMS;
    FILE *f;
     
    if(SAVING_PRINT) printf("saveEquivalenceList - Init\n");
    
    strFile = NULL;
    nMutants = m_oTestExecMap.nMutants;
    nTests = m_oTestExecMap.nTests;
    nDeadMutants = m_oMutantList.nDead;
    nAliveMutants = nMutants - nDeadMutants;
    if(nMutants>0 && nDeadMutants>0)
        fMS = (float) nDeadMutants / (float) nMutants;
    else
        fMS = 0.0;
    
    //Create the string for saving the file
    nLen = strlen(m_strResFolder) + strlen(EQUIVALENCE_FILE) + 1;
    strFile = malloc((nLen + 1) * sizeof (char));
    sprintf(strFile, "%s%s", m_strResFolder, EQUIVALENCE_FILE);
    if(SAVING_PRINT) printf("saveEquivalenceList - File to be saved: %s\n", strFile);
   
    f = fopen(strFile, "w");
    fprintf(f, "----------------\n");
    fprintf(f, "Total mutants: %d\n", nMutants);
    fprintf(f, "Equivalent mutants: %d\n", m_oTestExecMap.nEquivalents);
    fprintf(f, "Dupped mutants: %d\n", m_oTestExecMap.nDupped);
    fprintf(f, "----------------\n");

    if(SAVING_PRINT) printf("saveOverview - 0\n");
    if(m_stEnvValues->nClusterMutants)  
    {
        for (int i = 0; i <= m_oTestExecMap.nMutants; i++) {

            strMd5Hash = m_oTestExecMap.oMd5Map[i];

            if(strMd5Hash != NULL)
            {
                fprintf(f,"[%d] %s\n", i, strMd5Hash);           
            }
        }    
    }

    fprintf(f, "----------------\n");
    fflush(f);
    fclose(f);
        
    if(strFile != NULL)
        free(strFile);

    
    if(SAVING_PRINT) printf("saveOverview - End\n"); 
}
void saveMonitorResults()
{
    FILE *f;
    char* strFile, *strLine;
    int nLen, nAlarmsHandled, nTotalElements;

    
    if(m_stEnvValues != NULL && m_stEnvValues->nMonitorEnabled==1)
    {
                        
        //Create the string for saving the file
        nLen = strlen(m_strResFolder) + strlen(MONITOR_FILE) + 1;
        strFile = malloc((nLen + 1) * sizeof (char));
        sprintf(strFile, "%s%s", m_strResFolder, MONITOR_FILE);
        if(SAVING_PRINT) printf("saveMonitorResults - File to be saved: %s\n", strFile);
        
        nAlarmsHandled = m_oMonitorLines.nAlarms;
        nTotalElements = m_oMonitorLines.nElems;
        
        f = fopen(strFile, "w");
        fprintf(f, "----------------\n");
        fprintf(f, "Total elements: %d\n", nTotalElements);
        fprintf(f, "Total alarms: %d\n", nTotalElements);
        fprintf(f, "----------------\n");
        
        for (int i = 0; i<m_oMonitorLines.nElems; i++)
        {            
            strLine = m_oMonitorLines.array[i];
            fprintf(f, "[[%d\n%s\n]]\n",i,strLine);                      
        }
        
        fprintf(f, "----------------\n");
        fflush(f);
        fclose(f);

        if(strFile != NULL)
            free(strFile);
    }
}
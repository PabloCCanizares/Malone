
#include "EquivalenceChecking.h"

void createClusters()
{
    int i,j, nEquivalent;
    char* strMd5;
    char* strMutantMd5Eq;
    if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Creating clusters!\n");
    //Create the clusters
    
    for(i=m_stEnvValues->nStartingMutant; i<m_stEnvValues->nTotalMutants;i++)
    {
        T_stMutantExecution* pExecutionBase = m_oTestExecMap.oMutantMap[i];
        T_stEquivalentInfo* pEquivalentBase = m_oTestExecMap.oEquivalentInfo[i];

       
        if(pExecutionBase && pExecutionBase->nEquivalent == 0)
        {
            if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Checking base before Mutant %d, cluster head: %d | is clustered: %d | is cluster head: %d!!\n", i, pEquivalentBase->nMutantHeader, pExecutionBase->nIsClustered, pEquivalentBase->nIsClusterHead);
            //check if the mutant              
            if(pEquivalentBase && pExecutionBase->nIsClustered==0)
            {
                
                //Check the rest of mutants
                for(j=i+1; j<m_stEnvValues->nTotalMutants;j++)
                {
                    T_stMutantExecution* pExecutionComp = m_oTestExecMap.oMutantMap[j];
                    T_stEquivalentInfo* pEquivalentComp = m_oTestExecMap.oEquivalentInfo[j];
                 
                    if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Checking before Mutant %d, cluster head: %d | is mutant header: %d!!\n", j, pEquivalentComp->nMutantHeader, pExecutionComp->nIsClustered);
                    //printf("%s vs %s", strMd5, strMutantMd5Eq);                        
                    if(pExecutionComp != NULL && pEquivalentComp != NULL)
                    {
                        if(pExecutionComp->nEquivalent == 0 && pExecutionComp->nIsClustered==0)
                        {
                            nEquivalent=0;
                            if(i!=j && m_oTestExecMap.oMd5Map[i] != NULL && m_oTestExecMap.oMd5Map[j] != NULL)
                                nEquivalent = (strcmp(m_oTestExecMap.oMd5Map[i], m_oTestExecMap.oMd5Map[j]) == 0);
                            
                            if(nEquivalent)
                            {
                                if(pEquivalentComp->nIsClusterHead)
                                    printf("--------------> SOMETHING WRONG!!\n");
                                
                                //Base
                                pExecutionBase->nIsClustered=1;
                                pEquivalentBase->nIsClusterHead=1;
                                pEquivalentBase->nIsEnabled=1;
                                //pEquivalentBase->nMutantHeader=i;

                                //Comp
                                pExecutionComp->nIsClustered=1;
                                pEquivalentComp->nMutantHeader=i;                                
                                pEquivalentComp->nIsEnabled=1;
                                pEquivalentComp->nIsClusterHead=0;
                                
                                if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Mutant %d and %d are clustered!!\n", i,j);
                                if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Mutant %d, cluster head: %d | is mutant header: %d!!\n", j, pEquivalentComp->nMutantHeader, pEquivalentComp->nIsClusterHead);
                                //IF the selected method is aggresive, kill it right now!
                                /*if(m_stEnvValues->nClusterMutants == AGGRESIVE_EQUIV_METHOD)
                                {
                                    pExecutionComp->nFinished=1;
                                }*/
                                
                                //Set the clone mutant to the head
                                if(pEquivalentBase->nClones >= 0)
                                {
                                    if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Inserting into mutant:%d clone %d . Total clones: %d\n", i,j,pEquivalentBase->nClones);
                                    if(pEquivalentBase->pClones == NULL)
                                    {
                                        if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - The clone information is null, allocating it the first time\n");
                                        pEquivalentBase->pClones=0;
                                        pEquivalentBase->nMaxClones=INITIAL_MAX_CLONES;
                                        pEquivalentBase->pClones= malloc(INITIAL_MAX_CLONES*sizeof(int));                                                                                                                       
                                    }
                                    else
                                    {
                                        if(pEquivalentBase->nClones>=pEquivalentBase->nMaxClones)
                                        {
                                            if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Reallocating mutant, newsize: %d\n",pEquivalentBase->nMaxClones*2);
                                            pEquivalentBase->nMaxClones*=2;
                                            pEquivalentBase->pClones = realloc(pEquivalentBase->pClones, pEquivalentBase->nMaxClones* sizeof(int));
                                            if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Reallocating mutant, newsize: %d\n",pEquivalentBase->nMaxClones*2);
                                        }
                                            
                                    }
                                    pEquivalentBase->pClones[pEquivalentBase->nClones] = j; 
                                    pEquivalentBase->nClones++;
                                    if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Inserted into mutant:%d clone %d . Total clones: %d\n", i,j,pEquivalentBase->nClones);
                                
                                }
                                m_oTestExecMap.nDupped++;
                                if(pEquivalentComp->nIsClusterHead)
                                    printf("--------------> SOMETHING WRONG!!\n");
                            }                            
                        }                        
                    }
                    else
                    {
                        if(pExecutionComp == NULL)
                        {
                            printf("checkMutantsEquivalence - 3.%d - WARNING!! Execution comp is null!\n", i);
                        }
                        if(pEquivalentComp == NULL)
                            printf("checkMutantsEquivalence - 3.%d - WARNING!! Equivalent comp is clustered!!!\n", i);                            
                    }
                }
            }
            else
            {
                if(pEquivalentBase == NULL)
                {
                    printf("checkMutantsEquivalence - 2.%d - WARNING!! Execution base is null!\n", i);
                }
                else if(pExecutionBase->nIsClustered==0)
                    printf("checkMutantsEquivalence - 2.%d - WARNING!! Execution base is clustered!!!\n", i);

            }
        }
        else
        {                
            if(pExecutionBase == NULL)
                printf("checkMutantsEquivalence - 1.%d - WARNING!! Execution base is null!\n", i);
            else
                if(pExecutionBase->nEquivalent == 1)
                     printf("checkMutantsEquivalence - 1.%d - WARNING!! Execution base is equivalent!\n", i);                 
        }
    }               
    
    printf("checkMutantsEquivalence - Total equivalents: %d, total dupped: %d\n", m_oTestExecMap.nEquivalents, m_oTestExecMap.nDupped);
}
/**
 * Compare the binary files, using MD5 hash, of two mutants. ACtually this method is never used during the 
 * equivalent detection phase due to the high computational cost
 * @param nMutantSource
 * @param nMutantDest
 * @return 
 */
int areEquals(int nMutantSource, int nMutantDest)
{
    int nRet;
    char* strMd5, *strMutantSource, *strMutantDest;

    //Oye, retocar esto no? -> HAcer que funciona, porque no va ...
    if(EQUIVALENCE_PRINT_DEBUG) 
        printf("areEquals ? %d vs %d\n", nMutantSource,nMutantDest);
        
    strMutantSource = strMutantDest = NULL;
    
    strMutantSource = buildEquivLine(nMutantSource,BUILD_LINE_ORIGINAL_MODE);
    strMutantDest = buildEquivLine(nMutantDest,BUILD_LINE_ORIGINAL_MODE);
    
    strMd5 = (char*) execCommandLine("diff %s %s", strMutantSource, strMutantDest);

    nRet=0;
    if(strMd5 == NULL)
    {
        if(EQUIVALENCE_PRINT) 
        {
            if(EQUIVALENCE_PRINT_DEBUG) printf("The mutants %d and %d are equals!!!!\n", nMutantSource, nMutantDest);
            if(EQUIVALENCE_PRINT_DEBUG) printf("Execution line: diff %s %s\n",strMutantSource, strMutantDest);
            nRet=1;
        }
    }
    if(strMutantSource != NULL)
        free(strMutantSource);
    if(strMutantDest)
        free(strMutantDest);
    return nRet;       
}
void sequentialEquivalenceChecking()
{
    int i, j, nLen, nEquivalent;
    char* strMd5, *strMutantFile, *strOriginalFile;
    char exeLine[MAX_EXELINE];
    
    bzero(&exeLine,MAX_EXELINE);  
    //It is neccesary to check if the equivalent mutants and the duplicated ones.
    
    strMutantFile = strOriginalFile = strMd5 = NULL;
    
    if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - Init\n");
    
    //Md5Hash Original
    strMutantFile = buildEquivLine(i,BUILD_LINE_ORIGINAL_MODE);        
    sprintf (exeLine, "openssl md5 -r %s | cut -f 1 -d ' '", strMutantFile);        
    strMd5 = (char*) execCommandLine("%s", exeLine);

    if(strMd5 != NULL)
    {
        //Copy the result to the list
        nLen = (strlen(strMd5)*sizeof(char));
                 
        if(nLen>0)
        {
            m_oTestExecMap.oMd5Map[0] = malloc(nLen+1); 
            bzero(m_oTestExecMap.oMd5Map[0], nLen); 
            strcpy(m_oTestExecMap.oMd5Map[0], strMd5);   
            free(strMd5);
        }        
    }    
    //Md5Hash of all the mutants    
    for(i=1; i<=m_stEnvValues->nTotalMutants;i++)
    {
        strMutantFile = buildEquivLine(i,BUILD_LINE_MUTANT_MODE);        
        
        sprintf (exeLine, "openssl md5 -r %s | cut -f 1 -d ' '", strMutantFile);        
        strMd5 = (char*) execCommandLine("%s", exeLine);
        
        if(strMd5 != NULL)
        {
            //Copy the result to the list
            nLen = (strlen(strMd5)*sizeof(char));
            m_oTestExecMap.oMd5Map[i] = malloc(nLen+1); 
            bzero(m_oTestExecMap.oMd5Map[i], nLen);                        
            strcpy(m_oTestExecMap.oMd5Map[i], strMd5);   
            //free(strMd5);
        }
        
        if(m_oTestExecMap.oMd5Map[i] != NULL)
        {
            if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - %d - Md5: %s", i,m_oTestExecMap.oMd5Map[i]);
        }
        else
        {
            if(EQUIVALENCE_PRINT)  printf("checkMutantsEquivalence - %d - null!\n", i);
        }
    }
    
    if(EQUIVALENCE_PRINT) printf("Updating the equivalence structures v2\n");
    
    //openssl md5 -r milu_output/mutants/2/bin/mut.exe |cut -f 1 -d " "
    for(i=m_stEnvValues->nStartingMutant; i<m_stEnvValues->nTotalMutants;i++)
    {        
        nEquivalent=0;
        //Equivalent list: marked as finished and equivalents
        //nEquivalent = areEquals(0,i);
        if(m_oTestExecMap.oMd5Map[0] != NULL && m_oTestExecMap.oMd5Map[i] != NULL)
            nEquivalent = (strcmp(m_oTestExecMap.oMd5Map[0], m_oTestExecMap.oMd5Map[i]) == 0);
        
        if(nEquivalent)
        {
            if(EQUIVALENCE_PRINT) printf("The mutant %d is equivalent!!!!\n", i);
            //The selected mutant is equivalent
            T_stMutantExecution* pExecution = m_oTestExecMap.oMutantMap[i];
            if(pExecution != NULL)
            {
                pExecution->nEquivalent=1;
                pExecution->nFinished=1;
                m_oTestExecMap.nEquivalents++;
            }
        }
        else
        {
            if(m_oTestExecMap.oMd5Map[i] == NULL)
            {
                if(EQUIVALENCE_PRINT) printf("The mutant %d is null!\n", i);
            }
        }
        
    }  
    createClusters();   
    if(EQUIVALENCE_PRINT) printEquivInfo();   
    if(EQUIVALENCE_PRINT) printf("checkMutantsEquivalence - End\n");
}
void checkMutantsEquivalence()
{
    m_lEquivInitTick = getTick();
    if(m_stEnvValues && m_stEnvValues->nParallelMd5sum == 0)
    {
        if(m_nRank == MALONE_MASTER)
            sequentialEquivalenceChecking();
        else
            printf("checkMutantsEquivalence - WARNING! Misconfiguration on the system, this call only must be performed  by masters!!\n");
    }
    else
    {
        parallelEquivalenceChecking();
    }
    m_lEquivEndTick = getTick();
}
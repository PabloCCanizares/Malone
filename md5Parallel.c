/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md5Parallel.c
 * Author: Pablo C. Cañizares
 *
 * Created on October 4, 2018, 12:54 PM
 */


#define EQUIVALENCE_PRINT 1

#include "printers.h"
#include "Auxiliars.h"
#include "MPI_Operations.h"
#include "EquivalenceChecking.h"

int testEquivalenceChecking()
{
    //Divide the number of mutants among the available workers!
    int nMutants, nTests, nTotalRemaining, nRemWork, nIndex, i, nRet, nInit, nEnd, nNumReceives, nLen, nTotalMutants, nTotalSent;
    char *strMutantFile, *strMd5;
    char exeLine[MAX_EXELINE];

    
    if(EQUIVALENCE_PRINT) printf("<%d>-------------------------------------------------------------------->INIT\n", m_nRank);

    if (m_nRank == MALONE_MASTER) {
        if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - Master!! mode!\n", m_nRank);
        
        //Initialize       
        nMutants = m_stEnvValues->nTotalMutants;
        nTests = m_stEnvValues->nTotalTests;
        nIndex = nInit= 1;
        nRemWork = (m_nSize - 1);
        nTotalRemaining = nMutants;
        nEnd = nNumReceives = nTotalSent = 0;

        //Md5Hash Original
        if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - 0\n", m_nRank);
        strMutantFile = (char*) buildEquivLine(0,BUILD_LINE_ORIGINAL_MODE);  
        if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - 1\n", m_nRank);
        if(strMutantFile != NULL)
        {            
            if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - Original program binary: %s\n", m_nRank, strMutantFile);
            //sprintf (exeLine, "openssl md5 -r %s | cut -f 1 -d \" \"", strMutantFile);      
            sprintf (exeLine, "openssl md5 -r %s | cut -f 1 -d ' '", strMutantFile);      
            
            if(exeLine!=NULL)
            {                   
                if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - Line to execute: %s\n", m_nRank, exeLine);
                strMd5 = (char*) execCommandLine("%s", exeLine);
                if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - Obtained MD5: %s\n", m_nRank, exeLine);
            }
            else
                printf("<%d>parallelEquivalenceChecking - WARNING!!! Empty line\n", m_nRank);
            
        }
        else
            printf("<%d>parallelEquivalenceChecking - WARNING!!! Empty line\n", m_nRank);
        

        if(strMd5 != NULL)
        {
            //Copy the result to the list
            nLen = (strlen(strMd5)*sizeof(char));

            printf("<%d>parallelEquivalenceChecking - Original md5: %s [size: %d]!!\n", m_nRank, strMd5, nLen);
            if(nLen>0)
            {
                m_oTestExecMap.oMd5Map[0] = malloc(nLen+1); 
                bzero(m_oTestExecMap.oMd5Map[0], nLen); 
                strcpy(m_oTestExecMap.oMd5Map[0], strMd5);   
            }
        }  
        else
            printf("<%d>parallelEquivalenceChecking - WARNING!!! Empty md5!!\n", m_nRank);
            
    }
    if(EQUIVALENCE_PRINT) printf("<%d><--------------------------------------------------------------------END\n", m_nRank);
}
int parallelEquivalenceChecking() {
    //Divide the number of mutants among the available workers!
    int nMutants, nTests, nTotalRemaining, nRemWork, nIndex, i, nRet, nInit, nEnd, nNumReceives, nLen, nTotalMutants, nTotalSent;
    int oIndex[MAX_WORKERS];
    char *strMutantFile, *strMd5;
    char** strMd5List;
    char exeLine[MAX_EXELINE];
    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;
    
    if(EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - Init\n", m_nRank);

    if (m_nRank == MALONE_MASTER) {
        if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - Master!! mode!\n", m_nRank);
        
        //Initialize       
        nMutants = m_stEnvValues->nTotalMutants;
        nTests = m_stEnvValues->nTotalTests;
        nIndex = nInit= 1;
        nRemWork = (m_nSize - 1);
        nTotalRemaining = nMutants;
        nEnd = nNumReceives = nTotalSent = 0;
        strMd5 = NULL;
        
        
        //Md5Hash Original
        if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - 0\n", m_nRank);
        strMutantFile = (char*) buildEquivLine(0,BUILD_LINE_ORIGINAL_MODE);  
        if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - 1\n", m_nRank);
        if(strMutantFile != NULL)
        {            
            if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - Original program binary: %s\n", m_nRank, strMutantFile);
            sprintf (exeLine, "openssl md5 -r %s | cut -f 1 -d ' '", strMutantFile);      
            //sprintf (exeLine, "openssl md5 -binary %s", strMutantFile);      
            
            if(exeLine!=NULL)
            {                   
                if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - Line to execute: %s\n", m_nRank, exeLine);
                strMd5 = (char*) execCommandLine("%s", exeLine);
                if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - Obtained MD5: %s\n", m_nRank, exeLine);
            }
            else
                printf("<%d>parallelEquivalenceChecking - WARNING!!! Empty line\n", m_nRank);
            
        }
        else
            printf("<%d>parallelEquivalenceChecking - WARNING!!! Empty line\n", m_nRank);
        

        if(strMd5 != NULL)
        {
            //Copy the result to the list
            nLen = (strlen(strMd5)*sizeof(char));

            printf("<%d>parallelEquivalenceChecking - Original md5: %s [size: %d]!!\n", m_nRank, strMd5, nLen);
            if(nLen>0)
            {
                m_oTestExecMap.oMd5Map[0] = malloc(nLen+1); 
                bzero(m_oTestExecMap.oMd5Map[0], nLen); 
                strcpy(m_oTestExecMap.oMd5Map[0], strMd5);   
            }
        }  
        else
            printf("<%d>parallelEquivalenceChecking - WARNING!!! Empty md5!!\n", m_nRank);
    
        if(EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - Distributing %d mutants among %d workers\n", m_nRank, nMutants, m_nSize - 1);

        //Distribute the elements 
        while (nTotalRemaining > 0 && nRemWork) {
            oIndex[nIndex] = ceil(nTotalRemaining / nRemWork);
            nTotalRemaining -= ceil(nTotalRemaining / nRemWork);
            nIndex++;
            nRemWork--;
        }
        printDeployArray(&oIndex, m_nSize);
        //exeMode.nExecutionMode = (int) eMethod;

        
        //Fill the env values to send it to workers
        for (i = 1; i < m_nSize; i++) {
            nEnd += oIndex[i];
            exeVector[i].nMutantInit = nInit;
            exeVector[i].nMutantEnd = nEnd;
            exeVector[i].nTestInit = 0;
            exeVector[i].nTestEnd = m_stEnvValues->nTotalTests - 1;

            sendDeployMode((T_stExecutionStructure*) &exeVector[i], i);
            nInit = nEnd;
        }
        //Wait for receiving values from all workers and filling final result
        do {
            receiveMd5Mutants(&exeVector);
            nNumReceives++;
        } while (nNumReceives < (m_nSize - 1)); // Decreasing in 1, due to the master
    }
    else 
    {
        if (EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - Workers mode!\n", m_nRank);
        pExeRetMode = NULL;
        //receive the deployment method
        
        if(COMPATIBLE_MODE)
            receiveDeployModeRefP(&pExeRetMode);
        else            
            pExeRetMode = receiveDeployMode();

        assert(pExeRetMode != NULL);
        printf(" DP %d, %d\n", pExeRetMode->nExecutionMode, pExeRetMode->nMutantInit);
        
        if (EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - Received deploy mode!\n", m_nRank);
        if(pExeRetMode)
        {
            printDeployStruct(pExeRetMode);
        
            if(pExeRetMode->nMutantInit == 0)
                nTotalMutants = pExeRetMode->nMutantEnd-pExeRetMode->nMutantInit;
            else
                nTotalMutants = (pExeRetMode->nMutantEnd-pExeRetMode->nMutantInit)+1;

            if (EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - allocating for %d md5's\n", m_nRank, nTotalMutants);
            strMd5List = malloc (nTotalMutants*sizeof(char*)+1);
            nTotalSent = nTotalMutants+1;
            //execute
            if (pExeRetMode) {
                if (EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - Calculating md5's\n", m_nRank);

                nIndex = 0;
                //Md5Hash of all the mutants    
                for(i=pExeRetMode->nMutantInit; i<=pExeRetMode->nMutantEnd;i++)
                {
                    if (EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - Generating equivalent line md5's: %d\n", m_nRank, i);
                    strMutantFile = (char*) buildEquivLine(i,BUILD_LINE_MUTANT_MODE);        
                    
                    if(strMutantFile != NULL)
                    {
                        //sprintf (exeLine, "openssl md5 -r %s | cut -f 1 -d \" \"", strMutantFile);        
                        //strMd5 = (char*) execCommandLineHPC("%s", exeLine);

                        sprintf (exeLine, "openssl md5 -binary %s", strMutantFile);  
                        strMd5 = (char*) execCommandLineHPC("%s", exeLine);
                        if(strMd5 != NULL)
                        {
                            //Copy the result to the list
                            nLen = (strlen(strMd5)*sizeof(char));
                            strMd5List[nIndex] = malloc(nLen+1); 
                            bzero(strMd5List[nIndex], nLen);                        
                            strcpy(strMd5List[nIndex], strMd5);   
                            nTotalSent+=strlen(strMd5)+1;
                            //free(strMd5);                        
                        }
                        else
                        {
                            strMd5List[nIndex]=NULL;
                        }

                        if(strMd5List[nIndex] != NULL)
                        {
                            if(EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - %d - Md5: %s", m_nRank, i, strMd5List[nIndex]);
                        }
                        else
                        {
                            if(EQUIVALENCE_PRINT)  printf("<%d>parallelEquivalenceChecking - %d - null!\n", m_nRank, i);
                        }
                    }
                    else
                        printf("<%d>parallelEquivalenceChecking - Empty line while calculating Md5 on worker\n", m_nRank);
                    
                    nIndex++;
                }

                if (EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - Sending results to master\n", m_nRank);

                //send results to master!
                sendMd5Mutants(pExeRetMode, strMd5List, nTotalSent, MALONE_MASTER);

                if (EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - Worker <%d> finished!\n", m_nRank, m_nRank);
            } else
                printf("<%d>distribution_static_mutants - WARNING! Deploy mode is empty!!!\n", m_nRank);
        }
        
    }

    
    if(m_nRank == MALONE_MASTER)
    {
        int nEquivalent;
        if(EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - Checking cloned and equivalent mutants\n", m_nRank);
        
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
    }
    if (EQUIVALENCE_PRINT) printf("<%d>parallelEquivalenceChecking - End\n", m_nRank);
}

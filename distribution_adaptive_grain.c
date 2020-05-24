/* 
 * File:   distribution_adaptive_grain_scatter.c
 * Author: Pablo C. Cañizares
 *
 * Created on August 15, 2018, 12:54 PM
 */
#include "DistributionAlgorithms.h"
#include "Auxiliars.h"

#define MINIMUM_MUTANTS m_nSize-2

/**
 * This distribution takes advance of two previously designed algorithms: dynamic_mutants and  full_dynamic_scatter. 
 * As a result, we present a mixed algorithm which changes the size of the execution grain depending on the remaining workload.
 * @param eMethod
 * @return 
 */
int distribution_adaptive_grain_scatter(T_eExecutionMode eMethod) 
{    
    int nMutants, nTests, nSent, nIndex, nAlive, nWorkerSource, nFinish, nRemainBlocks, nCounterAux, nFirst, nRemainingMutants;
    int nIndexMutantInit, nIndexMutantEnd, nIndexTestInit, nIndexTestEnd;
    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;
    T_stTestInfo* pTest;
    T_stMutant* pMutant;
    MutantList* pMutantList;
    
    if (MALONE_DEBUG_DIST_COMMON) printf("<%d>distribution_adaptive_grain_scatter - Init\n", m_nRank);
    if (m_nRank == MALONE_MASTER) {
        if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_adaptive_grain_scatter - Master mode!\n", m_nRank);
        //Initialize
        nMutants = m_stEnvValues->nTotalMutants;
        nRemainingMutants = nMutants;
        nTests = m_stEnvValues->nTotalTests;
        pMutantList = NULL;
        
        m_oTestExecMap.nMutants = nMutants;
        m_oTestExecMap.nTests = nTests;

        nFinish = nIndexMutantInit = nIndexMutantEnd = nIndexTestInit = nIndexTestEnd = 0;
        nCounterAux = nSent = 0;
        nIndex = nFirst = 1;
        nWorkerSource = -1;

        //Si te estas preguntando por que usamos mutant init y mutant end, es por que queremos hacer un metodo general.
        //Donde se permita mandar varios mutantes
        if (m_stEnvValues->nStartingMutant != 0) {
            nIndexMutantInit = m_stEnvValues->nStartingMutant;
            nIndexMutantEnd = m_stEnvValues->nStartingMutant;
            nCounterAux = m_stEnvValues->nStartingMutant;
        }

        if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_adaptive_grain_scatter - Starting at mutant: %d\n", m_nRank, nIndex);
        if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_adaptive_grain_scatter - Distributing %d mutants with %d tests,  between %d workers\n", m_nRank, nMutants, nTests, m_nSize - 1);

        //Fill the env values to send it to workers
        while (nIndex < m_nSize) {
            //Initialize            
            exeVector[nIndex].nExecutionMode = (int) eMethod;
            exeVector[nIndex].nWorker = nIndex;            
            
            if(m_stEnvValues->nClusterMutants == 0)
            {
                if (nIndex <= nMutants) {
                    if(nRemainingMutants > MINIMUM_MUTANTS)
                    {
                        exeVector[nIndex].nTestInit = 0;
                        exeVector[nIndex].nTestEnd = m_stEnvValues->nTotalTests - 1;
                        exeVector[nIndex].nMutantInit = nIndexMutantInit;
                        exeVector[nIndex].nMutantEnd = nIndexMutantEnd;
                    }
                    else
                    {
                        exeVector[nIndex].nTestInit = nIndexTestInit;
                        exeVector[nIndex].nTestEnd = nIndexTestEnd;
                        exeVector[nIndex].nMutantInit = -1;
                        exeVector[nIndex].nMutantEnd = -1;
                    }

                } else {
                    exeVector[nIndex].nTestInit = -1;
                    exeVector[nIndex].nTestEnd = -1;
                    exeVector[nIndex].nMutantInit = -1;
                    exeVector[nIndex].nMutantEnd = -1;
                }
            }
            else
            {
                exeVector[nIndex].nTestInit = exeVector[nIndex].nTestEnd = -1;
                exeVector[nIndex].nMutantInit = exeVector[nIndex].nMutantEnd = -1;
            }
            if (m_stEnvValues->nClusterMutants == NORMAL_EQUIV_METHOD)   
                nFinish = updateCounters_adaptive_equiv_aggresive(&m_oTestExecMap, &exeVector[nIndex], &nIndexTestInit, &nIndexTestEnd, &nIndexMutantInit, &nIndexMutantEnd);
            else if (m_stEnvValues->nClusterMutants == AGGRESIVE_EQUIV_METHOD) 
                nFinish = updateCounters_adaptive_equiv_aggresive(&m_oTestExecMap, &exeVector[nIndex], &nIndexTestInit, &nIndexTestEnd, &nIndexMutantInit, &nIndexMutantEnd);
            else
                nFinish = updateCounters_adaptive(&m_oTestExecMap, &exeVector[nIndex], &nIndexTestInit, &nIndexTestEnd, &nIndexMutantInit, &nIndexMutantEnd);
            
            //Ensure the reorder (???)
            exeVector[nIndex].nTestInit = nIndexTestInit;
            exeVector[nIndex].nTestEnd = nIndexTestEnd;
            exeVector[nIndex].nMutantInit = nIndexMutantInit;
            exeVector[nIndex].nMutantEnd = nIndexMutantEnd;
            
            if(nFinish == 0)
                nSent++;

            if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_adaptive_grain_scatter - Distributing Mutant [%d-%d].[%d-%d] to worker %d | Remaining blocks: %d | elapsed: %ld \n", m_nRank, nIndexMutantInit, nIndexMutantEnd, nIndexTestInit, nIndexTestEnd, nIndex, nSent, (long int) getTick() - m_lMutantInitTick);            
            sendDeployMode(&exeVector[nIndex], nIndex);

            //Inc worker counter
            nIndex++;
        }
        nRemainBlocks = nSent;
        
        //Wait for receiving values from all workers and filling final result
        do {
            pMutantList = receiveMutants_adaptive(exeVector, &nWorkerSource);
            nRemainBlocks--;            
            
            if (hasRemainingWork(nIndexMutantInit, nIndexTestInit)) {

                nAlive = checkMutantsAlive(pMutantList);
                //If the mutant that failed test, is currently tested -> KO!
                if (nAlive == 0){              
                    if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_adaptive_grain_scatter - Mutant %d has dead in test %d | [worker -  %d]\n", m_nRank, nIndexMutantInit, nIndexTestInit, nWorkerSource);                    
                    updateKillMutant_adaptive(&m_oTestExecMap, &exeVector[nWorkerSource], pMutantList);
                }
                
                if (m_stEnvValues->nClusterMutants == NORMAL_EQUIV_METHOD)   
                    nFinish = updateCounters_adaptive_equiv_aggresive(&m_oTestExecMap, &exeVector[nWorkerSource], &nIndexTestInit, &nIndexTestEnd, &nIndexMutantInit, &nIndexMutantEnd);
                else if (m_stEnvValues->nClusterMutants == AGGRESIVE_EQUIV_METHOD) 
                    nFinish = updateCounters_adaptive_equiv_aggresive(&m_oTestExecMap, &exeVector[nWorkerSource], &nIndexTestInit, &nIndexTestEnd, &nIndexMutantInit, &nIndexMutantEnd);
                else
                    nFinish = updateCounters_adaptive(&m_oTestExecMap, &exeVector[nWorkerSource], &nIndexTestInit, &nIndexTestEnd, &nIndexMutantInit, &nIndexMutantEnd);

            }

            //If there is remaining mutants to distribute, send it!
            if (nFinish == 0) {
                exeVector[nWorkerSource].nMutantInit = nIndexMutantInit;
                exeVector[nWorkerSource].nMutantEnd = nIndexMutantEnd;
                exeVector[nWorkerSource].nTestInit = nIndexTestInit;
                exeVector[nWorkerSource].nTestEnd = nIndexTestEnd;
                nRemainBlocks++;
                if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_adaptive_grain_scatter - Distributing Mutant [%d-%d].[%d-%d] to worker %d | Remaining %d |elapsed: %ld\n", m_nRank, nIndexMutantInit, nIndexMutantEnd, nIndexTestInit, nIndexTestEnd, nWorkerSource, nRemainBlocks, (long int) getTick() - m_lMutantInitTick);
                
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);

            } else {
                //Worker has finished its work.
                if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_adaptive_grain_scatter - Worker %d has finished!\n", m_nRank, nWorkerSource);
                if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_adaptive_grain_scatter - Remaining blocks: %d\n", m_nRank, nRemainBlocks);
                exeVector[nWorkerSource].nMutantInit = exeVector[nWorkerSource].nMutantEnd = exeVector[nWorkerSource].nTestInit = exeVector[nWorkerSource].nTestEnd = -1;
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);
            }
            
            if(pMutantList)
                freeMutantList(pMutantList);
            
        } while (nRemainBlocks);
        if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_adaptive_grain_scatter - Master finished!!\n", m_nRank);
    } 
    //MALONE_WORKERS
    else {
        nFinish = 0;
        pMutantList = NULL;
        if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_adaptive_grain_scatter - Workers mode!\n", m_nRank);

        do {
            //receive the deployment method
            if(COMPATIBLE_MODE)
                receiveDeployModeRefP(&pExeRetMode);
            else
                pExeRetMode = receiveDeployMode();

            if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_adaptive_grain_scatter - Received deploy mode!\n", m_nRank);

            printDeployStruct(pExeRetMode);
            nFinish = (pExeRetMode == NULL || pExeRetMode->nMutantInit == -1);
            
            //Execute
            if (!nFinish) {
                if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_adaptive_grain_scatter - Executing mutants\n", m_nRank);
                //executing mutants using the scheme received
                malone_execute_mutants_by_scheme(pExeRetMode);

                //send results to master!
                nIndexTestInit = pExeRetMode->nTestInit;
                nIndexTestEnd = pExeRetMode->nTestEnd;
                nIndexMutantInit = pExeRetMode->nMutantInit;
                nIndexMutantEnd = pExeRetMode->nMutantEnd;
                if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_adaptive_grain_scatter - Sending results to master [M: <%d-%d> - T: <%d-%d>]\n", m_nRank, nIndexMutantInit, nIndexMutantEnd, nIndexTestInit, nIndexTestEnd);

                //The main idea is to return always a list of mutants, not only a single mutants, or a test
                //send results to master!
                sendMutants(pExeRetMode, MALONE_MASTER);
                
                //Reset the mutant in order to no re-send it
                resetMutant(pExeRetMode);
                
            } else {
                if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_adaptive_grain_scatter - Worker <%d> finished!\n", m_nRank, m_nRank);
            }
        } while (!nFinish);
    }

    if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_adaptive_grain_scatter - End\n", m_nRank);
}



int updateCounters_adaptive(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, int* nIndexTestInit, int* nIndexTestEnd, int* nIndexMutantInit, int* nIndexMutantEnd) {
    int nNeedRealloc, nSearch, nIndex, nFinish, nRemainingWork;
    nNeedRealloc = nFinish = 0;
    T_stMutantExecution* pMutantExec;

    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - Init\n",m_nRank);

    if (exeMap != NULL && exeWorker != NULL && nIndexTestInit != NULL && nIndexTestEnd != NULL 
            && nIndexMutantInit != NULL && nIndexMutantEnd != NULL) {
        
        if(exeMap->nCollapsed == 0)
            nRemainingWork = exeMap->nMutants - exeMap->nIndexMutant;
        else
            nRemainingWork = 0;
        
        //First, check if a worker has associated work
        if (exeWorker->nMutantInit != -1) {
            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - Worker %d has associated the mutant %d (Remaining mutants: %d) (Map index mutant: %d)\n", m_nRank, exeWorker->nWorker, exeWorker->nMutantInit, nRemainingWork, exeMap->nIndexMutant);
            nNeedRealloc = 0;

            //Analise the actual status of the adaptive algorithm
            if(nRemainingWork > MINIMUM_MUTANTS)
            {
                if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - Worker %d Need realloc! has associated the mutant %d (Remaining mutants: %d)\n", m_nRank, exeWorker->nWorker, exeWorker->nMutantInit, nRemainingWork);
                     
                if (exeWorker->nMutantInit <= exeMap->nMutants) 
                {
                    pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                    //Check if the mutant has been full analysed
                    if(pMutantExec && (exeWorker->nTestEnd >= exeMap->nTests-1))
                    {
                         //The mutant has finished!! Mark it and realloc!
                        if (pMutantExec->nFinished == 0) {
                            exeMap->nRemainingMutants--;
                            pMutantExec->nFinished = 1;
                            if (MALONE_DEBUG_DIST_UPDATE)printf("<%d>updateCounters_adaptive - The mutant %d has been marked as finished, indexTest: %d\n", m_nRank, exeWorker->nMutantInit, pMutantExec->nIndexTest);
                        }
                        exeWorker->nMutantInit = -1;
                        exeWorker->nMutantEnd = -1;
                        pMutantExec->nManaged--;
                        
                        //Ensure, the execution is finished
                        pMutantExec->nIndexTest = exeMap->nTests;
                    }
                    else
                        printf("Check: indexTest (%d) >= exeMap->nTests-1 (%d)\n",exeWorker->nTestEnd, exeMap->nTests-1);
                }
                nNeedRealloc=1;
            }
            else
            {
                //We need to check the execution of the mutant.
                if (exeWorker->nMutantInit <= exeMap->nMutants) {
                    pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];

                    if(pMutantExec)
                    {
                        pMutantExec->nIndexTest++;
                        if (pMutantExec->nFinished == 0 && pMutantExec->nIndexTest < exeMap->nTests && (exeWorker->nTestEnd < exeMap->nTests-1)) {
                            //Set the index and the mutant
                            *nIndexTestInit = pMutantExec->nIndexTest;
                            *nIndexTestEnd = pMutantExec->nIndexTest;
                            *nIndexMutantInit = exeWorker->nMutantInit;
                            *nIndexMutantEnd = exeWorker->nMutantEnd;
                            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - (fine-grain-mode) Worker %d has associated the mutant %d test %d (Remaining mutants: %d)\n", m_nRank, exeWorker->nWorker, exeWorker->nMutantInit, exeWorker->nTestInit, nRemainingWork);
                            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - (fine-grain-mode) Worker %d = [%d < %d && (%d < %d)]", m_nRank, exeWorker->nWorker,pMutantExec->nIndexTest, exeMap->nTests, exeWorker->nTestEnd, exeMap->nTests-1);
                        } else 
                        {
                            //The mutant has finished!! Mark it and realloc!
                            if (pMutantExec->nFinished == 0) {
                                exeMap->nRemainingMutants--;
                                pMutantExec->nFinished = 1;
                                if (MALONE_DEBUG_DIST_UPDATE)printf("<%d>updateCounters_adaptive - The mutant %d has been marked as finished, indexTest: %d\n", m_nRank, exeWorker->nMutantInit, pMutantExec->nIndexTest);
                            }
                            exeWorker->nMutantInit = -1;
                            exeWorker->nMutantEnd = -1;
                            nNeedRealloc = 1;
                            pMutantExec->nManaged--;
                        }
                    }
                    else
                    {
                        nFinish = 1;
                        printf("<%d>WARNING!!!! updateCounters has a NULL mutant exec [%d vs %d]\n", m_nRank, exeWorker->nMutantInit, exeMap->nMutants);
                    }
                } else
                    printf("<%d>WARNING!!!! updateCounters is increasing an invalid mutant [%d vs %d]\n", m_nRank, exeWorker->nMutantInit, exeMap->nMutants);
            }            
        } else
            nNeedRealloc = 1;

       
        //If its neccesary to change the mutant index.
        if (nNeedRealloc == 1) {
            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - Worker %d needs realloc\n", m_nRank, exeWorker->nWorker);
            if (exeMap->nCollapsed == 0) {
                if (exeMap->nIndexMutant == -1)
                    exeMap->nIndexMutant = m_stEnvValues->nStartingMutant;
                else
                {
                    //Search for the next mutant available
                    exeMap->nIndexMutant++;
                    nSearch = SEARCH_INITIAL;
                    nIndex = exeMap->nIndexMutant;
                    searchNextMutantNotProcessed(exeMap, &nSearch, &nIndex, &pMutantExec);
                }

                pMutantExec = exeMap->oMutantMap[exeMap->nIndexMutant];

                //Update the remaining 
                nRemainingWork = exeMap->nMutants - exeMap->nIndexMutant;
                
                //If the indexMutant has not reached the maximum, get param and increment.
                if (exeMap->nIndexMutant <= exeMap->nMutants) {
                    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - Worker %d associated mutant %d\n", m_nRank, exeWorker->nWorker, exeMap->nIndexMutant);

                    //Set the index
                    (*nIndexMutantInit) = exeMap->nIndexMutant;
                    (*nIndexMutantEnd) = exeMap->nIndexMutant;
                    (*nIndexTestInit) = 0;
                   
                    if(nRemainingWork > MINIMUM_MUTANTS)
                        (*nIndexTestEnd)= exeMap->nTests-1;
                    else
                        (*nIndexTestEnd)= 0;

                    
                    exeWorker->nMutantInit = exeMap->nIndexMutant;
                    exeWorker->nMutantEnd = exeMap->nIndexMutant;
                    exeWorker->nTestInit = *nIndexTestInit;
                    exeWorker->nMutantEnd = *nIndexTestEnd;
                    
                    //Update the list
                    pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                    if (pMutantExec)
                        pMutantExec->nManaged++;
                    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - associated! test %d\n", m_nRank, exeMap->nTests);                    
                    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - Worker: %d | Mutant: [%d-%d] | Test: [%d-%d] | Finished: %d\n", m_nRank, exeWorker->nWorker, *nIndexMutantInit, *nIndexMutantEnd, *nIndexTestInit, *nIndexTestEnd, nFinish);
                } else {
                    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - The execution has collapsed %d vs %d\n", m_nRank, exeMap->nIndexMutant, exeMap->nMutants);
                    exeMap->nCollapsed = 1;
                    exeMap->nIndexMutant = -1;
                }
            }
             // The problem has collapsed, the number of workers remaining is greater than the mutants
            if (exeMap->nCollapsed == 1) {
                //Second round! Search non finished mutants!
                if (exeMap->nIndexMutant == -1)
                    exeMap->nIndexMutant = m_stEnvValues->nStartingMutant;

                nSearch = SEARCH_INITIAL;
                nIndex = exeMap->nIndexMutant;
                //Search the first mutant which is not finished
                while (nSearch > 0) {

                    searchNextMutantNotProcessed(exeMap, &nSearch, &nIndex, &pMutantExec);

                    if (nSearch == 0 && pMutantExec != NULL) {
                        
                        //Mutant index found!
                        pMutantExec->nIndexTest++;
                        pMutantExec->nManaged++;
                        exeWorker->nMutantInit = nIndex;
                        exeWorker->nMutantEnd = nIndex;
                        
                        *nIndexTestInit = pMutantExec->nIndexTest;
                        *nIndexTestEnd = pMutantExec->nIndexTest;
                        *nIndexMutantInit = exeWorker->nMutantInit;
                        *nIndexMutantEnd = exeWorker->nMutantEnd;

                        //Increasing index to distribute the workers
                        nFinish = 0;
                    } else {
                        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - Worker %d: All the mutants has finished?\n", m_nRank, exeWorker->nWorker);
                        nFinish = 1;
                    }
                }
            }
        }
        if (nRemainingWork < MINIMUM_MUTANTS && m_stEnvValues->nSortTestSuite) {
            //*nIndexTestInit = m_oSortedIndexTest[*nIndexTestInit];
        }

        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - Worker: %d | Mutant: [%d-%d] | Test: [%d-%d] | Finished: %d\n", m_nRank, exeWorker->nWorker, *nIndexMutantInit, *nIndexMutantEnd, *nIndexTestInit, *nIndexTestEnd, nFinish);

    }

    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive - End\n", m_nRank);

    return nFinish;
}

int updateCounters_adaptive_equiv_aggresive(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, int* nIndexTestInit, int* nIndexTestEnd, int* nIndexMutantInit, int* nIndexMutantEnd) {
    int nNeedRealloc, nSearch, nIndex, nFinish, nRemainingWork;
    nNeedRealloc = nFinish = 0;
    T_stMutantExecution* pMutantExec;

    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - Init\n",m_nRank);

    if (exeMap != NULL && exeWorker != NULL && nIndexTestInit != NULL && nIndexTestEnd != NULL 
            && nIndexMutantInit != NULL && nIndexMutantEnd != NULL) {
        
        if(exeMap->nCollapsed == 0)
            nRemainingWork = exeMap->nMutants - exeMap->nIndexMutant;
        else
            nRemainingWork = 0;
        
        //First, check if a worker has associated work
        if (exeWorker->nMutantInit != -1) {
            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - Worker %d has associated the mutant %d (Remaining mutants: %d) (Map index mutant: %d)\n", m_nRank, exeWorker->nWorker, exeWorker->nMutantInit, nRemainingWork, exeMap->nIndexMutant);
            nNeedRealloc = 0;

            //Analise the actual status of the adaptive algorithm
            if(nRemainingWork > MINIMUM_MUTANTS)
            {
                if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - Worker %d Need realloc! has associated the mutant %d (Remaining mutants: %d)\n", m_nRank, exeWorker->nWorker, exeWorker->nMutantInit, nRemainingWork);
                     
                if (exeWorker->nMutantInit <= exeMap->nMutants) 
                {
                    pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                    //Check if the mutant has been full analysed
                    if(pMutantExec && (exeWorker->nTestEnd >= exeMap->nTests-1))
                    {
                         //The mutant has finished!! Mark it and realloc!
                        if (pMutantExec->nFinished == 0) {
                            exeMap->nRemainingMutants--;
                            pMutantExec->nFinished = 1;
                            if (MALONE_DEBUG_DIST_UPDATE)printf("<%d>updateCounters_adaptive_equiv_aggresive - The mutant %d has been marked as finished, indexTest: %d\n", m_nRank, exeWorker->nMutantInit, pMutantExec->nIndexTest);
                        }
                        exeWorker->nMutantInit = -1;
                        exeWorker->nMutantEnd = -1;
                        pMutantExec->nManaged--;
                        
                        //Ensure, the execution is finished
                        pMutantExec->nIndexTest = exeMap->nTests;
                    }
                    else
                        printf("Check: indexTest (%d) >= exeMap->nTests-1 (%d)\n",exeWorker->nTestEnd, exeMap->nTests-1);
                }
                nNeedRealloc=1;
            }
            else
            {
                //We need to check the execution of the mutant.
                if (exeWorker->nMutantInit <= exeMap->nMutants) {
                    pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];

                    pMutantExec->nIndexTest++;
                    if (pMutantExec->nFinished == 0 && pMutantExec->nIndexTest < exeMap->nTests && (exeWorker->nTestEnd < exeMap->nTests-1)) {
                        //Set the index and the mutant
                        *nIndexTestInit = pMutantExec->nIndexTest;
                        *nIndexTestEnd = pMutantExec->nIndexTest;
                        *nIndexMutantInit = exeWorker->nMutantInit;
                        *nIndexMutantEnd = exeWorker->nMutantEnd;
                        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - (fine-grain-mode) Worker %d has associated the mutant %d test %d (Remaining mutants: %d)\n", m_nRank, exeWorker->nWorker, exeWorker->nMutantInit, exeWorker->nTestInit, nRemainingWork);
                        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - (fine-grain-mode) Worker %d = [%d < %d && (%d < %d)]", m_nRank, exeWorker->nWorker,pMutantExec->nIndexTest, exeMap->nTests, exeWorker->nTestEnd, exeMap->nTests-1);
                    } else {
                        //The mutant has finished!! Mark it and realloc!
                        if (pMutantExec->nFinished == 0) {
                            exeMap->nRemainingMutants--;
                            pMutantExec->nFinished = 1;
                            if (MALONE_DEBUG_DIST_UPDATE)printf("<%d>updateCounters_adaptive_equiv_aggresive - The mutant %d has been marked as finished, indexTest: %d\n", m_nRank, exeWorker->nMutantInit, pMutantExec->nIndexTest);
                        }
                        exeWorker->nMutantInit = -1;
                        exeWorker->nMutantEnd = -1;
                        nNeedRealloc = 1;
                        pMutantExec->nManaged--;
                    }
                } else
                    printf("<%d>WARNING!!!! updateCounters is increasing an invalid mutant [%d vs %d]\n", m_nRank, exeWorker->nMutantInit, exeMap->nMutants);
            }            
        } else
            nNeedRealloc = 1;

       
        //If its neccesary to change the mutant index.
        if (nNeedRealloc == 1) {
            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - Worker %d needs realloc\n", m_nRank, exeWorker->nWorker);
            if (exeMap->nCollapsed == 0) {
                if (exeMap->nIndexMutant == -1)
                    exeMap->nIndexMutant = m_stEnvValues->nStartingMutant;
                else
                {
                    //Search for the next mutant available
                    exeMap->nIndexMutant++;                                        
                }
                //Search the next mutant not processed, not equivalent and not dupped child
                nSearch = SEARCH_INITIAL;
                nIndex = exeMap->nIndexMutant;
                
                
                while (nSearch > 0) {

                    searchNextMutantNotProcessed_equiv_aggresive(exeMap, &nSearch, &nIndex, &pMutantExec);

                }
                if(nSearch ==0)
                {    
                    exeMap->nIndexMutant = nIndex;
                    pMutantExec = exeMap->oMutantMap[exeMap->nIndexMutant];

                    //Update the remaining 
                    nRemainingWork = exeMap->nMutants - exeMap->nIndexMutant;

                    //If the indexMutant has not reached the maximum, get param and increment.
                    if (exeMap->nIndexMutant <= exeMap->nMutants) {
                        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - Worker %d associated mutant %d\n", m_nRank, exeWorker->nWorker, exeMap->nIndexMutant);

                        //Set the index
                        (*nIndexMutantInit) = exeMap->nIndexMutant;
                        (*nIndexMutantEnd) = exeMap->nIndexMutant;
                        (*nIndexTestInit) = 0;

                        if(nRemainingWork > MINIMUM_MUTANTS)
                            (*nIndexTestEnd)= exeMap->nTests-1;
                        else
                            (*nIndexTestEnd)= 0;


                        exeWorker->nMutantInit = exeMap->nIndexMutant;
                        exeWorker->nMutantEnd = exeMap->nIndexMutant;
                        exeWorker->nTestInit = *nIndexTestInit;
                        exeWorker->nMutantEnd = *nIndexTestEnd;

                        //Update the list
                        pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                        if (pMutantExec)
                            pMutantExec->nManaged++;
                        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - associated! test %d\n", m_nRank, exeMap->nTests);                    
                        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - Worker: %d | Mutant: [%d-%d] | Test: [%d-%d] | Finished: %d\n", m_nRank, exeWorker->nWorker, *nIndexMutantInit, *nIndexMutantEnd, *nIndexTestInit, *nIndexTestEnd, nFinish);
                    } else {
                        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - The execution has collapsed %d vs %d\n", m_nRank, exeMap->nIndexMutant, exeMap->nMutants);
                        exeMap->nCollapsed = 1;
                        exeMap->nIndexMutant = -1;
                    }
                }
                else
                {
                    exeMap->nCollapsed = 1;
                    exeMap->nIndexMutant = -1;
                }
                
            }
             // The problem has collapsed, the number of workers remaining is greater than the mutants
            if (exeMap->nCollapsed == 1) {
                //Second round! Search non finished mutants!
                if (exeMap->nIndexMutant == -1)
                    exeMap->nIndexMutant = m_stEnvValues->nStartingMutant;

                nSearch = SEARCH_INITIAL;
                nIndex = exeMap->nIndexMutant;
                //Search the first mutant which is not finished
                while (nSearch > 0) {

                    searchNextMutantNotProcessed_equiv_aggresive(exeMap, &nSearch, &nIndex, &pMutantExec);

                    if (nSearch == 0 && pMutantExec!= NULL) {
                        
                        //Mutant index found!
                        if(pMutantExec->nManaged !=0)
                            pMutantExec->nIndexTest++;
                        
                        pMutantExec->nManaged++;
                        exeWorker->nMutantInit = nIndex;
                        exeWorker->nMutantEnd = nIndex;
                        
                        //Final guard to avoid surprises!!
                        //if((pMutantExec->nIndexTest <= exeMap->nTests - 1))
                        //{
                            *nIndexTestInit = pMutantExec->nIndexTest;
                            *nIndexTestEnd = pMutantExec->nIndexTest;
                            *nIndexMutantInit = exeWorker->nMutantInit;
                            *nIndexMutantEnd = exeWorker->nMutantEnd;
                        //}
                        //else
                        //    nSearch = SEARCH_INITIAL;
                        //Increasing index to distribute the workers
                        nFinish = 0;
                    } else {
                        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - Worker %d: All the mutants has finished?\n", m_nRank, exeWorker->nWorker);
                        nFinish = 1;
                    }
                }
            }
        }
        if (nRemainingWork < MINIMUM_MUTANTS && m_stEnvValues->nSortTestSuite) {
            //*nIndexTestInit = m_oSortedIndexTest[*nIndexTestInit];
        }

        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - Worker: %d | Mutant: [%d-%d] | Test: [%d-%d] | Finished: %d\n", m_nRank, exeWorker->nWorker, *nIndexMutantInit, *nIndexMutantEnd, *nIndexTestInit, *nIndexTestEnd, nFinish);

    }

    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>updateCounters_adaptive_equiv_aggresive - End\n", m_nRank);

    return nFinish;
}

int checkMutantsAlive(MutantList* pMutantList)
{
    int nRet;
    T_stMutant* pMutant;
    nRet = 1;
    
    if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>checkMutantsAlive - Init\n", m_nRank);
    
    if(pMutantList != NULL)
    {
        nRet = (pMutantList->nDead == 0);
        if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>checkMutantsAlive - TotalMutants dead: %d\n", m_nRank, pMutantList->nDead);
        //Check for the mutants dead
        if(pMutantList->nDead>0)
        {
            for(int i=0;i<pMutantList->nElems;i++)
            {
                pMutant = pMutantList->array[i];
            }
        }
        
    }
    
    if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>checkMutantsAlive - End\n", m_nRank);
    return nRet;
}

//TODO: Esperar a que funcione con 1, y extendemos a la lista entera de mutantes
void updateKillMutant_adaptive(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, MutantList* pMutantList) {
    T_stMutantExecution* pMutantExec;
    T_stEquivalentInfo* pEquivInfo;
    T_stEquivalentInfo* pEquivInfoClone;
    int i, nClone;
    
    
    if (MALONE_DEBUG_DIST_COMMON) printf("updateKillMutant_adaptive - Init\n");

    if (exeMap != NULL && exeWorker != NULL && exeWorker->nMutantInit != -1) {
        exeMap->nRemainingMutants--;
        pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
        pEquivInfo = exeMap->oEquivalentInfo[exeWorker->nMutantInit];

        if (pMutantExec) {
            if (pMutantExec->nFinished == 1)
                printf("----------------------------------YET KILLED\n");
            pMutantExec->nFinished = 1;


            /* TEMPORALY DISABLED DUE TO DEVELOPMENT PHASE: 
             * if (pEquivInfo && m_stEnvValues->nClusterMutants == NORMAL_EQUIV_METHOD) {
                pEquivInfo->nKillerTest = exeWorker->nTestInit;
                if (pMutantExec->nIsClustered && pEquivInfo->nIsClusterHead) {
                    printf("CLUSTER HEAD HAS DIED!!! Mut: %d Test: %d\n", exeWorker->nMutantInit, pEquivInfo->nKillerTest);

                    //Notifying the clone mutants ...
                    printf("CLUSTER HEAD, notifying %d \n", pEquivInfo->nClones);

                    for (i = 0; i < pEquivInfo->nClones; i++) {
                        nClone = pEquivInfo->oClones[i];
                        if (nClone > 0) {
                            pEquivInfoClone = exeMap->oEquivalentInfo[exeWorker->nMutantInit];
                            if (pEquivInfoClone != NULL) {
                                pEquivInfoClone->nKillerTest = pEquivInfo->nKillerTest;
                                printf("    - clone %d notified with test: %d\n", nClone, pEquivInfoClone->nKillerTest);
                            }
                        } else printf("    - clone %d invalid!!\n", nClone);
                    }
                } else if (pMutantExec->nIsClustered) {
                    printf("CLUSTER CLONE HAS DIED: %d in test: %d\n", exeWorker->nMutantInit, pEquivInfo->nKillerTest);
                }
            }*/
            if (MALONE_DEBUG_DIST_COMMON) printf("updateKillMutant_adaptive - Worker: %d | Mutant: %d | Test: %d \n", exeWorker->nWorker, exeWorker->nMutantInit, exeWorker->nTestInit);
            exeWorker->nMutantInit = -1;
        }
    } else
        printf("updateKillMutant - PROBLEMS!!\n");

    if (MALONE_DEBUG_DIST_COMMON) printf("updateKillMutant_adaptive - End\n");
}

int searchNextMutantNotProcessed(T_stExecutionMap* exeMap, int* nSearch, int* nIndex, T_stMutantExecution** pMutantExec)
{
    int nMaxWorkersAllowed, nAllMutantsFinished;
    
    nAllMutantsFinished = 1;
    if ((*nIndex) <= exeMap->nMutants) {
        (*pMutantExec) = exeMap->oMutantMap[(*nIndex)];
        
        if ((*pMutantExec) && (*pMutantExec)->nFinished == 0) {
            
            nAllMutantsFinished = 0;
            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>searchNextMutantNotProcessed - Mutant %d NOT finished\n", m_nRank, (*nIndex));
            
            //pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
            if ((*pMutantExec)->nIndexTest < exeMap->nTests - 1) {
                
                //We try to maximise the distribution ...
                if (exeMap->nRemainingMutants != 0 && exeMap->nRemainingMutants <= m_nSize) {
                    
                    nMaxWorkersAllowed = ceil(m_nSize / exeMap->nRemainingMutants);
                    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>searchNextMutantNotProcessed - Managing: %d | nMaxWorkersAllowed= {%d/%d = %d}\n", m_nRank, (*pMutantExec)->nManaged, m_nSize, exeMap->nRemainingMutants, nMaxWorkersAllowed);
                } else
                    (*nSearch) = 0;
                
                if (nMaxWorkersAllowed >= (*pMutantExec)->nManaged)
                {
                    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>searchNextMutantNotProcessed - Mutant %d managed by worker %d\n", m_nRank, (*nIndex), m_nRank);
                    (*nSearch) = 0;
                }
            } else {
                if (MALONE_DEBUG_DIST_UPDATE)printf("<%d>searchNextMutantNotProcessed - The mutant %d has been marked as finished, indexTest: %d\n", m_nRank, (*nIndex), (*pMutantExec)->nIndexTest);
                
                (*pMutantExec)->nFinished = 1;
                exeMap->nRemainingMutants--;
            }
        } else {
            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>searchNextMutantNotProcessed - Mutant %d finished\n", m_nRank, (*nIndex));
            
        }
        if ((*nSearch) != 0)
            (*nIndex)++;
    } else {
        (*nIndex) = m_stEnvValues->nStartingMutant;
        if(nAllMutantsFinished == 0)
            (*nSearch) = (*nSearch) - 2;
        else
            (*nSearch) = -1;
    }
    return *nSearch;            
}

int searchNextMutantNotProcessed_equiv_aggresive(T_stExecutionMap* exeMap, int* nSearch, int* nIndex, T_stMutantExecution** pMutantExec)
{
    int nAuxSearch;
    T_stEquivalentInfo* pEquivInfo;
    int nMaxWorkersAllowed, nAllMutantsFinished, nRemainingWork;
    
    nAuxSearch = (*nSearch);
    
    nAllMutantsFinished = 1;
    if ((*nIndex) <= exeMap->nMutants) {
        (*pMutantExec) = exeMap->oMutantMap[(*nIndex)];
        pEquivInfo = exeMap->oEquivalentInfo[(*nIndex)];
        
        if ((*pMutantExec) && (*pMutantExec)->nFinished == 0) {
            
            nAllMutantsFinished = 0;
            //If the mutant is equivalent or cloned mutant (not cluster)
            if((*pMutantExec)->nEquivalent || 
              ((*pMutantExec)->nIsClustered && pEquivInfo && pEquivInfo->nIsClusterHead == 0))
            {                      
                //Transform the equivalent and dupped not cluster header to sand
                (*pMutantExec)->nFinished = 1;
                exeMap->nRemainingMutants--;
                if (MALONE_DEBUG_DIST_UPDATE) 
                {
                    if((*pMutantExec)->nEquivalent)
                        printf("<%d>searchNextMutantNotProcessed_equiv_aggresive - Equivalent mutant detected and marked as finished: %d \n", m_nRank, (*nIndex));
                    else
                        printf("<%d>searchNextMutantNotProcessed_equiv_aggresive - Dupped mutant detected and marked as finished: %d \n", m_nRank, (*nIndex));
                }
            }
            else
            {
                if (MALONE_DEBUG_DIST_UPDATE){
                    printf("<%d>searchNextMutantNotProcessed_equiv_aggresive - Mutant %d NOT finished [E: %d, D: %d, ICH: %d, MH: %d, NC: %d]\n", 
                        m_nRank, (*nIndex), (*pMutantExec)->nEquivalent, (*pMutantExec)->nIsClustered, pEquivInfo->nIsClusterHead, pEquivInfo->nMutantHeader, pEquivInfo->nClones);                
                    printMutantEquiv((*nIndex));
                }
                
                //pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                if ((*pMutantExec)->nIndexTest <= exeMap->nTests - 1) {

                    //We try to maximise the distribution ...
                    if (exeMap->nRemainingMutants != 0 && exeMap->nRemainingMutants <= m_nSize) {

                        nMaxWorkersAllowed = ceil(m_nSize / exeMap->nRemainingMutants);
                        if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>searchNextMutantNotProcessed_equiv_aggresive - Managing: %d | nMaxWorkersAllowed= {%d/%d = %d}\n", m_nRank, (*pMutantExec)->nManaged, m_nSize, exeMap->nRemainingMutants, nMaxWorkersAllowed);
                    } 
                    else
                    {
                        //Check the grain
                        /*if(exeMap->nCollapsed == 0)
                            nRemainingWork = exeMap->nMutants - exeMap->nIndexMutant;
                        else
                            nRemainingWork = 0;
                        
                        //Note that
                        if(nRemainingWork<MINIMUM_MUTANTS)       
                        */
                        (*nSearch) = 0;
                    }

                    if (nMaxWorkersAllowed >= (*pMutantExec)->nManaged)
                    {
                        
                        if(((*pMutantExec)->nManaged == 0 && (*pMutantExec)->nIndexTest <= exeMap->nTests - 1 ) || 
                            (*pMutantExec)->nManaged && (*pMutantExec)->nIndexTest+1 <= exeMap->nTests - 1)
                        {
                            (*nSearch) = 0;
                            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>searchNextMutantNotProcessed_equiv_aggresive - Mutant %d ready to be launched\n", m_nRank, (*nIndex));
                        }
                        else
                        {
                            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>searchNextMutantNotProcessed_equiv_aggresive - This mutant (%d) is being processed, and the next test will be out range\n", m_nRank, (*nIndex));
                            (*nSearch) = nAuxSearch;
                        }
                    }
                                                        
                } else {
                    if (MALONE_DEBUG_DIST_UPDATE)printf("<%d>searchNextMutantNotProcessed_equiv_aggresive - The mutant %d has been marked as finished, indexTest: %d\n", m_nRank, (*nIndex), (*pMutantExec)->nIndexTest);

                    (*pMutantExec)->nFinished = 1;
                    exeMap->nRemainingMutants--;
                    (*pMutantExec)->nManaged--;
                }
            }
            
        } else {
            if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>searchNextMutantNotProcessed_equiv_aggresive - Mutant %d finished\n", m_nRank, (*nIndex));
            
        }
        if ((*nSearch) != 0)
            (*nIndex)++;
    } else {
        (*nIndex) = m_stEnvValues->nStartingMutant;
        if(nAllMutantsFinished == 0)
            (*nSearch) = (*nSearch) - 2;
        else
            (*nSearch) = -1;
    }
    if (MALONE_DEBUG_DIST_UPDATE) printf("<%d>searchNextMutantNotProcessed_equiv_aggresive - End (index: %d, search: %d)\n", m_nRank, (*nIndex),(*nSearch));
}
/* 
 * File:   DistributionAlgorithms.h
 * Author: Pablo C. Cañizares
 *
 * Created on August 15, 2018, 12:54 PM
 */
#include "DistributionAlgorithms.h"

int distribution_full_dynamic_scatter(T_eExecutionMode eMethod) {    
    int nMutants, nTests, nSent, nIndex, nAlive, nWorkerSource, nIndexTest, nIndexMutant, nFinish, nRemainBlocks, nCounterAux, nFirst;

    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;
    T_stTestInfo* pTest;
    T_stMutant* pMutant;

    
    if (MALONE_DEBUG_DIST_COMMON) printf("<%d>distribution_full_dynamic_scatter - Init\n", m_nRank);
    if (m_nRank == MALONE_MASTER) {
        if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_full_dynamic_scatter - Master mode!\n", m_nRank);
        //Initialize
        nMutants = m_stEnvValues->nTotalMutants;
        nTests = m_stEnvValues->nTotalTests;

        m_oTestExecMap.nMutants = nMutants;
        m_oTestExecMap.nTests = nTests;

        nIndexTest = nCounterAux = nFinish = 0;
        nIndexMutant = nFirst = nIndex = 1;
        nWorkerSource = -1;
        
        nIndexMutant = nIndexTest = 0;
        nSent = 0;        
        if (m_stEnvValues->nStartingMutant != 0) {
            nIndexMutant = m_stEnvValues->nStartingMutant;
            nCounterAux = m_stEnvValues->nStartingMutant;
        }

        if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_full_dynamic_scatter - Starting at mutant: %d\n", m_nRank, nIndex);
        if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_full_dynamic_scatter - Distributing %d mutants with %d tests,  between %d workers\n", m_nRank, nMutants, nTests, m_nSize - 1);

        
        //Fill the env values to send it to workers
        while (nIndex < m_nSize) {
            //Initialize            
            exeVector[nIndex].nExecutionMode = (int) eMethod;
            exeVector[nIndex].nWorker = nIndex;

            if (nIndex <= nMutants) {
                exeVector[nIndex].nTestInit = nIndexTest;
                exeVector[nIndex].nTestEnd = nIndexTest;
                exeVector[nIndex].nMutantInit = -1;
                exeVector[nIndex].nMutantEnd = -1;                
            } else {
                exeVector[nIndex].nTestInit = -1;
                exeVector[nIndex].nTestEnd = -1;
                exeVector[nIndex].nMutantInit = -1;
                exeVector[nIndex].nMutantEnd = -1;
            }

            if (m_stEnvValues->nClusterMutants == 0)
                nFinish = updateCounters(&m_oTestExecMap, &exeVector[nIndex], &nIndexTest, &nIndexMutant);
            else
                nFinish = updateCounters_Equiv(&m_oTestExecMap, &exeVector[nIndex], &nIndexTest, &nIndexMutant);

            //Ensure the reorder
            exeVector[nIndex].nTestInit = nIndexTest;
            exeVector[nIndex].nTestEnd = nIndexTest;

            if(nFinish == 0)
                nSent++;
            
            sendDeployMode(&exeVector[nIndex], nIndex);

            if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_full_dynamic_scatter - Distributing Mutant %d.%d to worker %d| Remaining blocks: %d | elapsed: %d\n", m_nRank, nIndexMutant, nIndexTest, nIndex, nSent, (getTick() - m_lMutantInitTick));
            //Inc counters
            nIndex++;
        }
        nRemainBlocks = nSent;

        //Wait for receiving values from all workers and filling final result
        do {
            nAlive = receiveSingleTestAndCheck(&exeVector, &nWorkerSource);
            nRemainBlocks--;            
            
            if (hasRemainingWork(nIndexMutant, nIndexTest)) {
                if (nAlive == 1) {
                    //Increment mutant and test counters
                    if (m_stEnvValues->nClusterMutants == 0)
                        nFinish = updateCounters(&m_oTestExecMap, &exeVector[nWorkerSource], &nIndexTest, &nIndexMutant);
                    else
                        nFinish = updateCounters_Equiv(&m_oTestExecMap, &exeVector[nWorkerSource], &nIndexTest, &nIndexMutant);
                } else {
                    if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_full_dynamic_scatter - Mutant %d has dead in test %d | [worker -  %d]\n", m_nRank, nIndexMutant, nIndexTest, nWorkerSource);
                    //If the mutant that failed test, is currently tested -> KO!

                    updateKillMutant(&m_oTestExecMap, &exeVector[nWorkerSource]);
                    if (m_stEnvValues->nClusterMutants == 0)
                        nFinish = updateCounters(&m_oTestExecMap, &exeVector[nWorkerSource], &nIndexTest, &nIndexMutant);
                    else
                        nFinish = updateCounters_Equiv(&m_oTestExecMap, &exeVector[nWorkerSource], &nIndexTest, &nIndexMutant);

                }                
            }

            //If there is remaining mutants to distribute, send it!
            if (nFinish == 0) {
                exeVector[nWorkerSource].nMutantInit = exeVector[nWorkerSource].nMutantEnd = nIndexMutant;
                exeVector[nWorkerSource].nTestInit = nIndexTest;
                exeVector[nWorkerSource].nTestEnd = nIndexTest;
                nRemainBlocks++;
                if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_full_dynamic_scatter - Distributing Mutant %d.%d to worker %d | Remaining blocks: %d | Finished: %d | elapsed: %d\n", m_nRank, nIndexMutant, nIndexTest, nWorkerSource, nRemainBlocks, nFinish, (getTick() - m_lMutantInitTick));
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);
                
            } else {
                //Worker has finished its work.
                if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_full_dynamic_scatter - Worker %d has finished!\n", m_nRank, nWorkerSource);
                if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_full_dynamic_scatter - Remaining blocks: %d\n", m_nRank, nRemainBlocks);
                exeVector[nWorkerSource].nMutantInit = -1;
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);
            }
        } while (nRemainBlocks);
        if (MALONE_DEBUG_DIST_MASTER) printf("<%d>distribution_full_dynamic_scatter - Master finished!!\n", m_nRank);
    } else {
        nFinish = 0;
        if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_full_dynamic_scatter - Workers mode!\n", m_nRank);

        //if(DEBUG)
        do {
            //receive the deployment method
            pExeRetMode = receiveDeployMode();

            if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_full_dynamic_scatter - Received deploy mode!\n", m_nRank);

            printDeployStruct(pExeRetMode);
            nFinish = (pExeRetMode == NULL || pExeRetMode->nMutantInit == -1);
            //execute
            if (!nFinish) {
                if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_full_dynamic_scatter - Executing mutants\n", m_nRank);
                //executing mutants using the scheme received
                malone_execute_mutants_by_scheme(pExeRetMode);

                //send results to master!
                nIndexTest = pExeRetMode->nTestInit;
                nIndexMutant = pExeRetMode->nMutantInit;
                if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_full_dynamic_scatter - Sending results to master [M: %d - T: %d]\n", m_nRank, nIndexMutant, nIndexTest);

                pMutant = m_oMutantList.array[nIndexMutant];

                if (pMutant) {
                    pTest = pMutant->oTestList.tests[nIndexTest];
                    if (MALONE_DEBUG_DIST_WORKERS) printTest(pTest);
                }
            } else {
                if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_full_dynamic_scatter - Worker <%d> finished!\n", m_nRank, m_nRank);
            }
        } while (!nFinish);
    }

    if (MALONE_DEBUG_DIST_WORKERS) printf("<%d>distribution_full_dynamic_scatter - End\n", m_nRank);
}
/*John del futuro lunes. Me voy a FMS, espero que te portes.
Hay que refactorizar este método, es infernal, hacer que funcione, pero mñas desgranado, seguir algo aqui es querer palmar.
Despues, pasar a dar soporte a la equivalencia de adaptive-grain mode.*/

int getNextWorkloadGrain_equiv(T_stExecutionMap** exeMap, T_stExecutionStructure** exeWorker, T_stMutantExecution** pMutantExec, int** nIndexTest, int** nIndexMutant){
    
    int nIndexEquiv, nNeedRealloc;
    
    nIndexEquiv = nNeedRealloc= 0;
    if (MALONE_DEBUG_DIST_COMMON) printf("<%d>getNextWorkloadGrain_equiv - Init\n", m_nRank);
    if ((*exeWorker)->nMutantInit != -1) {
        if (MALONE_DEBUG_DIST_COMMON) printf("getNextWorkloadGrain_equiv - Worker %d has associated the mutant %d\n", (*exeWorker)->nWorker, (*exeWorker)->nMutantInit);
        nNeedRealloc = 0;
        
        nIndexEquiv = (*exeWorker)->nMutantInit;
        do {
            (*pMutantExec) = (*exeMap)->oMutantMap[nIndexEquiv];
            nIndexEquiv++;
        } while (nIndexEquiv <= (*exeMap)->nMutants && ((*pMutantExec)->nFinished != 0 || (*pMutantExec)->nEquivalent == 1));
        
        (*exeWorker)->nMutantInit = nIndexEquiv - 1;
        //We need to check the execution of the mutant.
        if ((*exeWorker)->nMutantInit <= (*exeMap)->nMutants) {
            
            (*pMutantExec) = (*exeMap)->oMutantMap[(*exeWorker)->nMutantInit];
            (*pMutantExec)->nIndexTest++;
            
            if ((*pMutantExec)->nFinished == 0 && (*pMutantExec)->nIndexTest < (*exeMap)->nTests) {
                //Set the index and the mutant
                *(*nIndexTest) = (*pMutantExec)->nIndexTest;
                *(*nIndexMutant) = (*exeWorker)->nMutantInit;
            } else {
                //The mutant has finished!! Mark it and realloc!
                if ((*pMutantExec)->nFinished == 0) {
                    (*exeMap)->nRemainingMutants--;
                    (*pMutantExec)->nFinished = 1;
                }
                (*exeWorker)->nMutantInit = -1;
                (*exeWorker)->nMutantEnd = -1;
                nNeedRealloc = 1;
                (*pMutantExec)->nManaged--;
            }
        } else
            printf("WARNING!!!! updateCounters_Equiv is increasing an invalid mutant: %d\n", (*exeWorker)->nMutantInit);
    } else
        nNeedRealloc = 1;
    
    if (MALONE_DEBUG_DIST_COMMON) printf("<%d>getNextWorkloadGrain_equiv - End\n", m_nRank);
    
    return nNeedRealloc;
}
/*
int updateCounters_Equiv(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, int* nIndexTest, int* nIndexMutant) {
    int nNeedRealloc, nSearch, nIndex, nFinish, nSkip, nNormalHandle, nKillThem, nMaxWorkersAllowed;
    T_stMutantExecution* pMutantExec;
    T_stEquivalentInfo* pEquivInfo;
    T_stEquivalentInfo* pEquivInfoHeader;

    nNeedRealloc = nFinish = nMaxWorkersAllowed = 0;

    if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Init\n");

    if (exeMap != NULL && exeWorker != NULL && nIndexTest != NULL && nIndexMutant != NULL) {
        nKillThem = -1;
        
        //First, check if a worker has associated work
        nNeedRealloc = getNextWorkloadGrain_equiv(&exeMap, &exeWorker, &pMutantExec, &nIndexTest, &nIndexMutant);

        //If its neccesary to change the mutant index.
        if (nNeedRealloc == 1) {
            if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Worker %d needs realloc\n", exeWorker->nWorker);
            if (exeMap->nCollapsed == 0) {
                nNormalHandle = checkEquivalence(exeMap, exeWorker, nIndexTest, nIndexMutant);

                //If the mutant is equivalent -> 
                if (nNormalHandle) {
                    //If the indexMutant has not reached the maximum, get param and increment.
                    if (exeMap->nIndexMutant <= exeMap->nMutants) {
                        if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Worker %d associated mutant %d\n", exeWorker->nWorker, exeMap->nIndexMutant);

                        //Set the index
                        *nIndexMutant = exeMap->nIndexMutant;
                        *nIndexTest = 0;
                        exeWorker->nMutantInit = exeMap->nIndexMutant;
                        exeWorker->nMutantEnd = exeMap->nIndexMutant;
                        //Update the list
                        pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                        pMutantExec->nManaged = 1;
                    } else {
                        if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - The execution is collapsed\n");
                        exeMap->nCollapsed = 1;
                        exeMap->nIndexMutant = -1;
                    }
                } else {
                    printf("updateCounters_Equiv - Internal handle!!\n");
                    pMutantExec->nManaged = 1;
                }
            }
            if (exeMap->nCollapsed == 1) {
                //Second round! Search non finished mutants!
                if (exeMap->nIndexMutant == -1)
                    exeMap->nIndexMutant = m_stEnvValues->nStartingMutant;

                nSearch = SEARCH_INITIAL;
                nIndex = exeMap->nIndexMutant;
                //Search the first mutant which is not finished
                while (nSearch > 0) {
                    if (nIndex <= exeMap->nMutants) {
                        pMutantExec = exeMap->oMutantMap[nIndex];

                        if (pMutantExec->nFinished == 0 && pMutantExec->nEquivalent == 0) {
                            //Check if the mutant isnt clusterheader                            
                            //In this case, skip them until all the running mutants are clustered
                            //printf("Test: %d\n",  pMutantExec->nIndexTest);
                            if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters - Mutant %d NOT finished\n", nIndex);
                            nSkip = nKillThem = -1;

                            if (pMutantExec->nIsClustered) {
                                pEquivInfo = exeMap->oEquivalentInfo[nIndex];
                                if (pEquivInfo && !pEquivInfo->nIsClusterHead) {
                                    //Check if has a killer test yet
                                    if (pEquivInfo->nKillerTest != -1) {
                                        nKillThem = pEquivInfo->nKillerTest;
                                        nSearch = 0;
                                        //Uncluster!
                                        pMutantExec->nIsClustered = 0;
                                        printf("updateCounters_Equiv - Clone %d Found killer test: %d \n", nIndex, nKillThem);
                                    } else if (nSearch <= SEARCH_INITIAL - (2 * SEARCH_SINGLEPHASE)) {
                                        printf("updateCounters_Equiv - ITs moment to kill them!! \n");
                                        //All remaining mutants are clustered
                                        //ITs moment to kill them!!
                                        pEquivInfoHeader = exeMap->oEquivalentInfo[pEquivInfo->nMutantHeader];
                                        //Getting the killer test!
                                        if (pEquivInfoHeader) {
                                            pEquivInfo->nKillerTest = pEquivInfoHeader->nKillerTest;
                                            if (MALONE_DEBUG_DIST_COMMON) printf("The killer test of M:%d  is %d\n", nIndex, pEquivInfo->nKillerTest);
                                        }
                                        //Try to check killer test or start by begining
                                        if (pEquivInfo->nKillerTest != -1 && pEquivInfo->nKillerTest < exeMap->nTests) {
                                            nKillThem = pEquivInfo->nKillerTest;
                                            nSearch = 0;
                                            //Uncluster!
                                            pMutantExec->nIsClustered = 0;
                                        } else {
                                            nSearch = 0;
                                            nKillThem = 0;

                                            //Uncluster!!
                                            pMutantExec->nIsClustered = 0;
                                        }
                                    } else
                                        nSkip = 1;
                                }
                            } else {
                                //We try to maximise the distribution ...
                                if (exeMap->nRemainingMutants != 0 && exeMap->nRemainingMutants <= m_nSize) {

                                    nMaxWorkersAllowed = ceil(m_nSize / exeMap->nRemainingMutants);
                                    if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters - Managing: %d | nMaxWorkersAllowed= {%d/%d = %d}\n", pMutantExec->nManaged, m_nSize, exeMap->nRemainingMutants, nMaxWorkersAllowed);

                                    nSearch = 0;
                                    nFinish = 0;
                                } else
                                    nSearch = 0;
                            }
                            if (nSkip == 0) {
                                //pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                                if (pMutantExec->nIndexTest < exeMap->nTests - 1)
                                    nSearch = 0;
                                else
                                    pMutantExec->nFinished = 1;
                            }
                        }
                        if (nSearch != 0)
                            nIndex++;
                    } else {
                        nIndex = m_stEnvValues->nStartingMutant;
                        nSearch = nSearch - SEARCH_SINGLEPHASE;
                    }

                    if (nSearch == 0) {
                        //Mutant index found!

                        //If there is a killing test, use it!!
                        if (nKillThem == -1) {
                            pMutantExec->nIndexTest++;
                        } else {
                            pMutantExec->nIndexTest = nKillThem;
                        }
                        exeWorker->nMutantInit = nIndex;
                        *nIndexTest = pMutantExec->nIndexTest;
                        *nIndexMutant = exeWorker->nMutantInit;

                        nFinish = 0;
                        //Increasing index to distribute the workers
                        //exeMap->nIndexMutant;
                    } else {
                        if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Worker %d: All the mutants has finished?\n", exeWorker->nWorker);
                        nFinish = 1;
                    }
                }
            }
        }
        if (m_stEnvValues->nSortTestSuite && nKillThem != -1) {
            *nIndexTest = m_oSortedIndexTest[*nIndexTest];
        }
        if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Mutant: %d | Test: %d | Finish: %d\n", *nIndexMutant, *nIndexTest, nFinish);
    }

    if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - End\n");

    return nFinish;
}*/
int updateCounters_Equiv(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, int* nIndexTest, int* nIndexMutant) {
    int nNeedRealloc, nSearch, nIndex, nFinish, nSkip, nNormalHandle, nKillThem, nMaxWorkersAllowed, nIndexEquiv;
    T_stMutantExecution* pMutantExec;
    T_stMutantExecution* pMutantExecHeader;
    T_stEquivalentInfo* pEquivInfo;
    T_stEquivalentInfo* pEquivInfoHeader;

    nNeedRealloc = nFinish = nMaxWorkersAllowed = 0;

    if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Init\n");

    if (exeMap != NULL && exeWorker != NULL && nIndexTest != NULL && nIndexMutant != NULL) {
        nKillThem = -1;
        //First, check if a worker has associated work
        if (exeWorker->nMutantInit != -1) {
            if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Worker %d has associated the mutant %d\n", exeWorker->nWorker, exeWorker->nMutantInit);
            nNeedRealloc = 0;

            /* No entiendo porque cojones codificamos esto
            nIndexEquiv = exeWorker->nMutantInit;
            do {
                pMutantExec = exeMap->oMutantMap[nIndexEquiv];
                nIndexEquiv++;
            } while (nIndexEquiv <= exeMap->nMutants && (pMutantExec->nFinished != 0 || pMutantExec->nEquivalent == 1));
            
              
            exeWorker->nMutantInit = nIndexEquiv - 1;
              */
            //We need to check the execution of the mutant.
            if (exeWorker->nMutantInit <= exeMap->nMutants) {

                pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                pMutantExec->nIndexTest++;

                if (pMutantExec->nFinished == 0 && pMutantExec->nIndexTest < exeMap->nTests) {
                    //Set the index and the mutant
                    *nIndexTest = pMutantExec->nIndexTest;
                    *nIndexMutant = exeWorker->nMutantInit;
                } else {
                    //The mutant has finished!! Mark it and realloc!
                    if (pMutantExec->nFinished == 0) {
                        exeMap->nRemainingMutants--;
                        pMutantExec->nFinished = 1;
                    }
                    exeWorker->nMutantInit = -1;
                    exeWorker->nMutantEnd = -1;
                    nNeedRealloc = 1;
                    pMutantExec->nManaged--;
                }
            } else
                printf("WARNING!!!! updateCounters_Equiv is increasing an invalid mutant: %d\n", exeWorker->nMutantInit);
        } else
            nNeedRealloc = 1;

        //If its neccesary to change the mutant index.
        if (nNeedRealloc == 1) {
            if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Worker %d needs realloc\n", exeWorker->nWorker);
            if (exeMap->nCollapsed == 0) {
                nNormalHandle = checkEquivalence(exeMap, exeWorker, nIndexTest, nIndexMutant);

                //If the mutant is equivalent -> 
                if (nNormalHandle) {
                    //If the indexMutant has not reached the maximum, get param and increment.
                    if (exeMap->nIndexMutant <= exeMap->nMutants) {
                        if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Worker %d associated mutant %d\n", exeWorker->nWorker, exeMap->nIndexMutant);

                        //Set the index
                        *nIndexMutant = exeMap->nIndexMutant;
                        *nIndexTest = 0;
                        exeWorker->nMutantInit = exeMap->nIndexMutant;
                        exeWorker->nMutantEnd = exeMap->nIndexMutant;
                        //Update the list
                        pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                        pMutantExec->nManaged = 1;
                    } else {
                        if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - The execution has collapsed\n");
                        exeMap->nCollapsed = 1;
                        exeMap->nIndexMutant = -1;
                    }
                } else {
                    printf("updateCounters_Equiv - Internal handle!! -> nIndexMutant:%d nIndexTest%d\n",(*nIndexMutant), (*nIndexTest));
                    if ((*nIndexMutant) <= exeMap->nMutants)
                    {
                        pMutantExec = exeMap->oMutantMap[(*nIndexMutant)];
                        pMutantExec->nManaged = 1; //ojo que aqui cae un null como un yunke
                    }
                }
            }
            if (exeMap->nCollapsed == 1) {
                //Second round! Search non finished mutants!
                if (exeMap->nIndexMutant == -1)
                    exeMap->nIndexMutant = m_stEnvValues->nStartingMutant;

                nSearch = SEARCH_INITIAL;
                nIndex = exeMap->nIndexMutant;
                //Search the first mutant which is not finished
                while (nSearch > 0) {
                    if (nIndex <= exeMap->nMutants) {
                        pMutantExec = exeMap->oMutantMap[nIndex];

                        if (pMutantExec->nFinished == 0 && pMutantExec->nEquivalent == 0) {
                            //Check if the mutant isnt clusterheader                            
                            //In this case, skip them until all the running mutants are clustered
                            //printf("Test: %d\n",  pMutantExec->nIndexTest);
                            if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters - Mutant %d NOT finished\n", nIndex);
                            nSkip = nKillThem = -1;

                            if (pMutantExec->nIsClustered) {
                                if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters - Mutant %d is clustered\n", nIndex);
                                pEquivInfo = exeMap->oEquivalentInfo[nIndex];
                                if (pEquivInfo && !pEquivInfo->nIsClusterHead) {
                                    if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters - Mutant %d not is the cluster head: %d\n", nIndex, pEquivInfo->nMutantHeader);
                                    //Check if has a killer test yet
                                    if (pEquivInfo->nKillerTest != -1) {
                                        nKillThem = pEquivInfo->nKillerTest;
                                        nSearch = 0;
                                        //Uncluster!
                                        pMutantExec->nIsClustered = 0;
                                        printf("updateCounters_Equiv - Clone %d Found killer test: %d \n", nIndex, nKillThem);
                                    } else
                                    {   
                                        if(pEquivInfo->nMutantHeader<exeMap->nMutants)
                                        {
                                            pMutantExecHeader = exeMap->oMutantMap[pEquivInfo->nMutantHeader];
                                            
                                            //Head of the cluster has finished, now is the moment to choose
                                            //Hardcore kill, or alone in the dark
                                            if(pMutantExecHeader != NULL && pMutantExecHeader->nFinished)
                                            {
                                                printf("updateCounters_Equiv - The cluster head finished, but alive!! \n");
                                                
                                                //Here, you decide the hardcore, or the conventional way
                                                //pMutantExec->nFinished=1;
                                                //Conventional:
                                                nSearch = 0;
                                                pMutantExec->nIsClustered = 0;
                                                
                                            }
                                        }
                                        else if (nSearch <= SEARCH_INITIAL - (2 * SEARCH_SINGLEPHASE)) {
                                            printf("updateCounters_Equiv - ITs moment to kill them!! \n");
                                            //All remaining mutants are clustered
                                            //ITs moment to kill them!!
                                            pEquivInfoHeader = exeMap->oEquivalentInfo[pEquivInfo->nMutantHeader];
                                            //Getting the killer test!
                                            if (pEquivInfoHeader) {
                                                pEquivInfo->nKillerTest = pEquivInfoHeader->nKillerTest;
                                                if (MALONE_DEBUG_DIST_COMMON) printf("The killer test of M:%d  is %d\n", nIndex, pEquivInfo->nKillerTest);
                                            }
                                            //Try to check killer test or start by begining
                                            if (pEquivInfo->nKillerTest != -1 && pEquivInfo->nKillerTest < exeMap->nTests) {
                                                nKillThem = pEquivInfo->nKillerTest;
                                                nSearch = 0;
                                                //Uncluster!
                                                pMutantExec->nIsClustered = 0;
                                            } else {
                                                nSearch = 0;
                                                nKillThem = 0;

                                                //Uncluster!!
                                                pMutantExec->nIsClustered = 0;
                                            }
                                        }
                                        else
                                        {
                                            if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters - Mutant %d [Killer: -1]\n", nIndex);
                                        }
                                    }
                                }
                                else
                                {
                                    if (MALONE_DEBUG_DIST_COMMON) 
                                    {
                                        if(pEquivInfo->nIsClusterHead)
                                            printf("updateCounters - Skipping, Mutant %d is cluster head\n", nIndex);
                                        else
                                            printf("updateCounters - WARNING! Skipping, Mutant %d with null pInfo\n", nIndex);
                                    }
                                    nSkip = 1;
                                }
                            } else {
                                //We try to maximise the distribution ...
                                if (exeMap->nRemainingMutants != 0 && exeMap->nRemainingMutants <= m_nSize) {

                                    nMaxWorkersAllowed = ceil(m_nSize / exeMap->nRemainingMutants);
                                    if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters - Managing: %d | nMaxWorkersAllowed= {%d/%d = %d}\n", pMutantExec->nManaged, m_nSize, exeMap->nRemainingMutants, nMaxWorkersAllowed);

                                    nSearch = 0;
                                    nFinish = 0;
                                } else
                                    nSearch = 0;
                            }
                            if (nSkip == 0) {
                                //pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                                if (pMutantExec->nIndexTest < exeMap->nTests - 1)
                                    nSearch = 0;
                                else
                                    pMutantExec->nFinished = 1;
                            }
                        }
                        if (nSearch != 0)
                            nIndex++;
                    } else {
                        nIndex = m_stEnvValues->nStartingMutant;
                        nSearch = nSearch - SEARCH_SINGLEPHASE;
                    }

                    if (nSearch == 0) {
                        //Mutant index found!

                        //If there is a killing test, use it!!
                        if (nKillThem != -1) {
                            pMutantExec->nIndexTest = nKillThem;
                        }
                        exeWorker->nMutantInit = nIndex;
                        *nIndexTest = pMutantExec->nIndexTest;
                        *nIndexMutant = exeWorker->nMutantInit;

                        nFinish = 0;
                        //Increasing index to distribute the workers
                        //exeMap->nIndexMutant;
                    } else {                        
                        nFinish = 1;
                        printf("updateCounters_Equiv - Worker %d: All the mutants has finished? (Nsearch %d)\n", exeWorker->nWorker, nSearch);
                    }
                }//End while
                
            }//End collapsed
                    
        }//End Realloc
        if (m_stEnvValues->nSortTestSuite && nKillThem != -1) {
            *nIndexTest = m_oSortedIndexTest[*nIndexTest];
        }
        if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - Mutant: %d | Test: %d | Finish: %d\n", *nIndexMutant, *nIndexTest, nFinish);
    }

    if (MALONE_DEBUG_DIST_COMMON) printf("updateCounters_Equiv - End\n");

    return nFinish;
}

int updateCounters(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, int* nIndexTest, int* nIndexMutant) {
    int nNeedRealloc, nSearch, nIndex, nFinish, nMaxWorkersAllowed;
    nNeedRealloc = nFinish = 0;
    T_stMutantExecution* pMutantExec;

    if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - Init\n");

    if (exeMap != NULL && exeWorker != NULL && nIndexTest != NULL && nIndexMutant != NULL) {
        //First, check if a worker has associated work
        if (exeWorker->nMutantInit != -1) {
            if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - Worker %d has associated the mutant %d\n", exeWorker->nWorker, exeWorker->nMutantInit);
            nNeedRealloc = 0;

            //We need to check the execution of the mutant.
            if (exeWorker->nMutantInit <= exeMap->nMutants) {
                pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];

                pMutantExec->nIndexTest++;
                if (pMutantExec->nFinished == 0 && pMutantExec->nIndexTest < exeMap->nTests) {
                    //Set the index and the mutant
                    *nIndexTest = pMutantExec->nIndexTest;
                    *nIndexMutant = exeWorker->nMutantInit;
                } else {
                    //The mutant has finished!! Mark it and realloc!
                    if (pMutantExec->nFinished == 0) {
                        exeMap->nRemainingMutants--;
                        pMutantExec->nFinished = 1;
                    }
                    exeWorker->nMutantInit = -1;
                    exeWorker->nMutantEnd = -1;
                    nNeedRealloc = 1;
                    pMutantExec->nManaged--;
                }
            } else
                printf("WARNING!!!! updateCounters is increasing an invalid mutant\n");
        } else
            nNeedRealloc = 1;

        //If its neccesary to change the mutant index.
        if (nNeedRealloc == 1) {
            if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - Worker %d needs realloc\n", exeWorker->nWorker);
            if (exeMap->nCollapsed == 0) {
                if (exeMap->nIndexMutant == -1)
                    exeMap->nIndexMutant = m_stEnvValues->nStartingMutant;
                else
                    exeMap->nIndexMutant++;

                pMutantExec = exeMap->oMutantMap[exeMap->nIndexMutant];

                //If the indexMutant has not reached the maximum, get param and increment.
                if (exeMap->nIndexMutant <= exeMap->nMutants) {
                    if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - Worker %d associated mutant %d\n", exeWorker->nWorker, exeMap->nIndexMutant);

                    //Set the index
                    *nIndexMutant = exeMap->nIndexMutant;
                    *nIndexTest = 0;
                    exeWorker->nMutantInit = exeMap->nIndexMutant;
                    exeWorker->nMutantEnd = exeMap->nIndexMutant;

                    //Update the list
                    pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                    if (pMutantExec)
                        pMutantExec->nManaged++;
                    if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - associated!");
                } else {
                    if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - The execution has collapsed %d vs %d\n", exeMap->nIndexMutant, exeMap->nMutants);
                    exeMap->nCollapsed = 1;
                    exeMap->nIndexMutant = -1;
                }
            }
            if (exeMap->nCollapsed == 1) {
                //Second round! Search non finished mutants!
                if (exeMap->nIndexMutant == -1)
                    exeMap->nIndexMutant = m_stEnvValues->nStartingMutant;

                nSearch = SEARCH_INITIAL;
                nIndex = exeMap->nIndexMutant;
                //Search the first mutant which is not finished
                while (nSearch > 0) {

                    if (nIndex <= exeMap->nMutants) {
                        pMutantExec = exeMap->oMutantMap[nIndex];

                        if (pMutantExec && pMutantExec->nFinished == 0) {

                            if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - Mutant %d NOT finished\n", nIndex);

                            //pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
                            if (pMutantExec->nIndexTest < exeMap->nTests - 1) {

                                //We try to maximise the distribution ...
                                if (exeMap->nRemainingMutants != 0 && exeMap->nRemainingMutants <= m_nSize) {

                                    nMaxWorkersAllowed = ceil(m_nSize / exeMap->nRemainingMutants);
                                    if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - Managing: %d | nMaxWorkersAllowed= {%d/%d = %d}\n", pMutantExec->nManaged, m_nSize, exeMap->nRemainingMutants, nMaxWorkersAllowed);
                                } else
                                    nSearch = 0;

                                if (nMaxWorkersAllowed > pMutantExec->nManaged)
                                    nSearch = 0;
                            } else {
                                if (MALONE_DEBUG_DIST_UPDATE)printf("updateCounters - The mutant %d has been marked as finished, indexTest: %d\n", nIndex, pMutantExec->nIndexTest);

                                pMutantExec->nFinished = 1;
                            }
                        } else {
                            if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - Mutant %d finished\n", nIndex);

                        }
                        if (nSearch != 0)
                            nIndex++;
                    } else {
                        nIndex = m_stEnvValues->nStartingMutant;
                        nSearch = nSearch - 2;
                    }

                    if (nSearch == 0) {
                        //Mutant index found!
                        pMutantExec->nIndexTest++;
                        pMutantExec->nManaged++;
                        exeWorker->nMutantInit = nIndex;
                        exeWorker->nMutantEnd = nIndex;
                        *nIndexTest = pMutantExec->nIndexTest;
                        *nIndexMutant = exeWorker->nMutantInit;

                        //Increasing index to distribute the workers
                        nFinish = 0;
                    } else {
                        if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - Worker %d: All the mutants have finished? (Search: %d)\n", exeWorker->nWorker, nSearch);
                        nFinish = 1;
                    }
                }
            }
        }
        if (m_stEnvValues->nSortTestSuite) {
            *nIndexTest = m_oSortedIndexTest[*nIndexTest];
        }

        if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - Worker: %d | Mutant: %d | Test: %d | Finished: %d\n", exeWorker->nWorker, *nIndexMutant, *nIndexTest, nFinish);

    }

    if (MALONE_DEBUG_DIST_UPDATE) printf("updateCounters - End\n");

    return nFinish;
}

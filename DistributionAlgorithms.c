#include "DistributionAlgorithms.h"
//TODO: Renombrar a distribution algorithms common.

int hasOriginalProgramRemainingWork(int nIndexTests) {
    int nRet;

    nRet = 0;
    if (MALONE_DEBUG_DIST_COMMON) printf("hasOriginalProgramRemainingWork - Init\n");

    if (MALONE_DEBUG_DIST_COMMON) printf("hasOriginalProgramRemainingWork - IndexTests: %d\n", nIndexTests);

    if (nIndexTests < m_stEnvValues->nTotalTests)
        nRet = 1;

    if (nRet == 1) {
        if (MALONE_DEBUG_DIST_COMMON) printf("hasOriginalProgramRemainingWork - The process has remaining work\n");
    } else {
        if (MALONE_DEBUG_DIST_COMMON) printf("hasOriginalProgramRemainingWork - The process has finished\n");
    }

    if (MALONE_DEBUG_DIST_COMMON) printf("hasOriginalProgramRemainingWork - End\n");

    return nRet;
}

int hasRemainingWork(int nIndexMutant, int nIndexTests) {
    int nRet;

    nRet = 0;
    if (MALONE_DEBUG_DIST_COMMON) printf("hasRemainingWork - Init\n");

    if (MALONE_DEBUG_DIST_COMMON) printf("hasRemainingWork - IndexMutant: %d - IndexTests: %d\n", nIndexMutant, nIndexTests);

    if (nIndexMutant < m_stEnvValues->nTotalMutants ||
            (nIndexMutant == m_stEnvValues->nTotalMutants && nIndexTests < m_stEnvValues->nTotalTests))
        nRet = 1;
    if (MALONE_DEBUG_DIST_COMMON) printf("hasRemainingWork - End\n");
    return nRet;
}

//TODO: Esto quitarlo de aqui, no tiene sentido, meter en equivalence checking?
void updateKillMutant(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker) {
    T_stMutantExecution* pMutantExec;
    T_stEquivalentInfo* pEquivInfo;
    T_stEquivalentInfo* pEquivInfoClone;
    int i, nClone;
    if (MALONE_DEBUG_DIST_COMMON) printf("updateKillMutant - Init\n");

    if (exeMap != NULL && exeWorker != NULL && exeWorker->nMutantInit != -1) {
        exeMap->nRemainingMutants--;
        pMutantExec = exeMap->oMutantMap[exeWorker->nMutantInit];
        pEquivInfo = exeMap->oEquivalentInfo[exeWorker->nMutantInit];

        if (pMutantExec) {
            if (pMutantExec->nFinished == 1)
                printf("----------------------------------YET KILLED\n");
            pMutantExec->nFinished = 1;


            if (pEquivInfo) {
                pEquivInfo->nKillerTest = exeWorker->nTestInit;
                if (pMutantExec->nIsClustered && pEquivInfo->nIsClusterHead) {
                    printf("CLUSTER HEAD HAS DIED!!! Mut: %d Test: %d\n", exeWorker->nMutantInit, pEquivInfo->nKillerTest);

                    //Notifying the clone mutants ...
                    printf("CLUSTER HEAD, notifying %d \n", pEquivInfo->nClones);

                    for (i = 0; i < pEquivInfo->nClones; i++) {
                        nClone = pEquivInfo->pClones[i];
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
            }
            if (MALONE_DEBUG_DIST_COMMON) printf("updateKillMutant - Worker: %d | Mutant: %d | Test: %d \n", exeWorker->nWorker, exeWorker->nMutantInit, exeWorker->nTestInit);
            exeWorker->nMutantInit = -1;
        }
    } else
        printf("updateKillMutant - PROBLEMS!!\n");

    if (MALONE_DEBUG_DIST_COMMON) printf("updateKillMutant - End\n");
}
//TODO: Esto quitarlo de aqui, no tiene sentido, meter en equivalence checking?
int checkEquivalence(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, int* nIndexTest, int* nIndexMutant) {
    int nNormalHandle, nEquivalent, nClusteredNotHeaderRunning, nManaged, nFinished;
    T_stMutantExecution* pMutantExec, *pMutantHeaderExec;
    T_stEquivalentInfo* pEquivInfo;

    if (MALONE_DEBUG_DIST_COMMON) printf("checkEquivalence - Init\n");
    nNormalHandle = 0;
    if (exeMap != NULL && exeWorker != NULL && nIndexTest != NULL && nIndexMutant != NULL) {
        if (MALONE_DEBUG_DIST_COMMON) printf("checkEquivalence - 1\n");
        do {
            nEquivalent = 0;
            nClusteredNotHeaderRunning = 0;
            nManaged = nFinished = 0;

            nNormalHandle = 1;
            if (exeMap->nIndexMutant == -1)
                exeMap->nIndexMutant = m_stEnvValues->nStartingMutant;
            else
                exeMap->nIndexMutant++;
            if (exeMap->nIndexMutant <= exeMap->nMutants) {
                pMutantExec = exeMap->oMutantMap[exeMap->nIndexMutant];
                pEquivInfo = exeMap->oEquivalentInfo[exeMap->nIndexMutant];

                if (pMutantExec && pEquivInfo) {
                    //if the mutant is equivalent,
                    nEquivalent = pMutantExec->nEquivalent;
                    nFinished = pMutantExec->nFinished;
                    nManaged = pMutantExec->nManaged;
                    //IF the mutant is clustered, is not the header, and the header is running
                    if (!nFinished && !nManaged && !nEquivalent && pMutantExec->nIsClustered && !pEquivInfo->nIsClusterHead) {
                        //Check if the clustered mutant not has a killer test
                        if (pEquivInfo->nKillerTest == -1) {
                            pMutantHeaderExec = exeMap->oMutantMap[pEquivInfo->nMutantHeader];
                            if (pMutantHeaderExec) {
                                if (pMutantHeaderExec->nFinished == 1) {
                                    //killing test  does not exists
                                    //Normal handle

                                    nNormalHandle = 1;
                                    nFinished = 1;
                                    printf("checkEquivalence - The header mutant has died ... Head: %d - Clone: %d - Kill Test: %d\n", pEquivInfo->nMutantHeader, exeMap->nIndexMutant, pEquivInfo->nKillerTest);
                                    //pMutantExec->nFinished = 1;
                                    //Forcing to check the parent.
                                    T_stEquivalentInfo* pEquivInfoParent;
                                    if (pEquivInfo->nMutantHeader > 0) {
                                        pEquivInfoParent = exeMap->oEquivalentInfo[pEquivInfo->nMutantHeader];
                                        if (pEquivInfoParent) {
                                            if (pEquivInfoParent->nKillerTest >= 0) {
                                                nFinished = 0;
                                                nNormalHandle = 0;
                                                *nIndexTest = pEquivInfoParent->nKillerTest;
                                                *nIndexMutant = exeMap->nIndexMutant;
                                                printf("The clone %d killer test is: %d vs %d ... Killing it!!\n", exeMap->nIndexMutant, pEquivInfoParent->nKillerTest, *nIndexTest);
                                                pMutantExec->nManaged = 1;
                                                //pMutantExec->nFinished = 1;
                                            } else {
                                                if (MALONE_DEBUG_DIST_COMMON) printf("The clone %d doesnt have a killer test: -1\n", exeMap->nIndexMutant);
                                            }
                                        }
                                    }
                                } else {
                                    //We skip this mutant until the mutant header finishes
                                    nClusteredNotHeaderRunning = 1;
                                }
                            }
                        } else {
                            printf("checkEquivalence - Mutant %d is part of cluster, killing test: %d\n", exeMap->nIndexMutant, pEquivInfo->nKillerTest);
                            //We must select the killing test and the actual mutant.
                            nNormalHandle = 0;
                            *nIndexTest = pEquivInfo->nKillerTest;
                            *nIndexMutant = exeMap->nIndexMutant;
                            pMutantExec->nManaged = 1;
                            printf("checkEquivalence - Transformation mutant %d is part of cluster, killing test: %d\n", (*nIndexMutant), (*nIndexTest));
                        }
                    } else {
                        //IN this case, normal handle.
                        if (nManaged)
                            if (MALONE_DEBUG_DIST_COMMON) printf("checkEquivalence - Yet managed!\n");
                        nNormalHandle = 1;
                        if (pEquivInfo->nIsClusterHead)
                            if (MALONE_DEBUG_DIST_COMMON) printf("checkEquivalence - Cluster head! Lets continue!\n");

                        if (nEquivalent)
                            if (MALONE_DEBUG_DIST_COMMON) printf("checkEquivalence - Mutant %d is equivalent! Skipping...\n", exeMap->nIndexMutant);
                    }

                } else
                    printf("checkEquivalence - WARNING!! Null pointers! Num mutant: %d\n", exeMap->nIndexMutant);
            }
        } while (nEquivalent || nClusteredNotHeaderRunning || nManaged || nFinished);
    } else
        printf("checkEquivalence - WARNING 1.0!!\n");
    if (MALONE_DEBUG_DIST_COMMON) printf("checkEquivalence - End: %d\n", nNormalHandle);

    return nNormalHandle;
}


void incCounters(int* nIndexTest, int* nIndexMutant) {
    *nIndexTest = *nIndexTest + 1;

    if (MALONE_DEBUG_DIST_COMMON) printf("incCounters - >IndexMutant:%d - IndexTest: %d\n", *nIndexMutant, *nIndexTest);
    if (*nIndexTest >= m_stEnvValues->nTotalTests) {
        *nIndexTest = 0;
        *nIndexMutant = *nIndexMutant + 1;
    }
    if (MALONE_DEBUG_DIST_COMMON) printf("incCounters - IndexMutant:%d - IndexTest: %d\n", *nIndexMutant, *nIndexTest);
}

int isEnabledDistWorkersLog() {
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugDistWorkers : 0;
}

int isEnabledDistMasterLog() {
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugDistMaster : 0;
}

int isEnabledDistCommonLog() {
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugDistCommon : 0;
}

int isEnabledDistUpdateLog() {
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugDistUpdates : 0;
}
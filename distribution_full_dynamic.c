/* 
 * File:   DistributionAlgorithms.h
 * Author: Pablo C. Cañizares
 *
 * Created on August 15, 2018, 12:54 PM
 */
#include "DistributionAlgorithms.h"

int distribution_full_dynamic(T_eExecutionMode eMethod) {
    //Divide the number of mutants among the available workers!
    int nMutants, nTests, nSent, nIndex, nIndexMutant, nIndexTest, nAlive, nWorkerSource, nFinish, nRemainBlocks, nCounterAux;
    int nTestRecalc, nMutantRecalc;
    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;
    T_stTestInfo* pTest;

    if (MALONE_DEBUG_DIST_COMMON) printf("distribution_full_dynamic - Init\n");
    if (m_nRank == MALONE_MASTER) {
        if (MALONE_DEBUG_DIST_MASTER) printf("distribution_full_dynamic - Master mode!\n");
        //Initialize
        nMutants = m_stEnvValues->nTotalMutants;
        nTests = m_stEnvValues->nTotalTests;
        m_oTestExecMap.nMutants = nMutants;
        m_oTestExecMap.nTests = nTests;
        nIndexMutant = 1;
        nIndexTest = nCounterAux = 0;
        nIndex = nSent = 1;
        nWorkerSource = -1;


        if (m_stEnvValues->nStartingMutant != 0) {
            nIndexMutant = m_stEnvValues->nStartingMutant;
            nCounterAux = m_stEnvValues->nStartingMutant;
        }


        if (MALONE_DEBUG_DIST_MASTER) printf("distribution_full_dynamic - Starting at mutant: %d\n", nIndex);
        if (MALONE_DEBUG_DIST_MASTER) printf("distribution_full_dynamic - Distributing %d mutants with %d tests,  between %d workers\n", nMutants, nTests, m_nSize - 1);

        //Fill the env values to send it to workers
        while (nIndex < m_nSize /*&& nIndex <= nMutants*/) {
            //Initialize            
            exeVector[nIndex].nExecutionMode = (int) eMethod;
            if (nIndex <= nMutants) {
                if (m_stEnvValues->nSortTestSuite) {
                    nTestRecalc = m_oSortedIndexTest[nIndexTest];
                } else {
                    nTestRecalc = nIndexTest;
                }

                exeVector[nIndex].nTestInit = nTestRecalc;
                exeVector[nIndex].nTestEnd = nTestRecalc;
                exeVector[nIndex].nMutantInit = nIndexMutant;
                exeVector[nIndex].nMutantEnd = nIndexMutant;
                nSent++;
            } else {
                exeVector[nIndex].nTestInit = -1;
                exeVector[nIndex].nTestEnd = -1;
                exeVector[nIndex].nMutantInit = -1;
                exeVector[nIndex].nMutantEnd = -1;
            }
            sendDeployMode(&exeVector[nIndex], nIndex);

            m_oTestExecMap.oMap[nIndexMutant][nTestRecalc] = nIndex;
            //Inc counters
            nIndex++;
            if (nIndex < m_nSize)
                incCounters(&nIndexTest, &nIndexMutant);
        }
        nIndex = nSent;
        nRemainBlocks = nIndex - 1;
        if (MALONE_DEBUG_DIST_MASTER) printf("distribution_full_dynamic - Executing Mutant %d of %d | elapsed: %ld\n", nCounterAux, nMutants, getTick() - m_lMutantInitTick);
        //Wait for receiving values from all workers and filling final result
        do {
            nAlive = receiveSingleTestAndCheck(&exeVector, &nWorkerSource);
            nRemainBlocks--;

            if (hasRemainingWork(nIndexMutant, nIndexTest)) {
                if (nAlive == 1) {
                    //Increment mutant and test counters
                    incCounters(&nIndexTest, &nIndexMutant);
                } else {
                    if (MALONE_DEBUG_DIST_MASTER) printf("distribution_full_dynamic - Mutant %d has dead in test %d | [worker -  %d]\n", nIndexMutant, nIndexTest, nWorkerSource);
                    //If the mutant that failed test, is currently tested -> KO!
                    if (exeVector[nWorkerSource].nMutantInit == nIndexMutant) {
                        nIndexTest = 0;
                        nIndexMutant++;
                    }
                    if (MALONE_DEBUG_DIST_MASTER)
                        printf("Killed mutant: %d | test: %d | Source: %d\n", nIndexMutant, nIndexTest, nWorkerSource);
                }
                if (nIndexMutant != nCounterAux) {
                    nCounterAux = nIndexMutant;
                    long int lElapsed = (long int) (getTick() - m_lTotalTime);
                    if (MALONE_DEBUG_DIST_MASTER) printf("distribution_full_dynamic - Executing Mutant %d of %d | Elapsed: %ld\n", nCounterAux, nMutants, lElapsed);
                }

            }

            //If there is remaining mutants to distribute, send it!
            if (nIndexMutant <= nMutants) {
                if (m_stEnvValues->nSortTestSuite) {
                    nTestRecalc = m_oSortedIndexTest[nIndexTest];
                } else {
                    nTestRecalc = nIndexTest;
                }

                exeVector[nWorkerSource].nMutantInit = exeVector[nWorkerSource].nMutantEnd = nIndexMutant;
                exeVector[nWorkerSource].nTestInit = nTestRecalc;
                exeVector[nWorkerSource].nTestEnd = nTestRecalc;
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);
                nRemainBlocks++;
            } else {
                //Worker has finished its work.
                if (MALONE_DEBUG_DIST_MASTER) printf("distribution_full_dynamic - Worker %d has finished!\n", nWorkerSource);
                if (MALONE_DEBUG_DIST_MASTER) printf("distribution_full_dynamic - Remaining blocks: %d\n", nRemainBlocks);
                exeVector[nWorkerSource].nMutantInit = -1;
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);
            }
        } while (hasRemainingWork(nIndexMutant, nIndexTest) || nRemainBlocks);
        if (MALONE_DEBUG_DIST_MASTER) printf("distribution_full_dynamic - Master finished!!\n");
    } else {
        nFinish = 0;
        if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_full_dynamic - Workers mode!\n");

        do {
            //receive the deployment method
            pExeRetMode = receiveDeployMode();

            if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_full_dynamic - Received deploy mode!\n");

            printDeployStruct(pExeRetMode);
            nFinish = (pExeRetMode == NULL || pExeRetMode->nMutantInit == -1);
            //execute
            if (!nFinish) {
                if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_full_dynamic - Executing mutants\n");
                //executing mutants using the scheme received
                malone_execute_mutants_by_scheme(pExeRetMode);

                //send results to master!
                nIndexTest = pExeRetMode->nTestInit;
                nIndexMutant = pExeRetMode->nMutantInit;
                if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_full_dynamic - Sending results to master [M: %d - T: %d]\n", nIndexMutant, nIndexTest);
                pTest = m_oMutantList.array[nIndexMutant]->oTestList.tests[nIndexTest];
                if (MALONE_DEBUG_DIST_WORKERS) printTest(pTest);
            } else {
                if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_full_dynamic - Worker <%d> finished!\n", m_nRank);
            }
        } while (!nFinish);
    }

    if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_full_dynamic - End\n");
}
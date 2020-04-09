/* 
 * File:   DistributionAlgorithms.h
 * Author: Pablo C. Cañizares
 *
 * Created on August 15, 2018, 12:54 PM
 */
#include "DistributionAlgorithms.h"
int distribution_dynamic_mutants(T_eExecutionMode eMethod) {
    //Divide the number of mutants among the available workers!
    int nMutants, nTests, nIndex, nRet, nNumReceives, nWorkerSource, nFinish, nSent;
    int oIndex[MAX_WORKERS];
    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;

    if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_dynamic_mutants - Init\n");

    if (m_nRank == MALONE_MASTER) {
        if (MALONE_DEBUG_DIST_MASTER) printf("distribution_dynamic_mutants - Master!! mode!\n");
        //Initialize
        nMutants = m_stEnvValues->nTotalMutants;
        nTests = m_stEnvValues->nTotalTests;
        nIndex = nSent = 1;
        nNumReceives = 0;
        nWorkerSource = -1;

        if (MALONE_DEBUG_DIST_MASTER) printf("distribution_dynamic_mutants - Distributing %d mutants with %d tests,  between %d workers\n", nMutants, nTests, m_nSize - 1);

        //Fill the env values to send it to workers
        while (nIndex < m_nSize/* && nIndex <= nMutants*/) {
            //Initialize
            exeVector[nIndex].nExecutionMode = (int) eMethod;

            if (nIndex <= nMutants) {
                exeVector[nIndex].nTestInit = 0;
                exeVector[nIndex].nTestEnd = m_stEnvValues->nTotalTests - 1;

                exeVector[nIndex].nMutantInit = nIndex;
                exeVector[nIndex].nMutantEnd = nIndex;
                nSent++;
            }// It is used to finish workers, when the number of mutantes is lesser than the workers.
            else {
                exeVector[nIndex].nTestInit = -1;
                exeVector[nIndex].nTestEnd = -1;

                exeVector[nIndex].nMutantInit = -1;
                exeVector[nIndex].nMutantEnd = -1;
            }
            sendDeployMode(&exeVector[nIndex], nIndex);
            nIndex++;
        }
        nIndex = nSent;
        //Wait for receiving values from all workers and filling final result
        do {
            nWorkerSource = receiveMutants(&exeVector);
            nNumReceives++;
            if (MALONE_DEBUG_DIST_MASTER) printf("distribution_dynamic_mutants - Received mutants results, total received: %d\n", nNumReceives);
            //If there is remaining mutants to send, send it!
            if (nIndex <= nMutants) {
                exeVector[nWorkerSource].nMutantInit = nIndex;
                exeVector[nWorkerSource].nMutantEnd = nIndex;
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);
                nIndex++;
            } else {
                //Mutant has finished its work.
                if (MALONE_DEBUG_DIST_MASTER) printf("distribution_dynamic_mutants - Worker %d has finished!\n", nWorkerSource);
                exeVector[nWorkerSource].nMutantInit = -1;
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);
                if (nNumReceives <= nMutants) {
                    if (MALONE_DEBUG_DIST_MASTER) printf("distribution_dynamic_mutants - Mutants remain: %d\n", nMutants - nNumReceives);

                }
            }
        } while (nNumReceives < nMutants); // Substracting 1 by the master
    } else {
        nFinish = 0;
        if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_dynamic_mutants - Workers mode!\n");


        //if(DEBUG)
        do {
            //receive the deployment method
            pExeRetMode = receiveDeployMode();

            if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_dynamic_mutants - Received deploy mode!\n");

            if (MALONE_DEBUG_DIST_WORKERS) printDeployStruct(pExeRetMode);

            nFinish = (pExeRetMode == NULL || pExeRetMode->nMutantInit == -1);
            //execute
            if (!nFinish) {
                if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_dynamic_mutants - Executing mutants\n");
                //executing mutants using the scheme received
                malone_execute_mutants_by_scheme(pExeRetMode);

                if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_dynamic_mutants - Sending results to master\n");

                //send results to master!
                sendMutants(pExeRetMode, MALONE_MASTER);

                if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_dynamic_mutants - Worker <%d> finished!\n", m_nRank);
            } else {
                if (pExeRetMode->nMutantInit == -1)
                    if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_dynamic_mutants - Worker %d finished!\n", m_nRank);
                    else

                        if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_dynamic_mutants - WARNING! Deploy mode is empty!!!\n");
            }

        } while (!nFinish);
    }

    if (MALONE_DEBUG_DIST_COMMON) printf("distribution_dynamic_mutants - Process <%d> Ends\n", m_nRank);
}

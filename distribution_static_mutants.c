/* 
 * File:   DistributionAlgorithms.h
 * Author: Pablo C. Cañizares
 *
 * Created on August 15, 2018, 12:54 PM
 */
#include "DistributionAlgorithms.h"

int distribution_static_mutants(T_eExecutionMode eMethod) {
    //Divide the number of mutants among the available workers!
    int nMutants, nTests, nTotalRemaining, nRemWork, nIndex, i, nRet, nInit, nEnd, nNumReceives;
    int oIndex[MAX_WORKERS];
    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;
    printf("distribution_static_mutants - Init\n");

    if (m_nRank == MALONE_MASTER) {
        printf("distribution_static_mutants - Master!! mode!\n");
        //Initialize
        //memset(&exeVector, 0, sizeof(T_stExecutionStructure)*MAX_WORKERS);
        nMutants = m_stEnvValues->nTotalMutants;
        nTests = m_stEnvValues->nTotalTests;
        nIndex = 1;
        nRemWork = (m_nSize - 1);
        nTotalRemaining = nMutants;
        nEnd = nNumReceives = 0;
        nInit = 1;

        printf("distribution_static_mutants - Distributing %d mutants with %d tests, among %d workers\n", nMutants, nTests, m_nSize - 1);

        //Distribute the elements 
        while (nTotalRemaining > 0 && nRemWork) {
            oIndex[nIndex] = ceil(nTotalRemaining / nRemWork);
            nTotalRemaining -= ceil(nTotalRemaining / nRemWork);
            nIndex++;
            nRemWork--;
        }
        printDeployArray(oIndex, m_nSize);
        //exeMode.nExecutionMode = (int) eMethod;

        //Fill the env values to send it to workers
        for (i = 1; i < m_nSize; i++) {
            nEnd += oIndex[i];
            exeVector[i].nExecutionMode = (int) eMethod;
            exeVector[i].nMutantInit = nInit;
            exeVector[i].nMutantEnd = nEnd;
            exeVector[i].nTestInit = 0;
            exeVector[i].nTestEnd = m_stEnvValues->nTotalTests - 1;

            sendDeployMode((T_stExecutionStructure*) &exeVector[i], i);
            nInit = nEnd;
        }
        //Wait for receiving values from all workers and filling final result
        do {
            receiveMutants(exeVector);
            nNumReceives++;
        } while (nNumReceives < (m_nSize - 1)); // Decreasing in 1, due to the master
    } else {
        if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_static_mutants - Workers mode!\n");

        //receive the deployment method
        if(COMPATIBLE_MODE)
            receiveDeployModeRefP(&pExeRetMode);
        else
            pExeRetMode = receiveDeployMode();

        if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_static_mutants - Received deploy mode!\n");
        //if(DEBUG)
        printDeployStruct(pExeRetMode);

        //execute
        if (pExeRetMode) {
            if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_static_mutants - Executing mutants\n");
            //executing mutants using the scheme received
            malone_execute_mutants_by_scheme(pExeRetMode);

            if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_static_mutants - Sending results to master\n");

            //send results to master!
            sendMutants(pExeRetMode, MALONE_MASTER);

            if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_static_mutants - Worker <%d> finished!\n", m_nRank);
        } else
            printf("distribution_static_mutants - WARNING! Deploy mode is empty!!!\n");
    }

    if (MALONE_DEBUG_DIST_WORKERS) printf("distribution_static_mutants - End\n");
}
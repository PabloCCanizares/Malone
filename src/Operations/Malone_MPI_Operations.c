/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_MPI_Operations.c
 *     This file contains the main MPI operations of MALONE.
 * @par Purpose:
 *    Provide the main communication features of MALONE.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     4 Dec 2015 - 11 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#include "Malone_MPI_Operations.h"

//TODO: Dividir MPI operations en minimo dos: MALONE_MPI_Ops_Tests && MALONE_MPI_Ops_Mutants

#ifdef DISABLELOGS
#define DEBUG_MPI_OPS 0
#else
#define DEBUG_MPI_OPS isEnabledMpiOperations()
#endif

/**
 * Initialise the deploy datatype
 */
void init_Datatype_deploy() {
    //Commit the struct
    MPI_Type_contiguous(NUM_DEPLOY_PARAMS, MPI_INT, &m_DeployType);    
    MPI_Type_commit(&m_DeployType);
}

/**
 * Initialise the datatype that corresponds with a test case
 */
void init_Datatype_test() {
    int base, i;
    int blocklen[5] = {MAX_RESULT_SIZE, 1, 1, 1, 1};
    MPI_Datatype type[5] = {MPI_CHAR, MPI_DOUBLE, MPI_INT, MPI_INT, MPI_INT};
    MPI_Aint disp[5];
    
    //Fix the deprecated function    
    if(COMPATIBLE_MODE)
    {
        MPI_Aint baseAddr, addr1, addr2, addr3, addr4;
        MPI_Get_address(&m_oRedTestList[0].res, &baseAddr); 
        MPI_Get_address(&m_oRedTestList[0].dTime, &addr1); 
        MPI_Get_address(&m_oRedTestList[0].nKill, &addr2); 
        MPI_Get_address(&m_oRedTestList[0].nTest, &addr3); 
        MPI_Get_address(&m_oRedTestList[0].nMutant, &addr4); 

        disp[0] = 0;
        disp[1] = addr1 - baseAddr;
        disp[2] = addr2 - baseAddr;
        disp[3] = addr3 - baseAddr;
        disp[4] = addr4 - baseAddr;
    }
    else
    {
        MPI_Address(&m_oRedTestList[0], disp);
        MPI_Address(&m_oRedTestList[0].dTime, disp + 1);
        MPI_Address(&m_oRedTestList[0].nKill, disp + 2);
        MPI_Address(&m_oRedTestList[0].nTest, disp + 3);
        MPI_Address(&m_oRedTestList[0].nMutant, disp + 4);

        base = disp[0];
        for (i = 0; i < 5; i++)
            disp[i] -= base;
    }
       
    MPI_Type_struct(5, blocklen, disp, type, &m_TestType);
    MPI_Type_commit(&m_TestType);
}

/**
 * Initialises the mutant datatype
 */
void init_Datatype_mutant() {
    int base, i;
    int blocklen[7] = {1, 1, 1, 1, MAX_TESTS, MAX_TESTS, MAX_TESTS};

    MPI_Datatype type[7] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_DOUBLE, MPI_INT, MPI_INT};
    MPI_Aint disp[7];

    if(COMPATIBLE_MODE)
    {
        MPI_Aint baseAddr, addr1, addr2, addr3, addr4, addr5, addr6;
        MPI_Get_address(&m_oRedMutantList[0].nNumber, &baseAddr); 
        MPI_Get_address(&m_oRedMutantList[0].nState, &addr1); 
        MPI_Get_address(&m_oRedMutantList[0].nTestKiller, &addr2); 
        MPI_Get_address(&m_oRedMutantList[0].nTests, &addr3); 
        MPI_Get_address(&m_oRedMutantList[0].oTime, &addr4); 
        MPI_Get_address(&m_oRedMutantList[0].oKill, &addr5);
        MPI_Get_address(&m_oRedMutantList[0].oTest, &addr6);
        
        disp[0] = 0;
        disp[1] = addr1 - baseAddr;
        disp[2] = addr2 - baseAddr;
        disp[3] = addr3 - baseAddr;
        disp[4] = addr4 - baseAddr;
        disp[5] = addr5 - baseAddr;
        disp[6] = addr6 - baseAddr;
    }
    else
    {
        //This method of building a datatype is deprecated and is not working in some computers.
        MPI_Address(m_oRedMutantList, disp);
        MPI_Address(&m_oRedMutantList[0].nState, disp + 1);
        MPI_Address(&m_oRedMutantList[0].nTestKiller, disp + 2);
        MPI_Address(&m_oRedMutantList[0].nTests, disp + 3);
        MPI_Address(&m_oRedMutantList[0].oTime, disp + 4);
        MPI_Address(&m_oRedMutantList[0].oKill, disp + 5);
        MPI_Address(&m_oRedMutantList[0].oTest, disp + 6);

        base = disp[0];
        for (i = 0; i < 7; i++)
            disp[i] -= base;
    }
    MPI_Type_struct(7, blocklen, disp, type, &m_MutantType);
    MPI_Type_commit(&m_MutantType);
}

/**
 *  Initialize the MPICH library
 */
void initializeMPI() {
    MPI_Init(0, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &m_nRank);
    MPI_Comm_size(MPI_COMM_WORLD, &m_nSize);
    initializeMPI_Datatype();
    printf("<%d> Initialized process %d of %d\n", m_nRank, m_nRank, m_nSize);
}

/**
 * Initialise the MPI Datatypes that are used in Malone
 */
void initializeMPI_Datatype() {
    init_Datatype_deploy();
    init_Datatype_test();
    init_Datatype_mutant();

    if (DEBUG_MPI_OPS) printf("initializeMPI_Datatype - Initialized\n");
}

/**
 * Sends to an specific worker process a deploy structure, which indicates the distribution algorithm to use and the portion of execution grain.
 * @param exeMode Deploy structure
 * @param nDest Identifier of the worker process, which must receive the deploy structure.
 */
void sendDeployMode(T_stExecutionStructure* exeMode, int nDest) {
    int exeParams[NUM_DEPLOY_PARAMS];

    if (DEBUG_MPI_OPS) printf("<%d>sendDeployMode - Init!\n", m_nRank);

    if (DEBUG_MPI_OPS) printf("<%d>sendDeployMode - Initialiting values!\n", m_nRank);

    //Copy the values from the struct
    exeParams[0] = exeMode->nExecutionMode;
    exeParams[1] = exeMode->nMutantInit;
    exeParams[2] = exeMode->nMutantEnd;
    exeParams[3] = exeMode->nTestInit;
    exeParams[4] = exeMode->nTestEnd;

    //Tick the init 
    exeMode->dInitTick = getTick();

    printDeployStruct(exeMode);

    if (DEBUG_MPI_OPS) printf("sendDeployMode - Sending data to %d!\n", nDest);

    MPI_Send(&exeParams, 1, m_DeployType, nDest, 0, MPI_COMM_WORLD);

    if (DEBUG_MPI_OPS) printf("sendDeployMode - Done!\n");
}

/**
 * Receives a deploy structure.
 * @return The deploy structure.
 */
T_stExecutionStructure* receiveDeployMode() {
    MPI_Status status;
    T_stExecutionStructure* exeMode;
    int exeParams[NUM_DEPLOY_PARAMS];

    if (DEBUG_MPI_OPS) printf("<%d> receiveDeployMode - Receiving\n", m_nRank);

    MPI_Recv(&exeParams, 1, m_DeployType, 0, 0, MPI_COMM_WORLD, &status);

    //Copy parameters    
    exeMode = (T_stExecutionStructure*) malloc(sizeof (T_stExecutionStructure));
    exeMode->nExecutionMode = exeParams[0];
    exeMode->nMutantInit = exeParams[1];
    exeMode->nMutantEnd = exeParams[2];
    exeMode->nTestInit = exeParams[3];
    exeMode->nTestEnd = exeParams[4];

    if (DEBUG_MPI_OPS) printf("<%d> receiveDeployMode - End\n", m_nRank);
    
    return exeMode;
}

void receiveDeployModeRefP(T_stExecutionStructure** exeMode) {
    MPI_Status status;
    int exeParams[NUM_DEPLOY_PARAMS];

    if (DEBUG_MPI_OPS) printf("<%d> receiveDeployMode - Receiving\n", m_nRank);

    MPI_Recv(&exeParams, 1, m_DeployType, 0, 0, MPI_COMM_WORLD, &status);

    //Copy parameters    
    *exeMode = (T_stExecutionStructure*) malloc(sizeof (T_stExecutionStructure));
    (*exeMode)->nExecutionMode = exeParams[0];
    (*exeMode)->nMutantInit = exeParams[1];
    (*exeMode)->nMutantEnd = exeParams[2];
    (*exeMode)->nTestInit = exeParams[3];
    (*exeMode)->nTestEnd = exeParams[4];
    (*exeMode)->nWorker = m_nRank;
    (*exeMode)->dInitTick = 0.0;
    (*exeMode)->dInitTick = 0.0;

    if (DEBUG_MPI_OPS) printf("<%d> receiveDeployMode - End\n", m_nRank);    
}

 void receiveDeployModeRef(T_stExecutionStructure* exeMode) {
    MPI_Status status;
    int exeParams[NUM_DEPLOY_PARAMS];

    if (DEBUG_MPI_OPS) printf("<%d> receiveDeployMode - Receiving\n", m_nRank);

    //Receive the deploy
    MPI_Recv(&exeParams, 1, m_DeployType, 0, 0, MPI_COMM_WORLD, &status);

    //Copy parameters    
    if(exeMode == NULL)
        exeMode = (T_stExecutionStructure*) malloc(sizeof (T_stExecutionStructure));
    
    exeMode->nExecutionMode = exeParams[0];
    exeMode->nMutantInit = exeParams[1];
    exeMode->nMutantEnd = exeParams[2];
    exeMode->nTestInit = exeParams[3];
    exeMode->nTestEnd = exeParams[4];
    exeMode->nWorker = m_nRank;
    exeMode->dInitTick = 0.0;
    exeMode->dInitTick = 0.0; 
    
    if (DEBUG_MPI_OPS) printf("<%d> receiveDeployMode - End\n", m_nRank);    
}

void sendTest(T_stTestInfo* pTest, int nDest) {
    T_stTI redTest;
    if (DEBUG_MPI_OPS) printf("<%d> sendTest -  Init\n", m_nRank);
    if (pTest != NULL) {
        printTest(pTest);
        test2redTest(pTest, &redTest);
        printTestRed(&redTest);
              
        MPI_Send(&redTest, 1, m_TestType, nDest, 0, MPI_COMM_WORLD);
    } else
        printf("WARNING!! The test is empty\n");

    if (DEBUG_MPI_OPS) printf("<%d>  sendTest - Test sent to %d!\n", m_nRank, nDest);
}

void sendTests(T_stExecutionStructure* pExeRetMode, int nDest) {
    T_stTestInfo* pTest;
    T_stMutant* pMutant;
    if (pExeRetMode) {
        pMutant = m_oMutantList.array[pExeRetMode->nMutantInit];
        if (pMutant) {
            pTest = pMutant->oTestList.tests[pExeRetMode->nTestInit];

            if (pTest) {
                test2redTest(pTest, &m_oRedTestList[0]);
                MPI_Send(m_oRedTestList, 1, m_TestType, nDest, 0, MPI_COMM_WORLD);
            }
        }
    }

    if (DEBUG_MPI_OPS) printf("<%d> End!\n", m_nRank);
}

T_stTestInfo* receiveTest(int nSource) {
    T_stTestInfo* pTest;
    MPI_Status status;
    T_stTI oRedTest;
    
    MPI_Recv(&oRedTest, 1, m_TestType, nSource, 0, MPI_COMM_WORLD, &status);
    if (m_nRank != MALONE_MASTER) {
        if (DEBUG_MPI_OPS) printf("Received Test: [%d] %s %lf %d | from %d | error %d\n",
                0, oRedTest.res, oRedTest.dTime, oRedTest.nKill, status.MPI_SOURCE, status.MPI_ERROR);

        //copy the values!!
        pTest = malloc(sizeof (T_stTestInfo));
        bzero(pTest->res, MAX_RESULT_SIZE);
        strcpy(pTest->res, oRedTest.res);
        pTest->dTime = oRedTest.dTime;
        pTest->nKill = oRedTest.nKill;
        pTest->nTest = oRedTest.nTest;
    }

    if (DEBUG_MPI_OPS) printf("<%d> End!\n", m_nRank);

    return pTest;
}

void receiveTestRefP(int nSource, T_stTestInfo** pTest) 
{    
    MPI_Status status;
    T_stTI oTest;
    MPI_Recv(&oTest, 1, m_TestType, nSource, 0, MPI_COMM_WORLD, &status);
    if (m_nRank != MALONE_MASTER) {
        if (DEBUG_MPI_OPS) printf("Received Test: [%d] %s %lf %d | from %d | error %d\n",
                0, oTest.res, oTest.dTime, oTest.nKill, status.MPI_SOURCE, status.MPI_ERROR);

        //copy the values!!
        *pTest = malloc(sizeof (T_stTestInfo));
        bzero((*pTest)->res, MAX_RESULT_SIZE);
        strcpy((*pTest)->res, oTest.res);
        (*pTest)->dTime = oTest.dTime;
        (*pTest)->nKill = oTest.nKill;
        (*pTest)->nTest = oTest.nTest;
    }

    if (DEBUG_MPI_OPS) printf("<%d> End!\n", m_nRank);
}
//TODO: Esto no es un poco raro? Solo se recibe 1 unico test, borrando
/*
T_stTestInfo* receiveTestList(int nSource, int nTests) 
{
    T_stTestInfo* pTest;
    MPI_Status status;

    MPI_Recv(m_oRedTestList, nTests, m_TestType, nSource, 0, MPI_COMM_WORLD, &status);


    if (DEBUG_MPI_OPS) printf("Received Test: [%d] %s %lf %d\n", 0, m_oRedTestList[0].res, m_oRedTestList[0].dTime, m_oRedTestList[0].nKill);
    //copy the values!!
    pTest = malloc(sizeof (T_stTestInfo));
    bzero(pTest->res, MAX_RESULT_SIZE);
    strcpy(pTest->res, m_oRedTestList[0].res);
    pTest->dTime = m_oRedTestList[0].dTime;
    pTest->nKill = m_oRedTestList[0].nKill;
    pTest->nTest = m_oRedTestList[0].nTest;

    if (DEBUG_MPI_OPS) printf("<%d> End!\n", m_nRank);

    return pTest;
}
*/
void sendMutant(T_stMutant* pMutant, int nDest) {
    T_stM oRedMutant;
    int i;

    printfMutantFull(pMutant);
    
    if (pMutant && nDest >= 0) {
        oRedMutant.nNumber = pMutant->nNumber;
        oRedMutant.nState = pMutant->nState;
        oRedMutant.nTestKiller = pMutant->nTestKiller;
        oRedMutant.nTests = pMutant->oTestList.nElems;

        for (i = 0; i < pMutant->oTestList.nElems; i++) {
            oRedMutant.oKill[i] = pMutant->oTestList.tests[i]->nKill;
            oRedMutant.oTest[i] = pMutant->oTestList.tests[i]->nTest;
            oRedMutant.oTime[i] = pMutant->oTestList.tests[i]->dTime;
        }

        MPI_Send(&oRedMutant, 1, m_MutantType, nDest, 0, MPI_COMM_WORLD);
    } else {
        if (!pMutant)
            printf("sendMutant - WARNING!! Null mutant.\n");
        if (nDest < 0)
            printf("sendMutant - WARNING!! Negative destination.\n");
    }
}

void resetMutant(T_stExecutionStructure* pExeMode) {
    int nMutantInit, nMutantEnd, nTotalMutants, i, j, nTestsToDelete;

    nMutantInit = nMutantEnd = nTotalMutants = nTestsToDelete = 0;

    if (DEBUG_MPI_OPS) printf("<%d>resetMutant - Init\n", m_nRank);
    if (pExeMode) {
        nMutantInit = pExeMode->nMutantInit;
        nMutantEnd = pExeMode->nMutantEnd;
        nTotalMutants = (nMutantEnd - nMutantInit) + 1;

        if (DEBUG_MPI_OPS) printf("<%d>resetMutant - Reseting %d mutants - [%d - %d]\n", m_nRank, nTotalMutants, nMutantInit, nMutantEnd);
        if (nTotalMutants > 0) {
            for (i = nMutantInit; i <= nMutantEnd; i++) {
                T_stMutant* pMutant = m_oMutantList.array[i];
                T_stTestInfo* pTest;
                if (pMutant) {
                    if (DEBUG_MPI_OPS) printf("<%d> resetMutant - Reseting mutant %d , total tests: %d \n", m_nRank, i, pMutant->oTestList.nElems);

                    for (j = 0; j < MAX_TESTS && nTestsToDelete < pMutant->oTestList.nElems; j++) {
                        pTest = pMutant->oTestList.tests[j];
                        if (pTest) {
                            if (DEBUG_MPI_OPS) printf("<%d> resetMutant - Reseting mutant %d , test index:%d - real:%d deleted\n", m_nRank, i, j, pTest->nTest);
                            //free(pTest);
                            pTest = NULL;
                            nTestsToDelete++;
                        }
                        pMutant->oTestList.tests[j] = NULL;
                    }
                    pMutant->oTestList.nElems = 0;
                    nTestsToDelete = 0;
                    
#ifdef EXPERIMENTAL_MEM_SAFE 
                    m_oRedMutantList[i - nMutantInit].nTests = 0;
                    m_oRedMutantList[i - nMutantInit].nState = 1;
                    if (DEBUG_MPI_OPS) printf("<%d> resetMutant - REAL Reseting mutant %d\n", m_nRank, i - nMutantInit);
#else 
                    m_oRedMutantList[i].nTests = 0;
                    m_oRedMutantList[i].nState = 1;
                    if (DEBUG_MPI_OPS) printf("<%d> resetMutant - REAL Reseting mutant %d\n", m_nRank, i);
#endif                     
                } else
                    printf("<%d>resetMutant - WARNING! empty mutant\n", m_nRank);

            }

        } else {
            printf("<%d>resetMutant - WARNING!! Negative number of mutants to send!!\n", m_nRank);
        }
    } else {
        if (!pExeMode)
            printf("<%d>resetMutant - WARNING!! Null mutant.\n", m_nRank);
    }

    if (DEBUG_MPI_OPS) printf("<%d>resetMutant - End\n", m_nRank);
}

void sendMutants(T_stExecutionStructure* pExeMode, int nDest) {
    int i, j, nTotalMutants, nMutantInit, nMutantEnd, nTotalTestsToSend;
    T_stMutant* pMutant;
    T_stTestInfo* pTest;
    if (DEBUG_MPI_OPS) printf("<%d>sendMutants - Init\n", m_nRank);
    if (pExeMode && nDest >= 0) {
        nTotalTestsToSend = 0;
        nMutantInit = pExeMode->nMutantInit;
        nMutantEnd = pExeMode->nMutantEnd;
        nTotalMutants = (nMutantEnd - nMutantInit) + 1;

        if (DEBUG_MPI_OPS) printf("<%d>sendMutants - Sending %d mutants - [%d - %d]\n", m_nRank, nTotalMutants, nMutantInit, nMutantEnd);
        if (nTotalMutants > 0) {
            for (i = nMutantInit; i <= nMutantEnd; i++) {
                pMutant = m_oMutantList.array[i];
                pTest = NULL;;
                
                if (pMutant) {
                    if (DEBUG_MPI_OPS) printf("<%d>sendMutants - [Mutant - %d] Inserting Tests: %d \n", m_nRank, i, pMutant->oTestList.nElems);
                    printMutant(pMutant);

#ifdef EXPERIMENTAL_MEM_SAFE 
                    m_oRedMutantList[i - nMutantInit].nNumber = pMutant->nNumber;
                    m_oRedMutantList[i - nMutantInit].nState = pMutant->nState;
                    m_oRedMutantList[i - nMutantInit].nTestKiller = pMutant->nTestKiller;
                    if (DEBUG_MPI_OPS) printf("<%d>sendMutants - [Mutant - %d] State: %d, killing test: %d\n", m_nRank, i, m_oRedMutantList[i - nMutantInit].nState, m_oRedMutantList[i - nMutantInit].nTestKiller);
#else                           
                    m_oRedMutantList[i].nNumber = pMutant->nNumber;
                    m_oRedMutantList[i].nState = pMutant->nState;
                    m_oRedMutantList[i].nTestKiller = pMutant->nTestKiller;
                    if (DEBUG_MPI_OPS) printf("<%d>sendMutants - [Mutant - %d] State: %d, killing test: %d\n", m_nRank, i, m_oRedMutantList[i].nState, m_oRedMutantList[i].nTestKiller);
#endif                      
                    nTotalTestsToSend = 0;
                    for (j = 0; j < MAX_TESTS && nTotalTestsToSend < pMutant->oTestList.nElems; j++) {

                        pTest = pMutant->oTestList.tests[j];
                        if (pTest) {
#ifdef EXPERIMENTAL_MEM_SAFE 
                            m_oRedMutantList[i - nMutantInit].oKill[j] = pTest->nKill;
                            m_oRedMutantList[i - nMutantInit].oTest[j] = pTest->nTest;
                            m_oRedMutantList[i - nMutantInit].oTime[j] = pTest->dTime;
                            m_oRedMutantList[i - nMutantInit].nTests++;
#else                            
                            m_oRedMutantList[i].oKill[j] = pTest->nKill;
                            m_oRedMutantList[i].oTest[j] = pTest->nTest;
                            m_oRedMutantList[i].oTime[j] = pTest->dTime;
                            m_oRedMutantList[i].nTests++;
#endif                        
                            /* To delete, debug intro
                            if(pTest)
                            {
                                printf("<+ Numbers: redmutIndex: %d, jIndex: %d\n", i-nMutantInit, j);
                                printf("<+ Overall test %d,%lf,%d,%s>\n",pTest->nTest, pTest->dTime,pTest->nKill,pTest->res);
                                printf("<+ sendp %d(%d, %d, %lf)\n",i-nMutantInit, pTest->nKill,pTest->nTest,pTest->dTime);
                                printf("<+ sendred: %d( %d, %d, %lf)\n", i-nMutantInit, m_oRedMutantList[i-nMutantInit].oKill[j], m_oRedMutantList[i-nMutantInit].oTest[j], m_oRedMutantList[i-nMutantInit].oTime[j]);
                                printf("<+ sendfix0: %d( %d, %d, %lf)\n", 0, m_oRedMutantList[0].oKill[0], m_oRedMutantList[0].oTest[0], m_oRedMutantList[0].oTime[0]);
                            }*/
                            
                            /* Also to delete
                            if (DEBUG_MPI_OPS) printf("<%d>sendMutants - Inserting test: ", m_nRank);
                            printTest(pTest);
                            if (DEBUG_MPI_OPS) printf("<%d>sendMutants - -\n", m_nRank);
                             * */
                            nTotalTestsToSend++;
                        }
                        else
                             printf("<%d>sendMutants - WARNING! empty test %d\n", m_nRank, j);
                    }

                } else
                    printf("<%d>sendMutants - WARNING! empty mutant\n", m_nRank);
                if (DEBUG_MPI_OPS) printf("<%d>sendMutants - Configured mutant %d with tests: %d test-list-size: %d\n", m_nRank, i, nTotalTestsToSend, m_oRedMutantList[i - nMutantInit].nTests);
            
            }
            if (DEBUG_MPI_OPS) printf("<%d>sendMutants - Informing the master with the number of mutants to send: %d\n", m_nRank, nTotalMutants);
            //Informs the master with the number of mutants to send        
            MPI_Send(&nTotalMutants, 1, MPI_INT, nDest, 0, MPI_COMM_WORLD);

            if (DEBUG_MPI_OPS) printf("<%d>sendMutants - Sending mutants!\n", m_nRank);
            //Then, send the mutants!            
#ifdef EXPERIMENTAL_MEM_SAFE 
           /* TODO: delete
            * printf("<+ sendfix0: %d( %d, %d, %ld)\n", 0, m_oRedMutantList[0].oKill[0], m_oRedMutantList[0].oTest[0], m_oRedMutantList[0].oTime[0]);
            printf("<+ sendfix1: %d( %d, %d, %ld)\n", 0, m_oRedMutantList[0].oKill[1], m_oRedMutantList[0].oTest[1], m_oRedMutantList[0].oTime[1]);
            * */
            MPI_Send(&m_oRedMutantList[0], nTotalMutants, m_MutantType, nDest, 0, MPI_COMM_WORLD);
#else   
            MPI_Send(&m_oRedMutantList[nMutantInit], nTotalMutants, m_MutantType, nDest, 0, MPI_COMM_WORLD);
#endif      

        } else {
            printf("<%d>sendMutants - WARNING!! Negative number of mutants to send!!\n", m_nRank);
        }
    } else {
        if (!pExeMode)
            printf("<%d>sendMutants - WARNING!! Null mutant.\n", m_nRank);
        if (nDest < 0)
            printf("<%d>sendMutants - WARNING!! Negative destination.\n", m_nRank);
    }
    #ifdef EXPERIMENTAL_MEM_SAFE  
    for (i = nMutantInit; i <= nMutantEnd; i++) {
        //After sending, reset the mutants in safe mode
        //we must reset the mutant!!
        m_oRedMutantList[i - nMutantInit].nTests=0;
    }

    #endif 
    if (DEBUG_MPI_OPS) printf("sendMutants - End\n");
}

void sendMd5Mutants(T_stExecutionStructure* pExeMode, char** strMd5List, int nTotalSizeInput, int nDest) {
    int i, nTotalMutants, nMutantInit, nMutantEnd, nTotalTestsToSend, nTotalSize, nFixedSize;    
    char* strMd5ListPlain;

    if (DEBUG_MPI_OPS) printf("<%d>sendMd5Mutants - Init\n", m_nRank);
    if (pExeMode && nDest >= 0) {
        nTotalTestsToSend = 0;
        nMutantInit = pExeMode->nMutantInit;
        nMutantEnd = pExeMode->nMutantEnd;

        nTotalMutants = (nMutantEnd - nMutantInit) + 1;

        if (DEBUG_MPI_OPS) printf("<%d>sendMd5Mutants - Sending %d mutants - [%d - %d]\n", m_nRank, nTotalMutants, nMutantInit, nMutantEnd);
        if (nTotalMutants > 0) {

            nTotalSize = sizeof (char) * sizeof (strMd5List);

            if (DEBUG_MPI_OPS) printf("<%d>sendMd5Mutants - Informing the master with the number of mutants to send: %d\n", m_nRank, nTotalMutants);
            //Informs the master with the number of mutants to send        
            MPI_Send(&nTotalMutants, 1, MPI_INT, nDest, 0, MPI_COMM_WORLD);

            //Informs the master with the number of mutants to send        
            MPI_Send(&nTotalSizeInput, 1, MPI_INT, nDest, 0, MPI_COMM_WORLD);

            if (DEBUG_MPI_OPS) printf("<%d>sendMd5Mutants - Sending md5 mutants!\n", m_nRank);

            //plain the md5List
            strMd5ListPlain = malloc(sizeof (char*)*nTotalSizeInput);
            bzero(strMd5ListPlain, nTotalSizeInput);

            int nResLen, nLineLen;
            nFixedSize = 0;
            for (i = 0; i < nTotalMutants; i++) {
                char* strMd5 = strMd5List[i];
                if (DEBUG_MPI_OPS) printf("md5_%d=[%s]\n", i, strMd5);
                if (strMd5 != NULL) {
                    nLineLen = strlen(strMd5) + 1;
                    memcpy(strMd5ListPlain + nFixedSize, strMd5, nLineLen);
                    nFixedSize += nLineLen;
                } else {
                    nFixedSize++;
                }
            }
            nFixedSize++;

            //Then, send the mutants!
            MPI_Send(strMd5ListPlain, sizeof (char)*nTotalSizeInput, MPI_CHAR, nDest, 0, MPI_COMM_WORLD);
            free(strMd5ListPlain);

        } else {
            printf("<%d>sendMd5Mutants - WARNING!! Negative number of mutants to send!!\n", m_nRank);
        }
    } else {
        if (!pExeMode)
            printf("<%d>sendMd5Mutants - WARNING!! Null mutant.\n", m_nRank);
        if (nDest < 0)
            printf("<%d>sendMd5Mutants - WARNING!! Negative destination.\n", m_nRank);
    }

    if (DEBUG_MPI_OPS) printf("sendMd5Mutants - End\n");
}

int receiveSingleTestAndCheck(T_stExecutionStructure pExeMode[MAX_WORKERS], int *pnWorkerSource) {
    int nRet, nTests, nWorkerSource, nMutantInit, nTestInit, nRecvCode;
    MPI_Status status;
    T_stTestInfo* pTest;
    T_stTI oRedTest;
    
    //status = malloc(sizeof(MPI_Status));
    if (DEBUG_MPI_OPS) printf("receiveSingleTestAndCheck - Init\n");

    nRet = nTests = nWorkerSource = nMutantInit = nTestInit = 0;
    nTests = 1;

    nRecvCode = MPI_Recv(&oRedTest, 1, m_TestType, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

    if (DEBUG_MPI_OPS) printf("<%d>receiveSingleTestAndCheck - Receiving %d test from %d | code: %d\n", m_nRank, nTests, status.MPI_SOURCE, nRecvCode);
    nWorkerSource = status.MPI_SOURCE;

    if (nWorkerSource != -1 && nWorkerSource < m_nSize) {
        nMutantInit = pExeMode[nWorkerSource].nMutantInit;
        nTestInit = pExeMode[nWorkerSource].nTestInit;

        if (DEBUG_MPI_OPS) printf("receiveSingleTestAndCheck- Received Test: w<%d> [%d-%d] %s %lf %d\n", nWorkerSource, nMutantInit, nTestInit, m_oRedTestList[0].res, m_oRedTestList[0].dTime, m_oRedTestList[0].nKill);

        //copy the values!!
        pTest = redtest2Test(&oRedTest);
        pTest->dInitTick = pExeMode[nWorkerSource].dInitTick;
        pTest->dEndTick = getTick();

        if (DEBUG_MPI_OPS) printf("receiveSingleTestAndCheck- ticks -  %f/%f\n", pTest->dInitTick, pTest->dEndTick);
        
        //TODO: Aligerar codigo, pasando este metodo completo a otro.
        if (!checkTestResult(pTest)) {
            pTest->nKill = 1;
            if (DEBUG_MPI_OPS) printf("receiveSingleTestAndCheck- Test %d of Mutant %d Killed!!\n", pTest->nTest, oRedTest.nMutant);
            
            //Check different min and max values
            if(nMutantInit >=0 && nMutantInit<MAX_MUTANTS && nTestInit>=0 && nTestInit < MAX_TESTS)
            {
                if(m_oTestExecMap.pMap != NULL && nMutantInit<m_oTestExecMap.nMutants && nTestInit<m_oTestExecMap.nTests)
                    m_oTestExecMap.pMap[nMutantInit][nTestInit] = nWorkerSource * 100;
                else
                    printf("receiveSingleTestAndCheck - WARNING! receiving test-mutant: %d-%d\n", nTestInit, nMutantInit);
            }
            else
                printf("receiveSingleTestAndCheck - WARNING! receiving test-mutant: %d-%d\n", nTestInit, nMutantInit);
            
        } else {
            nRet = 1;
            if(nMutantInit >=0 && nMutantInit<MAX_MUTANTS && nTestInit>=0 && nTestInit < MAX_TESTS)
            {
                if(m_oTestExecMap.pMap != NULL && nMutantInit<m_oTestExecMap.nMutants && nTestInit<m_oTestExecMap.nTests)
                    m_oTestExecMap.pMap[nMutantInit][nTestInit] = nWorkerSource;
                else
                    printf("receiveSingleTestAndCheck - WARNING! receiving test-mutant: %d-%d\n", nTestInit, nMutantInit);
            }
            else
                printf("receiveSingleTestAndCheck - WARNING! receiving test-mutant: %d-%d\n", nTestInit, nMutantInit);
        }

        allocMutant(nMutantInit);
        insertTestResult(nMutantInit, nTestInit, pTest);
    } else {
        printf("receiveSingleTestAndCheck - WARNING! Invalid worker source (%d) - Error: %d\n", nWorkerSource, status.MPI_ERROR);
        nRet = 0;
        char error_string[BUFSIZ];
        int length_of_error_string, error_class;

        MPI_Error_class(status.MPI_ERROR, &error_class);
        MPI_Error_string(error_class, error_string, &length_of_error_string);
        fprintf(stderr, "%3d: %s\n", m_nRank, error_string);
        MPI_Error_string(status.MPI_ERROR, error_string, &length_of_error_string);
        fprintf(stderr, "%3d: %s\n", m_nRank, error_string);
    }

    //Copy the worker source
    *pnWorkerSource = nWorkerSource;

    if (DEBUG_MPI_OPS) printf("receiveSingleTestAndCheck - End\n");
    return nRet;
}

int receiveSingleTestOriginal(T_stExecutionStructure pExeMode[MAX_WORKERS], int *pnWorkerSource) {
    int nRet, nTests, nWorkerSource, nTestInit, nRecvCode;
    MPI_Status status;
    T_stTestInfo* pTest;
    T_stTI oRedTest;
    
    if (DEBUG_MPI_OPS) printf("receiveSingleTestOriginal - Init\n");

    nRet = nTests = nWorkerSource = nTestInit = 0;
    nTests = 1;

    nRecvCode = MPI_Recv(&oRedTest, 1, m_TestType, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    if (DEBUG_MPI_OPS) printf("<%d>receiveSingleTestOriginal - Receiving %d test from %d | code: %d\n", m_nRank, nTests, status.MPI_SOURCE, nRecvCode);

    nWorkerSource = status.MPI_SOURCE;

    if (nWorkerSource != -1 && nWorkerSource < m_nSize) {
        nTestInit = pExeMode[nWorkerSource].nTestInit;

        if (DEBUG_MPI_OPS) printf("receiveSingleTestOriginal- Received Test: [%d] %s %lf %d\n", nTestInit, oRedTest.res, oRedTest.dTime, oRedTest.nKill);

        //translate from reduced test to test
        pTest = redtest2Test(&oRedTest);

        pTest->dInitTick = pExeMode[nWorkerSource].dInitTick;
        pTest->dEndTick = getTick();

        if (DEBUG_MPI_OPS) printf("receiveSingleTestOriginal- %f/%f\n", pTest->dInitTick, pTest->dEndTick);
        printTest(pTest);
        if (!checkTestResultOriginal(pTest)) {
            pTest->nKill = 1;
            if (DEBUG_MPI_OPS) printf("receiveSingleTestOriginal- Test %d failed!!\n", pTest->nTest);
        } else {
            nRet = 1;
            if (DEBUG_MPI_OPS) printf("receiveSingleTestOriginal- <src: %d>Inserting test %d\n", nWorkerSource, pTest->nTest);
            addResultIndex(m_pResList, pTest->nTest, pTest->res, pTest->dTime, 0);
            if (DEBUG_MPI_OPS) printf("%s", pTest->res);
        }
    } else {
        printf("receiveSingleTestOriginal - WARNING! Invalid worker source (%d) - Error: %d\n", nWorkerSource, status.MPI_ERROR);
        nRet = 0;
        char error_string[BUFSIZ];
        int length_of_error_string, error_class;

        MPI_Error_class(status.MPI_ERROR, &error_class);
        MPI_Error_string(error_class, error_string, &length_of_error_string);
        fprintf(stderr, "%3d: %s\n", m_nRank, error_string);
        MPI_Error_string(status.MPI_ERROR, error_string, &length_of_error_string);
        fprintf(stderr, "%3d: %s\n", m_nRank, error_string);
    }

    //Copy the worker source
    *pnWorkerSource = nWorkerSource;

    if (DEBUG_MPI_OPS) printf("receiveSingleTestOriginal - End\n");
    return nRet;
}

int receiveMutants(T_stExecutionStructure pExeMode[MAX_WORKERS]) {
    int i, nTotalMutants, nMutantInit, nMutantEnd, nTestInit, nTestEnd, nMutants, nWorkerSource, nTotalTests;
    MPI_Status status;
    T_stMutant* pMutant;

    if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - Init\n", m_nRank);

    if (pExeMode) {
        if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - Waiting for the number of mutants to receive\n", m_nRank);
        MPI_Recv(&nMutants, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - Receiving %d mutants from worker %d\n", m_nRank, nMutants, status.MPI_SOURCE);
        nWorkerSource = status.MPI_SOURCE;

        if (nWorkerSource != -1) {
            nMutantInit = pExeMode[nWorkerSource].nMutantInit;
            nMutantEnd = pExeMode[nWorkerSource].nMutantEnd;
            nTestInit = pExeMode[nWorkerSource].nTestInit;
            nTestEnd = pExeMode[nWorkerSource].nTestEnd;
            nTotalMutants = (nMutantEnd - nMutantInit)+1;
            nTotalTests = (nTestEnd - nTestInit);

            if (nMutantInit > 0) {
                //
#ifdef EXPERIMENTAL_MEM_SAFE 
                MPI_Recv(&m_oRedMutantList[0], nMutants, m_MutantType, nWorkerSource, 0, MPI_COMM_WORLD, &status);
#else
                MPI_Recv(&m_oRedMutantList[nMutantInit], nMutants, m_MutantType, nWorkerSource, 0, MPI_COMM_WORLD, &status);
#endif
                if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - Received mutants!! Mutants: %d, tests: %d\n", m_nRank, nTotalMutants, nTotalTests);

                printf("<%d>receiveMutants - 0\n", m_nRank);
                
                //Transform
                for (i = nMutantInit; i <= nMutantEnd; i++) {
                    
#ifdef EXPERIMENTAL_MEM_SAFE 
                    pMutant = redMut2Mut((T_stM*) &m_oRedMutantList[i-nMutantInit]);                    
#else
                    pMutant = redMut2Mut((T_stM*) &m_oRedMutantList[i]);
#endif                    
                    insertMutantTestByTest(pMutant, i, status.MPI_SOURCE);
                    
                    if (pMutant != NULL)
                        free_mutant(pMutant);
                    
                    printf("<%d>receiveMutants - 3.%d\n", m_nRank, i);
                }
            } else {
                printf("<%d>receiveMutants - WARNING!! Invalid mutant index!\n", m_nRank);
            }
        } else {
            printf("<%d>receiveMutants - WARNING!! Invalid source!\n", m_nRank);
        }
    } else {
        printf("<%d>receiveMutants - WARNING!! Execution structure is empty!\n", m_nRank);
    }

    if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - End\n", m_nRank);

    return nWorkerSource;
}

int receiveMd5Mutants(T_stExecutionStructure pExeMode[MAX_WORKERS]) {
    int i, nTotalMutants, nMutantInit, nMutantEnd, nMutants, nWorkerSource, nTotalSize, nIndexAux;
    MPI_Status status;
    char* strMd5, *strMd5List;

    if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - Init\n", m_nRank);

    i = nTotalMutants = nMutantInit = nMutantEnd = nMutants = nWorkerSource = nTotalSize = nIndexAux = 0;
    if (pExeMode) {
        if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - Waiting for the number of mutants to receive\n", m_nRank);
        MPI_Recv(&nMutants, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - Receiving %d mutants from worker %d\n", m_nRank, nMutants, status.MPI_SOURCE);
        nWorkerSource = status.MPI_SOURCE;

        if (nWorkerSource != -1) {
            nMutantInit = pExeMode[nWorkerSource].nMutantInit;
            nMutantEnd = pExeMode[nWorkerSource].nMutantEnd;
            nTotalMutants = (nMutantEnd - nMutantInit) + 1;

            if (nMutantInit > 0) {

                MPI_Recv(&nTotalSize, 1, MPI_INT, nWorkerSource, 0, MPI_COMM_WORLD, &status);

                strMd5List = malloc(sizeof (char*)*nTotalSize);

                MPI_Recv(strMd5List, nTotalSize, MPI_BYTE, nWorkerSource, 0, MPI_COMM_WORLD, &status);

                if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - Received mutants!! Mutants: %d\n", m_nRank, nTotalMutants);

                //Transform
                int nInterLen = 0;
                for (i = nMutantInit; i <= nMutantEnd; i++) {

                    strMd5 = strMd5List + nInterLen;
                    if (strMd5 != NULL) {
                        if (DEBUG_MPI_OPS) printf("%d > %s", i, strMd5);
                        //Insert the MD5 hash in the list
                        m_oTestExecMap.oMd5Map[i] = strdup(strMd5);
                        nInterLen += strlen(strMd5) + 1;
                    } else {
                        m_oTestExecMap.oMd5Map[i] = NULL;
                        //nInterLen += ((int) strlen(strMd5)) + 1;
                    }

                }
            } else {
                printf("<%d>receiveMutants - WARNING!! Invalid mutant index!\n", m_nRank);
            }
        } else {
            printf("<%d>receiveMutants - WARNING!! Invalid source!\n", m_nRank);
        }
    } else {
        printf("<%d>receiveMutants - WARNING!! Execution structure is empty!\n", m_nRank);
    }

    if (DEBUG_MPI_OPS) printf("<%d>receiveMutants - End\n", m_nRank);

    return nWorkerSource;
}

MutantList* receiveMutants_adaptive(T_stExecutionStructure pExeMode[MAX_WORKERS], int* pnWorkerSource) {
    int i, nTotalMutants, nMutantInit, nMutantEnd, nMutants, nWorkerSource, nTestInit, nTestEnd, nTotalTests, nRetIndex;
    MPI_Status status;
    T_stMutant* pMutant;
    MutantList* pMutantList;

    if (DEBUG_MPI_OPS) printf("<%d>receiveMutants_adaptive - Init\n", m_nRank);

    if (pExeMode) {

        nRetIndex = 0;
        if (DEBUG_MPI_OPS) printf("<%d>receiveMutants_adaptive - Waiting for the number of mutants to receive\n", m_nRank);
        MPI_Recv(&nMutants, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        pMutantList = generateCleanMutantList(nMutants);

        if (DEBUG_MPI_OPS) printf("<%d>receiveMutants_adaptive - Receiving %d mutants from worker %d\n", m_nRank, nMutants, status.MPI_SOURCE);
        nWorkerSource = status.MPI_SOURCE;

        if (nWorkerSource != -1) {
            nMutantInit = pExeMode[nWorkerSource].nMutantInit;
            nMutantEnd = pExeMode[nWorkerSource].nMutantEnd;
            nTestInit = pExeMode[nWorkerSource].nTestInit;
            nTestEnd = pExeMode[nWorkerSource].nTestEnd;
            nTotalMutants = (nMutantEnd - nMutantInit) + 1;
            nTotalTests = (nTestEnd - nTestInit) + 1;

            if (nMutantInit > 0) {
                
#ifdef EXPERIMENTAL_MEM_SAFE                 
                MPI_Recv(&m_oRedMutantList[0], nMutants, m_MutantType, nWorkerSource, 0, MPI_COMM_WORLD, &status);
#else
                MPI_Recv(&m_oRedMutantList[nMutantInit], nMutants, m_MutantType, nWorkerSource, 0, MPI_COMM_WORLD, &status);
#endif
                if (DEBUG_MPI_OPS) printf("<%d>receiveMutants_adaptive - Received mutants from worker %d!! Mutants: %d, (theoretically) tests: %d\n", m_nRank, status.MPI_SOURCE, nTotalMutants, nTotalTests);

                //Transform
                for (i = nMutantInit; i <= nMutantEnd; i++) {
                    
                    //printf("<+ received: %d( %d, %d, %ld)\n", i, m_oRedMutantList[i-nMutantInit].oKill[i], m_oRedMutantList[i-nMutantInit].oTest[i], m_oRedMutantList[i-nMutantInit].oTime[i]);
#ifdef EXPERIMENTAL_MEM_SAFE   
                    pMutant = redMut2Mut((T_stM*) & m_oRedMutantList[i-nMutantInit]);
#else                    
                    pMutant = redMut2Mut((T_stM*) & m_oRedMutantList[i]);
#endif
                    //insert
                    insertMutantTestByTest(pMutant, i, nWorkerSource);

                    if (pMutant && (pMutant->nState == 0))
                        pMutantList->nDead++;

                    pMutantList->array[nRetIndex] = pMutant;
                    pMutantList->nElems++;
                    nRetIndex++;

                    //reset mutant 
#ifdef EXPERIMENTAL_MEM_SAFE                      
                    m_oRedMutantList[i-nMutantInit].nTests = 0;
                    m_oRedMutantList[i-nMutantInit].nTestKiller = -1;
                    m_oRedMutantList[i-nMutantInit].nState = 1;                    
#else
                    m_oRedMutantList[i].nTests = 0;
                    m_oRedMutantList[i].nTestKiller = -1;
                    m_oRedMutantList[i].nState = 1;
#endif
                }

            } else {
                printf("<%d>receiveMutants_adaptive - WARNING!! Invalid mutant index!\n", m_nRank);
            }
            *pnWorkerSource = nWorkerSource;
        } else {
            printf("<%d>receiveMutants_adaptive - WARNING!! Invalid source!\n", m_nRank);
        }
    } else {
        printf("<%d>receiveMutants_adaptive - WARNING!! Execution structure is empty!\n", m_nRank);
    }

    if (DEBUG_MPI_OPS) printf("<%d>receiveMutants_adaptive - End\n", m_nRank);

    return pMutantList;
}

T_stMutant* receiveMutant(int nSource) {
    MPI_Status status;
    T_stMutant* pMutant;

    if (nSource >= 0) {
        MPI_Recv(&m_oRedMutantList, 1, m_MutantType, nSource, 0, MPI_COMM_WORLD, &status);

        pMutant = redMut2Mut((T_stM*) & m_oRedMutantList[0]);
    } else
        printf("receiveMutant - WARNING!! Negative source.\n");

    return pMutant;
}

T_stMutant* receiveMutantAnySource(int* pnMutant) {    
    MPI_Status status;
    T_stMutant* pMutant;
    T_stM oMutant;
    
    MPI_Recv(&oMutant, 1, m_MutantType, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

    if(pnMutant)
        *pnMutant=status.MPI_SOURCE;
    
    pMutant = redMut2Mut((T_stM*) &oMutant);

    return pMutant;
}

int sendOriginalTestResults(T_stTestList* pTestList) {
    int nRet, nTestItems;

    if (DEBUG_MPI_OPS) printf("sendOriginalTestResults - Init\n");

    if (pTestList) {
        nTestItems = pTestList->nElems;

        //transform the test list to a compatible structure
        testList2redTestList((T_stTI*) & m_oRedTestList, pTestList);

        if (DEBUG_MPI_OPS) printf("Sending the size of the test suite: %d tests\n", nTestItems);
        //Send to workers the number of tests to process
        MPI_Bcast(&nTestItems, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (DEBUG_MPI_OPS) printf("Sending the test suite\n");
        //Send the specific tests
        MPI_Bcast(m_oRedTestList, nTestItems, m_TestType, 0, MPI_COMM_WORLD);
    } else
        printf("sendOriginalTestResults - WARNING! The test suite is empty!!!!\n");

    if (DEBUG_MPI_OPS) printf("sendOriginalTestResults - End\n");

    return nRet;
}

void receiveOriginalTestResults(T_stTestList* pResList) {
    int nTestItems;

    if (DEBUG_MPI_OPS) printf("receiveOriginalTestResults - Init\n");

    if (pResList) {
        //Receive from master the number of tests to process
        if (DEBUG_MPI_OPS) printf("Receiving the size of the test suite\n");        
        MPI_Bcast(&nTestItems, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (DEBUG_MPI_OPS) printf("receiveOriginalTestResults - Receiving the size of the test suite: %d\n", nTestItems);

        if (nTestItems > 0) {
            pResList->nElems = nTestItems;

            //Send the specific tests
            MPI_Bcast(&m_oRedTestList, nTestItems, m_TestType, 0, MPI_COMM_WORLD);

            //transform the test list to a compatible structure
            redtestList2TestList((T_stTI*) &m_oRedTestList, pResList, nTestItems);
        } else {
            printf("None elements received!!\n");
        }
    }
    if (DEBUG_MPI_OPS) printf("receiveOriginalTestResults - End\n");
}

T_stTestList* receiveOriginalTestResults_r() {
    int nTestItems;
    T_stTestList* pTestList;

    if (DEBUG_MPI_OPS) printf("receiveOriginalTestResults_r - Init\n");

    //Receive from master the number of tests to process
    MPI_Bcast(&nTestItems, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (DEBUG_MPI_OPS) printf("receiveOriginalTestResults_r - Received test suite: %d tests!\n", nTestItems);
    if (nTestItems > 0) {
        //Send the specific tests
        MPI_Bcast(m_oRedTestList, nTestItems, m_TestType, 0, MPI_COMM_WORLD);

        //transform the test list to a compatible structure
        pTestList = (T_stTestList*) redtestList2TestList_r(m_oRedTestList, nTestItems);
    } else
        printf("receiveOriginalTestResults_r - None elements received!!\n");

    if (DEBUG_MPI_OPS) printf("receiveOriginalTestResults_r - End\n");
    return pTestList;
}

int isEnabledMpiOperations() {
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugMpiOps : 0;
}
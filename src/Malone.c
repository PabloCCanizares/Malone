/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone.c
 *     Main functions of the framework, which provide the principal features of 
 *     MALONE 
 * @par Purpose:
 *     Provide the API of MALONE.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     25 Oct 2015 - 20 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "Malone.h"


#ifdef DISABLELOGS
#define MALONE_MASTER_LOG 1
#define MALONE_WORKER_LOG 1
#define MALONE_MAIN_COMMAND 1
#else
#define MALONE_MASTER_LOG isEnabledMasterLog()
#define MALONE_WORKER_LOG isEnabledWorkersLog()
#endif

//TODO: Temporal define to disable memory allocation
#define FREE_MEM 1

void MonitorAlarmhandler(int sig) {
    char* strCommand, *strResult;
    
    // ignore this signal
    signal(SIGALRM, SIG_IGN);
        
    printf("<%d>Alarm [MonitorAlarmhandler] handled [%d]\n", m_nRank, m_oMonitorLines.nAlarms);
    
    if (m_pListMonitorLines)
    {
        int i;
        for (i = 0; *(m_pListMonitorLines + i); i++)
        {            
            strCommand = *(m_pListMonitorLines + i);
            strResult = execCommandLine(strCommand);
            
            if(m_oMonitorLines.nElems == 0)
            {
                m_oMonitorLines.array = malloc(m_oMonitorLines.nMax*sizeof(*m_oMonitorLines.array)+1);
            }
            else
            {
                if(m_oMonitorLines.nElems>= m_oMonitorLines.nMax)
                {
                    m_oMonitorLines.nMax*=2;
                    m_oMonitorLines.array = realloc(m_oMonitorLines.array, (m_oMonitorLines.nMax) * sizeof(*m_oMonitorLines.array)+1);
                }
            }
            
            m_oMonitorLines.array[m_oMonitorLines.nElems] = malloc(strlen(strResult) * sizeof(char)+1);
            strcpy(m_oMonitorLines.array[m_oMonitorLines.nElems], strResult);
            
            m_oMonitorLines.nElems++;            
        }
        m_oMonitorLines.nAlarms++;
    }
    
    signal(SIGALRM, MonitorAlarmhandler); /* reinstall the handler    */
    if (m_stEnvValues->nMonitorFrequency >= 0)
        alarm(m_stEnvValues->nMonitorFrequency);
    
    printf("<%d>Alarm [MonitorAlarmhandler] handled end [Elems:%d, alarms: %d]\n", m_nRank, m_oMonitorLines.nElems, m_oMonitorLines.nAlarms);
}
/**
 * Function that checks if the provided configuration of the framework is correct, or on the contrary, describes the error.
 * @return true if the cfg is correct.
 */
int malone_isReady() {
    int nRet, nError;

    nRet = nError = 0;

    if (m_nRank == MALONE_MASTER) {
        printf("<%d> isReadyTomalone_isReadyRumble - Checking the validity of the global configuration\n", m_nRank);

        if (m_stConfigValues != NULL && m_stEnvValues != NULL) {
            //Analyse if the number of mutants is greater than the maximum/minimum allowed
            if (m_nSize <= MAX_WORKERS && m_nSize >= 1) {
                //If the number of workers is 1, check if the sequential mode is active
                if ((m_nSelAlgorithm != 1 && m_nSize > 1) || (m_nSelAlgorithm == 1 && m_nSize == 1)) {
                    //Analyse if the number of tests is greater than the maximum/minimum allowed
                    if (m_stEnvValues->nTotalTests <= MAX_TESTS && m_stEnvValues->nTotalTests >= 1) {
                        //Check compiling mutants!
                        if (m_stEnvValues->nCompilationEnabled == 1 && m_stEnvValues->nParallelCompilation == 1 &&
                                m_stEnvValues->nCompilationNumWorkers == 0) {
                            nError++;
                        }
                        nRet = (nError == 0);
                    } else
                        printf("<%d> malone_isReady - Error setting the number of TCs to be applied, valid range [%d - %d]\n", m_nRank, 1, MAX_TESTS);
                } else
                    printf("<%d> malone_isReady - Error configurating the number of WORKERS, the valid range is [%d - %d]. "
                        "A unique worker only is valid for non-sequential mode (selected: %d)\n", m_nRank, 2, MAX_TESTS, m_nSelAlgorithm);
            } else
                printf("<%d> malone_isReady - Error setting the number of workers, valid range [%d - %d]\n", m_nRank, 1, MAX_WORKERS);
        } else {
            if (m_stConfigValues == NULL)
                printf("<%d> malone_isReady - Error loading config structure\n", m_nRank);
            if (m_stEnvValues == NULL)
                printf("<%d> malone_isReady - Error loading environment structure\n", m_nRank);
        }
    } else {
        if (m_strConfig != NULL)
            nRet = 1;
    }

    printf("<%d> malone_isReady - Code res %d\n", m_nRank, nRet);

    return nRet;
}

int malone_initialize_debug_mode()
{
    int nRet, nTCs;

    printf("malone_initialize_debug_mode - Init\n");
    nTCs = 0;
    nRet = 1;
    m_lInitTick = getTick();
    m_lInitializeInitTick = getTick();

    initialize_auxiliars();
    initializeMPI();
    loadConfig();
    
    m_lInitializeEndTick = getTick();

    //The main difference between this method and the previous one, is the existence of a environment variable
    if(m_strEnvironment != NULL)
    {
        printf("malone_initialize_debug_mode - Loading the environment from file\n");
        nRet = malone_load_environment_debug();
    }
    else
        printf("Environment file is empty ... %s", m_strEnvironment);
    
    m_lInitializeEndTick = getTick();

    printf("malone_initialize_debug_mode - End\n");
    
    return nRet;
}
int malone_initialize() {
    int nRet, nTCs;

    nRet = nTCs = 0;
    m_lInitTick = getTick();
    m_lInitializeInitTick = getTick();

    initialize_auxiliars();
    initializeMPI();
    loadConfig();

    if (malone_load_environment_values(m_strEnvironment)) {
        if (malone_isReady()) {
            
            if (m_stEnvValues->nMonitorEnabled == 1) {
                if (m_nRank == MALONE_MASTER)
                    printf("<%d> Monitor enabled\n", m_nRank);
                                
                signal(SIGALRM, MonitorAlarmhandler);
                if (m_stEnvValues->nMonitorFrequency >= 0)
                {
                    alarm(1);
                    if (m_nRank == MALONE_MASTER)
                        printf("<%d> Monitor enabled, alarm clock each: %d\n", m_nRank, m_stEnvValues->nMonitorFrequency);
                }
                else
                {
                    if (m_nRank == MALONE_MASTER)
                        printf("<%d> Monitor disabled due to the invalid frequency%d\n", m_nRank, m_stEnvValues->nMonitorFrequency);
                }
            }
            if (m_nRedirectToDiskEnabled) {
                if (m_nRank == MALONE_MASTER)
                    printf("<%d> Redirecting stdout to %s\n", m_nRank, m_stConfigValues->strResultsFile);
                redirect_stdout(m_stConfigValues->strResultsFile);
            }

            if (m_nRank == MALONE_MASTER) {
                printf("<%d> MALONE INITIALIZED: Total processes:%d\n", m_nRank, m_nSize);
                if (m_strEnvironment != NULL)
                    printf("<%d> Executing environment: %s\n", m_nRank, m_strEnvironment);
            }

            showDebugOptions();

            if (m_stEnvValues != NULL && m_stEnvValues->nStandalone == 1) {
                if (m_nRank == MALONE_MASTER)
                    printf("<%d> malone_initialize - Standalone!!\n", m_nRank);

                nTCs = readTestSuite();
                if (nTCs > 0) {
                    nRet = 1;
                    printf("<%d> malone_initialize - Test suite (%d tcs) readed sucessfully!!\n", m_nRank, nTCs);
                } else
                    printf("<%d> malone_initialize - Error reading the test suite\n", m_nRank);

                //Barrier
                //MPI_Barrier(MPI_COMM_WORLD);

                if (m_nRank == MALONE_MASTER) {
                    printf("<%d> malone_initialize - Final barrier completed! Lets get ready\n", m_nRank);
                }
            } else
                nRet = 1;
        } else
            printf("<%d> malone_initialize - There exist some problem related with the configuration!!\n", m_nRank);
    }
    m_lInitializeEndTick = getTick();

    return nRet;
}

void malone_free() {
    printf("<%d> free_Malone!!\n", m_nRank);
    
   
    if (m_nRank == MALONE_MASTER) {
        //End mpi
        MPI_Finalize();
        /*MPI_Abort(MPI_COMM_WORLD, 777);
        int i;
        i=i/0;
        //Dios, perdoname x esto. Lo arreglaré mientras analizo la escalabilidad del sistema
        printf("<%d> Aborting free_Malone!!\n", m_nRank);*/
    } else
        //End mpi
        MPI_Finalize();

    //Now trying to delete the shared memory
    free_auxiliars();
}

int malone_compile_original() {
    int nRet;
    char* strCompilationOriginal;
    if (MALONE_MASTER_LOG) printf("<%d> malone_compile - Init\n", m_nRank);

    //Initialise
    nRet = 0;
    m_lCompInitTick = getTick();
    if (m_nRank == MALONE_MASTER) {
        if (m_stEnvValues && m_stEnvValues->nCompilationEnabled) {
            printf("<%d> malone_compile_original - Compilation is enabled, compiling original\n", m_nRank);
            if (m_stEnvValues->strCompLineOriginal != NULL) {
                strCompilationOriginal = generateCompilationLine(0, BUILD_LINE_ORIGINAL_MODE);
                if (strCompilationOriginal != NULL) {
                    //redirects stderr to stdout below this line
                    dup2(1, 2);
                    printf("<%d> malone_compile_original - Compiling!\n", m_nRank);
                    nRet = 1;
                    execCommandLine("cd %s && %s 2>/dev/null", m_stEnvValues->strAppPath, strCompilationOriginal);
                }

                if (strCompilationOriginal != NULL)
                    free(strCompilationOriginal);
            } else
                nRet = 0;
        } else
            printf("<%d> malone_compile_original - Compilation mode is disabled\n", m_nRank);
    }
    m_lCompEndTick = getTick();

    if (MALONE_MASTER_LOG) printf("<%d> malone_compile_original - End\n", m_nRank);

    return nRet;
}

int malone_compile_mutants() {
    int nRet, nMutantInit, nMutantEnd, nMutants, nCompilingProcesses, nGrain, nStartingMutant;
    char* strCompilationMutants;
    if (MALONE_MASTER_LOG) printf("<%d> malone_compile_mutants - Init\n", m_nRank);

    //Initialise
    nRet = 1;
    m_lCompInitTick = getTick();
    if (m_stEnvValues && m_stEnvValues->nCompilationEnabled) {
        nStartingMutant = m_stEnvValues->nStartingMutant;
        nMutantInit = nStartingMutant;
        nMutantEnd = nStartingMutant - 1;
        if (m_stEnvValues->nParallelCompilation == 1) {
            //Calculate the total number of mutants to compile
            nMutants = m_stEnvValues->nTotalMutants;
            nCompilingProcesses = MIN(m_stEnvValues->nCompilationNumWorkers, m_nSize);

            printf("<%d> malone_compile_mutants - Compiling processes: %d\n", m_nRank, nCompilingProcesses);
            if (m_nRank <= nCompilingProcesses) {
                if (nCompilingProcesses > nMutants) {
                    nMutantInit = m_nRank;
                    nMutantEnd = m_nRank;
                } else {
                    //TODO: Mejorar esto
                    if (nCompilingProcesses > 0 && (nMutants - nStartingMutant) > 0)
                        nGrain = ceil((nMutants - nStartingMutant) / nCompilingProcesses);
                    else
                        nGrain = nMutants;

                    printf("<%d> malone_compile_mutants - Total grain: %d\n", m_nRank, nGrain);

                    if (m_nRank != 0) {
                        nMutantInit = nStartingMutant + (m_nRank * nGrain);

                        if ((m_nRank == m_nSize - 1) || nStartingMutant + (((m_nRank * nGrain) + nGrain) - 1) > nMutants)
                            nMutantEnd = nMutants;
                        else
                            nMutantEnd = nStartingMutant + (((m_nRank * nGrain) + nGrain) - 1);
                    } else {
                        nMutantInit = nStartingMutant;
                        if ((nStartingMutant + (nGrain - 1)) > nMutants)
                            nMutantEnd = nMutants;
                        else
                            nMutantEnd = nStartingMutant + (nGrain - 1);
                    }
                }
            }
        } else {
            if (m_nRank == MALONE_MASTER) {
                nMutantInit = m_stEnvValues->nStartingMutant;
                nMutantEnd = m_stEnvValues->nTotalMutants;
                printf("<%d>a[%d - %d]\n", m_nRank, nMutantInit, nMutantEnd);
            }
        }

        if (m_stEnvValues->strCompLineMutants != NULL) {
            if (nMutantInit <= nMutantEnd) {
                printf("<%d> malone_compile_mutants - Compilation is enabled, compiling mutants [%d - %d]\n", m_nRank, nMutantInit, nMutantEnd);
                for (int i = nMutantInit; i <= nMutantEnd; i++) {
                    printf("<%d> malone_compile_mutants - Compiling mutant :%d\n", m_nRank, i);
                    strCompilationMutants = generateCompilationLine(i, BUILD_LINE_MUTANT_MODE);
                    if (strCompilationMutants != NULL) {
                        printf("<%d> malone_compile_mutants - Executing line: %s\n", m_nRank, strCompilationMutants);
                        execCommandLine("cd %s && %s 2>/dev/null", m_stEnvValues->strMutantsPath, strCompilationMutants);
                    }
                }
            }
        } else
            nRet = 0;
    } else
        printf("<%d> malone_compile_mutants - Compilation mode is disabled\n", m_nRank);

    m_lCompEndTick = getTick();

    if (MALONE_MASTER_LOG) printf("<%d> malone_compile_mutants - End\n", m_nRank);

    return nRet;
}

void malone_generate_mutants() {

    if(m_nRank == MALONE_MASTER)
    {
        printf("<%d> Malone_generate_mutants - Init\n", m_nRank);

        m_lGenInitTick = getTick();
        if (m_stEnvValues && m_stEnvValues->nMutantGenerationEnabled) {
            if (m_stEnvValues->strGenerationMutantsLine != NULL) {
                execCommandLine("%s", m_stEnvValues->strGenerationMutantsLine);
            }
        } else
            printf("<%d> malone_compile - Compilation mode is disabled\n", m_nRank);

        m_lGenEndTick = getTick();
        printf("<%d> Malone_generate_mutants - End\n", m_nRank);
    }

}

int malone_execute_Original_Program() {
    int nRet;

    if (MALONE_MASTER_LOG) printf("<%d> malone_execute_Original_Program - Init\n", m_nRank);

    //Initialize
    m_lOriginalInitTick = getTick();
    nRet = 0;

    printf("<%d> malone_execute_Original_Program - Executing original program, selecting mode ... \n", m_nRank);
    if (m_stEnvValues->nDistributeOriginal == 0) {
        //Sequential mode
        printf("<%d> malone_execute - Executing in Sequential mode!\n", m_nRank);
        nRet = malone_execute_Original_Program_Sequential();
    } else {
        //Distributed mode.
        printf("<%d> malone_execute - Executing in Distributed mode!\n", m_nRank);
        nRet = malone_execute_Original_Program_Distributed();
    }

    m_lOriginalEndTick = getTick();

    return nRet;
}

int malone_sequential() {
    int nRet;

    //TODO

    return nRet;
}

int malone_execute_Original_Program_Distributed() {
    //Divide the number of mutants among the available workers!
    int nRet, nMutants, nTests, nSent, nIndex, nIndexMutant, nIndexTest, nAlive, nWorkerSource, nFinish, nRemainBlocks, nCounterAux, nErrorFound;
    T_stExecutionStructure exeVector[MAX_WORKERS];

    //Initialize
    nRet = 0;
    m_lOriginalInitTick = getTick();
    if (m_nRank == MALONE_MASTER) {
        if (MALONE_MASTER_LOG) printf("<%d> malone_execute_Original_Program_Distributed - Master mode!\n", m_nRank);

        //Initialize
        nMutants = m_stEnvValues->nTotalMutants;
        nTests = m_stEnvValues->nTotalTests;
        m_oTestExecMap.nMutants = nMutants;
        m_oTestExecMap.nTests = nTests;

        //Original program
        nIndexMutant = 0;
        nIndexTest = nCounterAux = 0;
        nSent = nErrorFound = 0;
        nWorkerSource = -1;

        //Starts with 1, due to its the first worker!!
        nIndex = 1;

        if (MALONE_MASTER_LOG) printf("<%d> malone_execute_Original_Program_Distributed - Starting at test: %d\n", m_nRank, nIndex);
        if (MALONE_MASTER_LOG) printf("<%d> malone_execute_Original_Program_Distributed - Distributing %d tests,  between %d workers\n", m_nRank, nTests, m_nSize - 1);

        //Fill the env values to send it to workers
        while (nIndex < m_nSize /*&& nIndex <= nMutants*/) {
            //Initialize            
            //  exeVector[nIndex].nExecutionMode = (int) eMethod;
            if (nIndex <= nTests) {
                exeVector[nIndex].nTestInit = nIndexTest;
                exeVector[nIndex].nTestEnd = nIndexTest;
                exeVector[nIndex].nMutantInit = 0;
                exeVector[nIndex].nMutantEnd = 0;
                nSent++;
            } else {
                exeVector[nIndex].nTestInit = -1;
                exeVector[nIndex].nTestEnd = -1;
                exeVector[nIndex].nMutantInit = -1;
                exeVector[nIndex].nMutantEnd = -1;
            }
            sendDeployMode(&exeVector[nIndex], nIndex);

            if(nIndexMutant >=0 && nIndexMutant<MAX_MUTANTS && nIndexTest>=0 && nIndexTest < MAX_TESTS)
                m_oTestExecMap.pMap[nIndexMutant][nIndexTest] = nIndex;
            //Inc counters
            nIndex++;
            nIndexTest++;
        }
        //Lets continue to distribute the rest of the workload: -1 due to the disposition of the next loop
        nIndexTest = nSent - 1;
        nRemainBlocks = nSent;

        //Wait for receiving values from all workers and filling final result
        do {
            if (nIndexTest < nTests) {
                if (MALONE_MASTER_LOG) printf("<%d> malone_execute_Original_Program_Distributed - Executing Test %d of %d\n", m_nRank, nIndexTest, nTests);
            } else {
                if (MALONE_MASTER_LOG) printf("<%d> malone_execute_Original_Program_Distributed - Receiving %d of %d\n", m_nRank, nIndexTest, nTests);
            }

            nAlive = receiveSingleTestOriginal((T_stExecutionStructure *)&exeVector, &nWorkerSource);
            nRemainBlocks--;
            
            if(nIndexMutant >=0 && nIndexMutant<MAX_MUTANTS && nIndexTest>=0 && nIndexTest < MAX_TESTS)
                m_oTestExecMap.pMap[nIndexMutant][nIndexTest] = nWorkerSource;

            if (hasOriginalProgramRemainingWork(nIndexTest)) {
                if (nAlive == 1) {
                    //Increment mutant and test counters
                    nIndexTest++;
                } else {
                    printf("<%d> malone_execute_Original_Program_Distributed - Original program has failed in test %d | [worker -  %d]\n", m_nRank, nIndexTest, nWorkerSource);

                    //TODO: Cancel the executions!!
                    nErrorFound = 1;
                }
            } else {
                if(nAlive ==1)
                {
                    printf("<%d> malone_execute_Original_Program_Distributed - The original program has been completed\n", m_nRank);
                    nIndexTest++;
                }
                else
                {
                    nErrorFound = 1;
                }
            }

            //If there is remaining test to distribute, send it!
            if (nIndexTest < nTests) {
                //Original program.
                exeVector[nWorkerSource].nMutantInit = 0;
                exeVector[nWorkerSource].nTestInit = nIndexTest;
                exeVector[nWorkerSource].nTestEnd = nIndexTest;
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);
                nRemainBlocks++;
            } else {
                //Worker has finished its work.
                if (MALONE_MASTER_LOG) printf("<%d> malone_execute_Original_Program_Distributed - Worker %d has finished!\n", m_nRank, nWorkerSource);
                if (MALONE_MASTER_LOG) printf("<%d> malone_execute_Original_Program_Distributed - Remaining blocks: %d\n", m_nRank, nRemainBlocks);
                exeVector[nWorkerSource].nMutantInit = -1;
                exeVector[nWorkerSource].nMutantEnd = -1;
                exeVector[nWorkerSource].nTestInit = -1;
                exeVector[nWorkerSource].nTestEnd = -1;
                sendDeployMode(&exeVector[nWorkerSource], nWorkerSource);
            }
        } while ((hasOriginalProgramRemainingWork(nIndexTest) || nRemainBlocks) && nErrorFound == 0);

        nRet = (nErrorFound == 0);
        if (MALONE_MASTER_LOG) printf("<%d> malone_execute_Original_Program_Distributed - Master finished!!\n", m_nRank);
    }

    m_lOriginalEndTick = getTick();
    if (MALONE_MASTER_LOG) printf("<%d> distribution_full_dynamic - End\n", m_nRank);

    return nRet;
}

int malone_execute_Original_Program_Sequential() {
    int nTotalTests;
    int nRet, nIndexTest, i, nProg, nMaxOriginalTimeout;
    char* strResult;
    char* exeLine;
    double dInit, dTime;
    printf("<%d> malone_execute_Original_Program_Sequential - Init\n", m_nRank);

    nRet = 1;
    nIndexTest = 0;
    nProg = -1;
    nTotalTests = m_stEnvValues->nTotalTests;
    printf("<%d> malone_execute_Original_Program_Sequential - Executing original program...\n", m_nRank);
    printf("<%d> malone_execute_Original_Program_Sequential - Total tests: %d\n", m_nRank, nTotalTests);

    m_lOriginalInitTick = getTick();

    while (nIndexTest < nTotalTests && nRet) {

        exeLine = (char*) buildExeLine(0, nIndexTest, BUILD_LINE_ORIGINAL_MODE);

        if (exeLine != NULL) {
            printf("<%d> malone_execute_Original_Program_Sequential - Executing test %d of %d - ", m_nRank, nIndexTest + 1, nTotalTests);
            dInit = getTick();
            nMaxOriginalTimeout = getOriginalTimeout();
            
            if(existsMarkerToken())
                strResult = (char*) execCommandLine("timeout %d %s | grep '%s'", nMaxOriginalTimeout, exeLine, getMarkerToken());
            else
                strResult = (char*) execCommandLine("timeout %d %s", nMaxOriginalTimeout, exeLine);

            dTime = getTick() - dInit;
            if (strResult != NULL && !strstr(strResult, "Error") && strstr(strResult, getMarkerToken())) {
                printf("%s", strResult);
                addResult(m_pResList, strResult, dTime, 0);
                nIndexTest++;
            } else {
                nRet = 0;
                if (strResult != NULL) {
                    printf("<%d> Error: %s\n", m_nRank, strResult);
                    printf("<%d> malone_execute_Original_Program_Sequential - Error: %s\n", m_nRank, strResult);
                } else {
                    printf("<%d> Error: NULL value!!!!\n", m_nRank);
                    printf("<%d> malone_execute_Original_Program_Sequential - NULL Error!!\n", m_nRank);
                }
            }
            if (FREE_MEM && strResult != NULL)
                free(strResult);
        } else {
            nRet = 0;
            printf("<%d> malone_execute_Original_Program_Sequential - Execution line of test %d is NULL!\n", m_nRank, nIndexTest + 1);
        }
    }

    if (nRet == 1)
        printf("<%d> malone_execute_Original_Program_Sequential - Original program OK!\n", m_nRank);
    else
        printf("<%d> malone_execute_Original_Program_Sequential - Original program NOT ok!!\n", m_nRank);

    m_lOriginalEndTick = getTick();
    printf("<%d> malone_execute_Original_Program_Sequential - End\n", m_nRank);

    return nRet;
}

int malone_notificate_mode(T_eExecutionMode eMethod) {
    int nRet, nMode;
    nMode = (int) eMethod;

    nRet = MPI_Bcast(&nMode, 1, MPI_INT, 0, MPI_COMM_WORLD);

    return nRet;
}

int malone_receive_mode() {
    T_eExecutionMode eMethod;
    int nMode;
    nMode = (int) eMethod;

    MPI_Bcast(&nMode, 1, MPI_INT, 0, MPI_COMM_WORLD);

    return nMode;
}

int malone_load_environment_values(char* strPath) {
    int nRet;

    if (MALONE_WORKER_LOG || m_nRank == MALONE_MASTER)
        printf("<%d> malone_load_environment_values - Init\n", m_nRank);

    if (strPath != NULL && strlen(strPath) > 0) {

        if (m_nRank == MALONE_MASTER) {
            if (m_stEnvValues == NULL) {
                printf("<%d> malone_load_environment_values - loading env file: %s\n", m_nRank, strPath);
                m_stEnvValues = (T_stIniValues*) loadEnvironmentFromIniFile(strPath);
            }

            if (m_stEnvValues != NULL) {
                printf("<%d> malone_load_environment_values - sending env file: %s\n", m_nRank, strPath);
                nRet = doSendEnvFile(strPath);
            } else {
                nRet = 0;
                printf("<%d> malone_load_environment_values - WARNING!! General error, the environment file is unable to be loaded\n", m_nRank);
            }
        } else {
            nRet = doSendEnvFile(strPath);
        }
    } else {
        printf("<%d> WARNING!! Env path is empty\n", m_nRank);
    }

    if (MALONE_WORKER_LOG || m_nRank == MALONE_MASTER)
        printf("<%d> malone_load_environment_values - End\n", m_nRank);

    return nRet;
}
/**
 * This methods selects the distribution algorithm, and starts the the mutation testing process using the selected algorithm. 
 * @param eMethod:  Enum that represents the distribution algorithm.
 * @return true if the distribution algorithm exists.
 */
int malone_distribute_workflow(T_eExecutionMode eMethod) {
    int nRet;

    m_lMutantInitTick = getTick();
    switch (eMethod) {
        case eStaticMutants:
            if (m_nRank == MALONE_MASTER)
                printf("<%d> Executing static mutant distribution\n", m_nRank);
            distribution_static_mutants(eMethod);
            break;
        case eStaticTests:
            //TODO: Este metodo no esta hecho! Tratar de hacerlo porque muy dificil no parece.
        case eDynamic:
            if (m_nRank == MALONE_MASTER)
                printf("<%d> Executing dynamic mutant distribution\n", m_nRank);
            distribution_dynamic_mutants(eMethod);
            break;
        case eFullDynamic:
            if (m_nRank == MALONE_MASTER)
                printf("<%d> Executing embarrasingly mutant distribution\n", m_nRank);

            if (m_stEnvValues->nScatterWorkload == 0)
                distribution_full_dynamic(eMethod);
            else
                distribution_full_dynamic_scatter(eMethod);
            break;
        case eAdaptiveGrain:
            distribution_adaptive_grain_scatter(eMethod);
            break;

        default:
            printf("<%d> malone_distribute_workflow - ERROR!!! The selected distribution mode does not exists\n", m_nRank);
            nRet = 0;
            break;
    }

    m_lMutantEndTick = getTick();
    return nRet;
}

/**
 * In this function, the worker processes execute the mutants given by the input structure T_stExecutionStructure.
 * @param pExeMode Scheme of the execution mode, 
 * @return 
 */
int malone_execute_mutants_by_scheme(T_stExecutionStructure* pExeMode) {
    int nRet, nIndexTest, i, nTotalTests, nReorderTest, nTotalMutants, nIndexMutant;
    char* strResult;
    char* exeLine;
    double dInit, dTime, dOriginalTime;
    int nMutantInit, nMutantEnd, nTestInit, nTestEnd, nIsNull;
    T_stTestInfo* pTest;

    if (MALONE_WORKER_LOG) printf("<%d> malone_execute_mutants_by_scheme - Init, mode: %d\n", m_nRank, pExeMode->nExecutionMode);
    if (pExeMode) {
        nRet = 1;
        nIndexTest = nReorderTest = 0;

        nMutantInit = pExeMode->nMutantInit;
        nMutantEnd = pExeMode->nMutantEnd;
        nTestInit = pExeMode->nTestInit;
        nTestEnd = pExeMode->nTestEnd;

        nIndexMutant = nMutantInit;

        nTotalTests = (nTestEnd - nTestInit) + 1;
        nTotalMutants = nMutantEnd - nMutantInit + 1;

        if (MALONE_WORKER_LOG) {
            printf("<%d> malone_execute_mutants_by_scheme - Executing mutants with scheme <%d>\n", m_nRank, pExeMode->nExecutionMode);
            printf("<%d> malone_execute_mutants_by_scheme - Total mutants: %d - [%d - %d]\n", m_nRank, nTotalMutants, nMutantInit, nMutantEnd);
            printf("<%d> malone_execute_mutants_by_scheme - Total tests: %d - [%d - %d]\n", m_nRank, nTotalTests, nTestInit, nTestEnd);
        }

        nIndexMutant = 0;

        //Mutant index starts in 1, because mutant 0 is the original app
        for (nIndexMutant = nMutantInit; (nIndexMutant <= nMutantEnd) && nRet; nIndexMutant++) {
            //Reset control 
            nIndexTest = nTestInit;
            nTotalTests = nTestEnd;
            nRet = 1;
            nIsNull = 0;

            allocMutant(nIndexMutant);
            if (MALONE_WORKER_LOG) printf("<%d> M.E. [MUT] - Executing mutant %d of %d\n", m_nRank, nIndexMutant, nMutantEnd);

            if (pExeMode->nExecutionMode == 4 || pExeMode->nExecutionMode == 5)
                nTotalTests++;


            while (nIndexTest < nTotalTests && nRet) {

                if (m_stEnvValues->nSortTestSuite && m_nSelAlgorithm == 5) {
                    nReorderTest = m_oSortedIndexTest[nIndexTest];
                } else {
                    nReorderTest = nIndexTest;
                }
                exeLine = (char*) buildExeLine(nIndexMutant, nReorderTest, BUILD_LINE_MUTANT_MODE);
                if (exeLine != NULL) {
                    if (MALONE_WORKER_LOG) printf("<%d> M.E. [MUT] - Executing test %d (s:%d) of %d\n", m_nRank, nIndexTest, nReorderTest, nTotalTests);
                    dInit = getTick();

                    dOriginalTime = getOriginalTime(m_pResList, nReorderTest);

                    if (MALONE_WORKER_LOG) printf("<%d> Original test <%d> execution: %f\n", m_nRank, nReorderTest, dOriginalTime);
                    dOriginalTime = getMutantsTimeout(dOriginalTime);

                    //TODO: Think in changing this... HPCCommand
                    if(existsMarkerToken())
                        strResult = (char*) execCommandLine("cd %s \n timeout %f %s | grep '%s'", m_stEnvValues->strAppPath, dOriginalTime, exeLine, getMarkerToken());
                    else
                        strResult = (char*) execCommandLine("cd %s \n timeout %f %s", m_stEnvValues->strAppPath, dOriginalTime, exeLine);
                    
                    dTime = getTick() - dInit;

                    if (strResult != NULL && !strstr(strResult, "Error") && !strstr(strResult, "ERROR") && strstr(strResult, getMarkerToken())) {
                        //addResult_t(&pTestsList, strResult, dTime);
                        nRet = 1;
                    } else {
                        nRet = 0;
                        if (strResult != NULL) {
                            if (MALONE_WORKER_LOG) printf("<%d> M.E. [MUT] - Error: %s\n", m_nRank, strResult);
                        } else {
                            if (MALONE_WORKER_LOG) {
                                printf("<%d> M.E. [MUT] - NULL ERROR!!\n", m_nRank);
                            }
                            if (strResult == NULL) {
                                strResult = malloc(4 * sizeof (char));
                                strcpy(strResult, "N!L\0");
                            }
                        }
                    }

                    //Pass
                    if (nRet) {
                        
                        pTest = (T_stTestInfo*) createTestTrap2(nReorderTest, strResult, dTime, 0);
                        if (pExeMode->nExecutionMode != 4) 
                        {
                            nRet = checkResult(nIndexTest, strResult);
                            pTest->nKill = (nRet==0);
                            if (nRet == 0 && MALONE_WORKER_LOG) printf("<%d> M.E. [MUT] - Test fail in TC: %d!\n", m_nRank, nReorderTest);
                        }
                    }
                    else //Fail
                    {
                        if (MALONE_WORKER_LOG) printf("<%d> M.E. [MUT] - Test fail!!\n", m_nRank);
                        pTest = (T_stTestInfo*) createTestTrap2(nReorderTest, strResult, dTime, 1);//TODO:pabloSays: esto que es?
                        nRet = 0;
                        printTest(pTest);
                    }
                    //Send test & Free memory
                    if (pExeMode->nExecutionMode == (T_eExecutionMode) eFullDynamic) {
                        sendTest(pTest, MALONE_MASTER);

                        if (FREE_MEM)
                            free(pTest);
                    } else {
                        //Insert test result                      
                        insertTestResult_unsorted(nIndexMutant, nReorderTest, pTest);
                    }
                    //In all cases, free the strResult variable
                    if (FREE_MEM && strResult != NULL) {
                        free(strResult);
                    }
                    nIndexTest++;
                }
                else
                {
                    printf("malone_execute_mutants_by_scheme - WARNING! Empty line!! Aborting mpi session\n");
                    MPI_Abort(MPI_COMM_WORLD,777);
                }
            }
        }

        //Free the mutants test results? -> After send it!
    }

    if (MALONE_WORKER_LOG) printf("<%d> malone_execute_mutants_by_scheme - End\n", m_nRank);

    return nRet;
}

int malone_execute_originalprogram_by_scheme(T_stExecutionStructure* pExeMode) {
    int nRet, nTotalTests, nTotalMutants, nIndexMutant;
    char* strResult;

    char* exeLine;
    double dInit, dTime, dOriginalTime;
    T_stTestInfo* pTest;
    int nMutantInit, nMutantEnd, nTestInit, nTestEnd, nTestIndex;

    if (MALONE_WORKER_LOG) printf("<%d> malone_execute_originalprogram_by_scheme - Init\n", m_nRank);
    if (pExeMode) {
        nRet = 1;
        nMutantInit = pExeMode->nMutantInit;
        nMutantEnd = pExeMode->nMutantEnd;
        nTestInit = pExeMode->nTestInit;
        nTestEnd = pExeMode->nTestEnd;
        nIndexMutant = nMutantInit;

        nTestIndex = nTestInit;
        nTotalTests = (nTestEnd - nTestInit) + 1;
        nTotalMutants = nMutantEnd - nMutantInit + 1;

        if (MALONE_WORKER_LOG) {
            printf("<%d> malone_execute_originalprogram_by_scheme - Executing tests with scheme <%d>\n", m_nRank, pExeMode->nExecutionMode);
            printf("<%d> malone_execute_originalprogram_by_scheme - Total tests: %d - [%d - %d]\n", m_nRank, nTotalTests, nTestInit, nTestEnd);
        }

        nIndexMutant = 0;

        nRet = 1;

        nTotalTests++;

        while (nTestIndex <= nTestEnd && nRet) {

            exeLine = (char*) buildExeLine(0, nTestIndex, BUILD_LINE_ORIGINAL_MODE);

            if (exeLine != NULL) {                
                if (MALONE_WORKER_LOG) printf("<%d> M.E. [OP] - Executing test %d of %d\n", m_nRank, nTestIndex, nTestEnd);
                dInit = getTick();

                dOriginalTime = getOriginalTime(m_pResList, nTestIndex);
                if (MALONE_WORKER_LOG) printf("<%d> Original test <%d> execution: %f\n", m_nRank, nTestIndex, dOriginalTime);
                dOriginalTime = getOriginalTimeout();

                if(existsMarkerToken())
                    strResult = (char*) execCommandLine("cd %s \n timeout %f %s | grep '%s'", m_stEnvValues->strAppPath, dOriginalTime, exeLine, getMarkerToken());
                else
                    strResult = (char*) execCommandLine("cd %s \n timeout %f %s", m_stEnvValues->strAppPath, dOriginalTime, exeLine);
                
                dTime = getTick() - dInit;

                if (strResult != NULL && !strstr(strResult, "Error") && !strstr(strResult, "ERROR") && strstr(strResult, getMarkerToken())) {
                    //addResult_t(&pTestsList, strResult, dTime);
                } else {
                    nRet = 0;
                    if (strResult != NULL) {
                        if (MALONE_WORKER_LOG)
                            printf("<%d> M.E. [OR] - Error: %s\n", m_nRank, strResult);
                    } else {
                        if (MALONE_WORKER_LOG)
                            printf("<%d> M.E. [OR] - NULL ERROR!!\n", m_nRank);
                        if (strResult == NULL) {
                            strResult = malloc(4 * sizeof (char));
                            strcpy(strResult, "N!L\0");
                            //strlcpy(strResult, "N!L\0", 4);                            
                        }
                    }
                }
            } else {
                nRet = 0;
                strResult = NULL;
            }

            //Pass
            if (nRet) {
                pTest = (T_stTestInfo*) createTestTrap2(nTestIndex, strResult, dTime, 0);
            }
            else //Fail
            {
                if (MALONE_WORKER_LOG) {
                    if (strResult != NULL)
                        printf("<%d> M.E. [OR] - Test fail [res: %s]!!\n", m_nRank, strResult);
                    else
                        printf("<%d> M.E. [OR] - Test fail [res: NULL]!!\n", m_nRank);
                }
                pTest = (T_stTestInfo*) createTestTrap2(nTestIndex, strResult, dTime, 1);
                nRet = 0;
                printTest(pTest);
            }

            nTestIndex++;
            sendTest(pTest, MALONE_MASTER);

            if (FREE_MEM) {
                if (strResult != NULL)
                    free(strResult);

                free(pTest);
            }
        }
    }
    if (MALONE_WORKER_LOG) printf("<%d> malone_execute_originalprogram_by_scheme - End\n", m_nRank);

    return nRet;
}

int malone_execute_mutants_single(int nMutantInit, int nMutantEnd) {
    int nRet, nIndexTest, i, nTotalTests, nTotalMutants, nIndexMutant;
    char* strResult;
    char* exeLine;
    long int lElapsed;
    double dInit, dTime, dOriginalTime;
    T_stTestInfo* pTest;

    if (MALONE_MASTER_LOG)printf("<%d> malone_execute_mutants_single - Init\n", m_nRank);

    nRet = 1;
    nIndexTest = 0;
    nTotalTests = m_stEnvValues->nTotalTests;
    nTotalMutants = m_stEnvValues->nTotalMutants;
    dOriginalTime = 0;

    if (MALONE_MASTER_LOG)printf("<%d> Executing original program...\n", m_nRank);
    if (MALONE_MASTER_LOG)printf("<%d> Total mutants: %d\n", m_nRank, nTotalMutants);
    if (MALONE_MASTER_LOG)printf("<%d> Total tests: %d\n", m_nRank, nTotalTests);

    if (MALONE_MASTER_LOG) printEnvFile(m_stEnvValues);
    nIndexMutant = 0;

    if (m_stEnvValues->nStartingMutant != 0) {
        nMutantInit = m_stEnvValues->nStartingMutant;
    }

    //Mutant index starts in 1, because mutant 0 is the original app
    for (nIndexMutant = nMutantInit; nIndexMutant <= nMutantEnd; nIndexMutant++) {
        //Alloc in memory the requested mutant
        nIndexTest = 0;
        nRet = 1;
        allocMutant(nIndexMutant);
        lElapsed = (long int) (getTick() - m_lTotalTime);
        if (MALONE_MASTER_LOG) printf("<%d> Executing mutant %d of %d | elapsed: %ld\n", m_nRank, nIndexMutant, nTotalMutants, lElapsed);
        while (nIndexTest < nTotalTests && nRet) {

            pTest = NULL;
            strResult = NULL;
            exeLine = (char*) buildExeLine(nIndexMutant, nIndexTest, BUILD_LINE_MUTANT_MODE);

            if (MALONE_WORKER_LOG) printf("<%d> Executing test %d of %d\n", m_nRank, nIndexTest + 1, nTotalTests);

            dInit = getTick();
            dOriginalTime = getOriginalTime(m_pResList, nIndexTest);

            if (MALONE_WORKER_LOG) printf("<%d> Original test <%d> execution: %f\n", m_nRank, nIndexTest, dOriginalTime);

            dOriginalTime = getMutantsTimeout(dOriginalTime);

            if(existsMarkerToken())
                strResult = (char*) execCommandLine("cd %s \n timeout %f %s | grep '%s'", m_stEnvValues->strAppPath, dOriginalTime, exeLine, getMarkerToken());
            else
                strResult = (char*) execCommandLine("cd %s \n timeout %f %s", m_stEnvValues->strAppPath, dOriginalTime, exeLine);
            
            dTime = getTick() - dInit;

            if (strResult != NULL && !strstr(strResult, "Error") && strstr(strResult, getMarkerToken())) {
                //addResult_t(&pTestsList, strResult, dTime);
            } else {
                nRet = 0;
                if (strResult != NULL) {
                    if (MALONE_WORKER_LOG)printf("<%d> Error: %s\n", m_nRank, strResult);
                } else {
                    if (MALONE_WORKER_LOG)printf("<%d> Error: NULL!!\n", m_nRank);
                }
            }
            //Pass
            if (checkResult(nIndexTest, strResult)) {
                //pTest = createTest(nIndexTest, strResult, dTime, 0);
                pTest = createTestTrap2(nIndexTest, strResult, dTime, 0);
                if (MALONE_WORKER_LOG)printf("<%d> malone_execute_mutants_single- Test OK\n", m_nRank);
                printTest(pTest);
            }
            else //Fail
            {
                if (MALONE_WORKER_LOG)printf("<%d> malone_execute_mutants_single - Test fail!!\n", m_nRank);
                //pTest = (T_stTestInfo*) createTest(nIndexTest, strResult, dTime, 1);
                pTest = createTestTrap2(nIndexTest, strResult, dTime, 0);
                nRet = 0;
                printTest(pTest);
            }
            if (!nRet)
                printf("<%d> malone_execute_mutants_single - Mutant %d has dead on test %d\n", m_nRank, nIndexMutant, nIndexTest);
            //Insert test result
            insertTestResult(nIndexMutant, nIndexTest, pTest);
            nIndexTest++;
            if (FREE_MEM && strResult != NULL)
                free(strResult);
        }
    }

    if (nRet == 1)
        printf("<%d> Original program OK!\n", m_nRank);
    else
        printf("<%d> Original program NOT ok!!\n", m_nRank);

    printf("<%d> malone_execute_mutants_single - End\n", m_nRank);

    return nRet;
}

int malone_distributed_mutation_testing(T_eExecutionMode eMethod) {
    int nRet;

    nRet = 0;

    printf("<%d> malone_distributed_mutation_testing - Init\n", m_nRank);

    if (m_nRank == MALONE_MASTER) {
        m_lTotalTime = getTick();

        //Master notificates the distribution mode
        malone_notificate_mode(eMethod);

        //Initialise the execution map
        createExecutionStructure();
        
        printf("<%d> malone_distributed_mutation_testing - Compiling original program\n", m_nRank);
        //Compile original
        malone_compile_original();

       
        printf("<%d> malone_distributed_mutation_testing - Executing original program\n", m_nRank);
        //Execute the original program
        if (malone_execute_Original_Program()) {
                                    
            printf("<%d> malone_distributed_mutation_testing - Sending original results\n", m_nRank);
            //TODO: El fulldynamic no necesita los resultados, que hacemos?
            //Send the results of the tests applied to original program
            //if(eMethod != eFullDynamic)
            sendOriginalTestResults(m_pResList);

            printf("<%d> malone_distributed_mutation_testing - Generating mutants\n", m_nRank);
            //Generate mutants
            malone_generate_mutants();
            
            //testEquivalenceChecking();
            printf("<%d> malone_distributed_mutation_testing - Compiling mutants\n", m_nRank);
            //Compile mutants
            malone_compile_mutants();
            
            //if is necessary to reorder the test suite
            if (m_stEnvValues->nSortTestSuite != 0) {
                m_lSortInitTick = getTick();
                printf("<%d> malone_distributed_mutation_testing - Sorting the TS\n", m_nRank);
                //Reorder test suite
                reorderTestSuite(&m_pResList);

                //Create index conversion
                createIndexVector(&m_pResList, m_oReorderIndexTest);
                createSortIndexVector(&m_pResList, m_oSortedIndexTest);
                printIndexVector(m_oReorderIndexTest, m_pResList->nElems);
                m_lSortEndTick = getTick();
            }

            //testEquivalenceChecking();
            if (m_stEnvValues->nSortTestSuite != 0 || m_stEnvValues->nClusterMutants != 0 || m_stEnvValues->nScatterWorkload != 0) {                
                createHeuristicStructures();
            }

            //If check equivalents
            if (m_stEnvValues->nClusterMutants != 0) {
                printf("<%d> Checking equivalences!!\n", m_nRank);
                checkMutantsEquivalence();
            }

            //Select a distribution mode!
            //TODO: Cambiar este nombre a algo más predecible malone_mutant_execution_distribute_workflow
            malone_distribute_workflow(eMethod);

            //testEquivalenceChecking();
            m_lEndTick = getTick();
            malone_save_results();
            nRet = 1;
        } else {
            printf("<%d> malone_distributed_mutation_testing - Original program has failed!!\n", m_nRank);
        }

    } else
        printf("<%d> malone_distributed_mutation_testing - Only for masters ;) ...\n!!", m_nRank);

    printf("<%d> malone_distributed_mutation_testing - End\n", m_nRank);

    //TODO: Temporal kill
    if (nRet == 0) {
        printf("<%d> Aborting execution, code 777\n", m_nRank);
        sleep(1);
        MPI_Abort(MPI_COMM_WORLD, 777);
    }
    return nRet;
}
//WORKERS
int malone_distributed_mutation_testing_workers() {
    int nRet;
    T_eExecutionMode eMethod;
    if (MALONE_WORKER_LOG) printf("<%d> malone_distributed_mutation_testing_workers - Init\n", m_nRank);

    //Wait for distribution method
    eMethod = (T_eExecutionMode) malone_receive_mode();

    if(eMethod != eSequential)
    {
        //If the user has selected distributed original program execution, lets do it!!
        if (m_stEnvValues != NULL && m_stEnvValues->nDistributeOriginal == 1) {
            if (MALONE_WORKER_LOG) printf("<%d> malone_distributed_mutation_testing_workers - Executing distributed original application test\n", m_nRank);
            malone_execute_Original_Program_Distributed_workers();
        }


        if (MALONE_WORKER_LOG) printf("<%d> malone_distributed_mutation_testing_workers - Receiving original test results\n", m_nRank);

        m_pResList = receiveOriginalTestResults_r();
        if (MALONE_WORKER_LOG) printTestResults(m_pResList);

        //Check if is enabled the parallel generation
        malone_generate_mutants();

        //Check if is enabled the parallel compilation
        malone_compile_mutants();

        //if is necessary to reorder the test suite
        if (m_stEnvValues->nSortTestSuite != 0 && m_nSelAlgorithm == 5) {
            //Reorder test suite
            reorderTestSuite(&m_pResList);

            //Create index conversion
            createIndexVector(&m_pResList, m_oReorderIndexTest);
            createSortIndexVector(&m_pResList, m_oSortedIndexTest);
            if (MALONE_WORKER_LOG) printIndexVector(m_oReorderIndexTest, m_pResList->nElems);
        }

            //Checking if equivalence is parallel
        if (m_stEnvValues->nClusterMutants != 0 && m_stEnvValues->nParallelMd5sum != 0 ) {
            if (MALONE_WORKER_LOG) printf("<%d> malone_distributed_mutation_testing_workers - Checking equivalence in parallel\n", m_nRank);
            checkMutantsEquivalence();
        }

        //Select a distribution mode!
        malone_distribute_workflow(eMethod);

        printf("<%d> malone_distributed_mutation_testing_workers - End\n", m_nRank);

        //Check if the monitor mode is enabled
        malone_check_Monitoring_Data();
    }
    else
        printf("<%d> malone_distributed_mutation_testing_workers - Sequential mode identified, exiting!\n", m_nRank);
    
    
    return nRet;
}

int malone_execute_Original_Program_Distributed_workers() {
    //Divide the number of mutants among the available workers!
    int nMutants, nTests, nSent, nIndex, nIndexMutant, nIndexTest, nAlive, nWorkerSource, nFinish, nRemainBlocks, nCounterAux, nErrorFound;
    int nRet;
    T_stExecutionStructure exeVector[MAX_WORKERS];
    T_stExecutionStructure* pExeRetMode;
    T_stTestInfo* pTest;

    nFinish = nRet = 0;
    if (MALONE_WORKER_LOG) printf("<%d> malone_execute_Original_Program_Distributed_workers - Workers mode!\n", m_nRank);

    //if(DEBUG)
    do {
        if (MALONE_WORKER_LOG) printf("<%d> malone_execute_Original_Program_Distributed_workers - Receiving execution deploymode!\n", m_nRank);
        //receive the deployment method
        pExeRetMode = receiveDeployMode();

        if (MALONE_WORKER_LOG) printf("<%d> malone_execute_Original_Program_Distributed_workers - Received deploy mode!\n", m_nRank);

        printDeployStruct(pExeRetMode);
        nFinish = (pExeRetMode == NULL || pExeRetMode->nTestInit == -1);
        //execute
        if (!nFinish) {
            if (MALONE_WORKER_LOG) printf("<%d> malone_execute_Original_Program_Distributed_workers - Executing tests\n", m_nRank);


            //executing mutants using the scheme received
            malone_execute_originalprogram_by_scheme(pExeRetMode);

            //send results to master!
            nIndexTest = pExeRetMode->nTestInit;
            nIndexMutant = pExeRetMode->nMutantInit;
            if (MALONE_WORKER_LOG) printf("<%d> malone_execute_Original_Program_Distributed_workers - Results already sent to master [T: %d]\n", m_nRank, nIndexTest);

        } else {
            if (MALONE_WORKER_LOG) printf("<%d> malone_execute_Original_Program_Distributed_workers - Worker <%d> finished!\n", m_nRank, m_nRank);
        }
    } while (!nFinish);

    nRet = nFinish;
    if (MALONE_WORKER_LOG) printf("<%d> malone_execute_Original_Program_Distributed_workers - End\n", m_nRank);

    return nRet;
}
void malone_check_Monitoring_Data()
{
    //First, check if the monitor is enabled!
    if(m_stEnvValues != NULL && m_stEnvValues->nMonitorEnabled==1)
    {
        if(m_nRank == MALONE_MASTER)
        {
            //Master work
            if (MALONE_MASTER_LOG) printf("<%d> malone_check_Monitoring_Data - Receiving monitoring data\n", m_nRank);
            
            //First, receive the once time records!!
            //TODO: Complete the code
        }
        else
        {
            //Workers work
            if (MALONE_WORKER_LOG) printf("<%d> malone_check_Monitoring_Data - Sending monitoring data\n", m_nRank);
            //TODO: Complete the code
        }
    }
}
int isEnabledMasterLog() {
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugMainMaster : 0;
}

int isEnabledWorkersLog() {
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugMainWorkers : 0;
}

/**
 * Save the results of the mutation testing process to disk:
 * - Time spent during the main phases: initialization, generation, compilation, original program and mutant execution
 * - Execution map
 */
int malone_save_results() {
    int nRet;

    nRet = 1;

    printf("<%d> malone_save_results() - Init\n", m_nRank);

    //Calculating the duration of each phase
    m_lInitializationTime = m_lInitializeEndTick - m_lInitializeInitTick;
    m_lGenerationTime = m_lGenEndTick - m_lGenInitTick;
    m_lCompTime = m_lCompEndTick - m_lCompInitTick;
    m_lEquivTime = m_lEquivEndTick - m_lEquivInitTick;
    m_lSortTime = m_lSortEndTick - m_lSortInitTick;
    m_lOriginalTime = m_lOriginalEndTick - m_lOriginalInitTick;
    m_lMutantsTime = m_lMutantEndTick - m_lMutantInitTick;
    m_lTotalTime = m_lEndTick - m_lInitTick;
    
    printf("<%d> malone_save_results() - Duration of mutation phases -  [Init: %ld | Gen:%ld | Comp: %ld | Equiv: %ld | Sort: %ld | Orig: %ld | Mut: %ld | Total: %ld]\n",
            m_nRank, m_lInitializationTime, m_lGenerationTime, m_lCompTime, m_lEquivTime, m_lSortTime, m_lOriginalTime, m_lMutantsTime, m_lTotalTime);
    //Print the results!            
    printResults(m_oMutantList.nElems, m_oMutantList.nDead, m_oTestExecMap.nEquivalents,m_oTestExecMap.nDupped,0, m_lCompTime,  m_lOriginalTime, m_lTotalTime);

    printf("<%d> Total worker processes: %d\n", m_nRank, m_nSize);

    printf("<%d> Check if the res folder [%s] exists\n", m_nRank, PATH_RES_DIR);

    if (generateResFolder()) {
        //Save the execution map!
        saveExecutionMap();
        //Save the time of the original program!
        saveOriginalProgramExecutionTimes();
        //Original program results (md5)
        saveOriginalProgramResults();
        //Save the execution times of the rest of mutants
        saveWholeExecutionTimesMap();
        //Tick Map
        saveMutationTestingTickMap();
        //copy the env file and config
        saveConfigAndEnvironmentFiles();
        //save overview
        saveOverview();
        //save equivalence
        saveEquivalenceList();
        //Save the results achieved during the mutation testing monitoring.
        saveMonitorResults();
        printf("<%d> malone_save_results() - End with code: %d\n", m_nRank, nRet);
    }

    return nRet;
}

int malone_load_environment_debug()
{
    int nRet, nTCs;
    
    nRet = nTCs = 0;
    
    if (malone_load_environment_values(m_strEnvironment)) {
        
        printf("DebugMode? %d", m_nDebugMode);
        if (1 || m_nDebugMode){// || isReadyToRumble()) {
            
            if (m_stEnvValues->nMonitorEnabled == 1) {
                if (m_nRank == MALONE_MASTER)
                    printf("<%d> Monitor enabled\n", m_nRank);
                                
                signal(SIGALRM, MonitorAlarmhandler);
                if (m_stEnvValues->nMonitorFrequency >= 0)
                {
                    alarm(1);
                    if (m_nRank == MALONE_MASTER)
                        printf("<%d> Monitor enabled, alarm clock each: %d\n", m_nRank, m_stEnvValues->nMonitorFrequency);
                }
                else
                {
                    if (m_nRank == MALONE_MASTER)
                        printf("<%d> Monitor disabled due to the invalid frequency%d\n", m_nRank, m_stEnvValues->nMonitorFrequency);
                }
            }
            if (m_nRedirectToDiskEnabled) {
                if (m_nRank == MALONE_MASTER)
                    printf("<%d> Redirecting stdout to %s\n", m_nRank, m_stConfigValues->strResultsFile);
                redirect_stdout(m_stConfigValues->strResultsFile);
            }

            if (m_nRank == MALONE_MASTER) {
                printf("<%d> MALONE INITIALIZED: Total processes:%d\n", m_nRank, m_nSize);
                if (m_strEnvironment != NULL)
                    printf("<%d> Executing environment: %s\n", m_nRank, m_strEnvironment);
            }

            showDebugOptions();

            if (m_stEnvValues != NULL && m_stEnvValues->nStandalone == 1) {
                if (m_nRank == MALONE_MASTER)
                    printf("<%d> malone_load_environment_debug - Standalone!!\n", m_nRank);

                nTCs = readTestSuite();
                if (nTCs > 0) {
                    nRet = 1;
                    printf("<%d> malone_load_environment_debug - Test suite (%d tcs) readed sucessfully!!\n", m_nRank, nTCs);
                } else
                    printf("<%d> malone_load_environment_debug - Error reading the test suite\n", m_nRank);

                //Barrier
                //MPI_Barrier(MPI_COMM_WORLD);

                if (m_nRank == MALONE_MASTER) {
                    printf("<%d> malone_load_environment_debug - Final barrier completed! Lets get ready\n", m_nRank);
                }
            } else
                nRet = 1;
        } else
            printf("<%d> malone_load_environment_debug - There exist some problem related with the configuration!!\n", m_nRank);
    }
}
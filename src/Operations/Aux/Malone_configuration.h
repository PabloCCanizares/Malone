/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_configuration.h
 *     Configuration and MALONE environmental files.
 *     MALONE 
 * @par Purpose:
 *     Provide the methods to load and charge the configuration and environment
 * files.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  3 September 2018
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <glib.h>
    
//#include "../../Malone_options.h"    
#include "Auxiliars.h"  
    
#include <stdio.h>
#include <stdlib.h>
    
#ifdef DISABLELOGS
#define DEBUG_CONFIG 0
#else
#define DEBUG_CONFIG isEnabledConfigLog()
#endif
    
typedef struct
{
    //Log
    int nDebugMainMaster;
    int nDebugMainWorkers;
    int nDebugMpiOps;    
    int nDebugAux;  
    int nDebugDistMaster;
    int nDebugDistWorkers;
    int nDebugDistCommon;    
    int nDebugDistUpdates;
    int nDebugPrinters;    
    int nDebugExecutionMode;
    int nDebugTrans;
    int nDebugMainCommand;    
    int nDebugConfig;    
    //Paths
    char* strResultsFile;
    
}T_stConfigValues;

typedef struct
{
    //General
    char* strFramework;
    char* strAppPath;
    char* strMutantsPath;
    char* strAppName;        
    char* strExecLineOriginal;
    char* strExecLineMutants;  
    char* strGenerationMutantsLine;
    int nTotalTests;
    int nTotalMutants;
    int nStartingMutant;
    
    //Optimizations
    int nDistributeOriginal;
    int nSortTestSuite;
    int nScatterWorkload;
    int nClusterMutants;
    int nParallelCompilation;
    int nParallelMd5sum;
    int nMultipleCoordinators;
    
    //StandAlone
    int nStandalone;
    char* strTestSuiteFile;
    
    //compilation
    int nCompilationEnabled;
    int nCompilationWithScript;
    int nCompilationNumWorkers;
    char* strCompScript;
    char* strCompLineOriginal;
    char* strCompLineMutants;
        
    
    //timeouts
    int nDebugMaxOriginalTimeout;
    int nDebugMaxMutantsTimeout;
    int nDebugMutantsMinimumTimeout;
    
    //Monitor mode
    int nMonitorEnabled;
    int nMonitorFrequency;
    char* strMonitorLines;
    char* strMonitorOnceLines;
    
    //Misc
    char* strMarkerToken;        
    int nMutantGenerationEnabled;
    int nRedirectToDiskEnabled;
    
}T_stIniValues;

//Monitor: In the future, generate its own class
typedef struct {
    int nAlarms;
    int nElems;    
    int nMax;
    char** array;
} MonitorLines;

//Configuration and environments paths
#define PATH_ENV_DIR "/Environments/"
#define PATH_CFG_FILE "Config.ini"
    
//Config defines
#define MALONE_DEBUG_MAIN_MASTER_LOG "MALONE_DEBUG_MAIN_MASTER"
#define MALONE_DEBUG_MAIN_WORKERS_LOG "MALONE_DEBUG_MAIN_WORKERS"
#define MALONE_DEBUG_MPI_OPS_LOG "MALONE_DEBUG_MPI_OPS"
#define MALONE_DEBUG_AUX_LOG "MALONE_DEBUG_AUX"
#define MALONE_DEBUG_DIST_COMMON_LOG "MALONE_DEBUG_DIST_COMMON"
#define MALONE_DEBUG_DIST_MASTER_LOG "MALONE_DEBUG_DIST_MASTER"
#define MALONE_DEBUG_DIST_WORKERS_LOG "MALONE_DEBUG_DIST_WORKERS"
#define MALONE_DEBUG_DIST_UPDATE_LOG "MALONE_DEBUG_DIST_UPDATE"
#define MALONE_DEBUG_TRANS_LOG "MALONE_DEBUG_TRANS"
#define MALONE_DEBUG_MAIN_COMMAND_LOG "MALONE_DEBUG_MAIN_COMMAND"
#define MALONE_DEBUG_PRINT_LOG "MALONE_DEBUG_PRINT"
#define MALONE_DEBUG_CONFIG_LOG "MALONE_DEBUG_CONFIG"
#define MALONE_REDIRECTION_FILE_LOG "RedirectionFile"

//Environments defines
#define MALONE_ENV_FRAMEWORK_PATH "FrameworkPath"
#define MALONE_ENV_APPLICATION_PATH "ApplicationPath"
#define MALONE_ENV_MUTANT_PATH "MutantPath"
#define MALONE_ENV_APP_NAME "ApplicationName"
#define MALONE_ENV_EXEC_LINE_ORIGINAL "ExecutionLineOriginal"
#define MALONE_ENV_EXEC_LINE_MUTANTS "ExecutionLineMutants"
#define MALONE_ENV_GENERATION_MUTANTS_LINE "GenerationLineMutants"

#define MALONE_ENV_TOTAL_TESTS "TotalTests"
#define MALONE_ENV_TOTAL_MUTANTS "TotalMutants"
#define MALONE_ENV_STARTING_MUTANT "StartingMutant"
#define MALONE_ENV_DISTRIBUTE_ORIGINAL "DistributeOriginal"
#define MALONE_ENV_SORT_TESTSUITE "SortTestSuite" 
#define MALONE_ENV_SCATTER_WORKLOAD "ScatterWorkload"
#define MALONE_ENV_CLUSTER_MUTANTS "ClusterMutants"
#define MALONE_ENV_PARALLEL_COMP "ParallelCompilation"
#define MALONE_ENV_MULT_COORDINATORS "MultipleCoordinators"
#define MALONE_ENV_STANDALONE "Standalone"
#define MALONE_ENV_TESTSUITE_FILE "TestSuiteFile"
#define MALONE_ENV_MAX_ORIGINAL_TIMEOUT  "MALONE_MAX_ORIGINAL_TIMEOUT" 
#define MALONE_ENV_MAX_MUTANTS_TIMEOUT_FACTOR "MALONE_MAX_MUTANTS_TIMEOUT_FACTOR" 
#define MALONE_ENV_MAX_MUTANTS_MINIMUM_TIME "MALONE_MAX_MUTANTS_MINIMUM_TIME"
#define MALONE_ENV_MARKER_TOKEN "MarkerToken" 
#define MALONE_ENV_MUTANT_GENERATION_ENABLED  "MutantGenerationEnabled"
#define MALONE_ENV_PARALLEL_MD5 "ParallelMd5sum"

#define MALONE_ENV_COMPILATION_LINE_ORIGINAL "CompilationLineOriginal"
#define MALONE_ENV_COMPILATION_WITH_SCRIPT "CompilationWithScript"
#define MALONE_ENV_COMPILATION_LINE_MUTANTS "CompilationLineMutants"
#define MALONE_ENV_COMPILATION_ENABLED "CompilationEnabled" 
#define MALONE_ENV_COMPILATION_NUM_WORKERS "CompilationNumWorkers"
#define MALONE_ENV_COMPILATION_SCRIPT "CompilationScript"

#define MALONE_ENV_MONITOR_ENABLED  "MonitorEnabled"
#define MALONE_ENV_MONITOR_LINES  "MonitorLines"
#define MALONE_ENV_MONITOR_FREQ  "MonitorFrequency"
#define MALONE_ENV_MONITOR_ONCE_LINES  "MonitorOnceLines"

//Monitor char lists
char** m_pListMonitorLines;
char** m_pListMonitorOnceLines;
MonitorLines m_oMonitorLines;
MonitorLines m_oMonitorOnceLines;
int m_nOnceLines;

T_stIniValues* m_stEnvValues;
T_stConfigValues* m_stConfigValues;
int m_nRankId;
char* m_strConfig;

//Config
void initialise_config();
void loadConfig();         //TODO: Cuidado que hay que actualizar el strConfig
int isEnabledConfigLog();

T_stConfigValues *loadConfigFromIni(char *strPath);
T_stIniValues *loadEnvironmentFromIniMemory(const char *pBuffer,int nSize);
T_stIniValues *loadEnvironmentFromIniFile(char *strPath);
T_stIniValues *loadEnvironmentKeys(GKeyFile *key_file);
void printEnvFile(T_stIniValues* pValues);

//free
void free_envfile(T_stIniValues* pEnvFile);
#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_H */


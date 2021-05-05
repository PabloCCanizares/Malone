/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_configuration.c
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

#include "Malone_configuration.h"

void free_envfile(T_stIniValues* pEnvFile)
{    
    if(pEnvFile)
    {
        if(pEnvFile->strAppName != NULL)
        {
            free(pEnvFile->strAppName);
            pEnvFile->strAppName = NULL;
        }
        if(pEnvFile->strAppPath != NULL)
        {
            free(pEnvFile->strAppPath);
            pEnvFile->strAppPath = NULL;
        }
        if(pEnvFile->strCompLineMutants != NULL)
        {
            free(pEnvFile->strCompLineMutants);
            pEnvFile->strCompLineMutants = NULL;
        }
        if(pEnvFile->strCompLineOriginal != NULL)
        {
            free(pEnvFile->strCompLineOriginal);
            pEnvFile->strCompLineOriginal = NULL;
        }
        
        //TODO: Falta aun, completar y unir a los tests!!
        free(pEnvFile);
        pEnvFile = NULL;
    }
}
void initialise_config()
{
    m_nRankId = -1;
    
    //TODO: Inicialise the structures
    
}
//Load the given configuration file
void loadConfig() {    
    char* strMaloneHome;
    int nLen;

    strMaloneHome = getenv("MALONE_HOME");
    if (DEBUG_CONFIG) printf("loadConfig - Malone environment variable: %s\n", strMaloneHome);
    nLen = strlen(strMaloneHome) + 1 + strlen(PATH_CFG_FILE);
    m_strConfig = (char*) malloc(nLen + 1);
    sprintf(m_strConfig, "%s/%s", strMaloneHome, PATH_CFG_FILE);
    if (DEBUG_CONFIG) printf("loadConfig - Config file is: %s\n", m_strConfig);

    m_stConfigValues = loadConfigFromIni(m_strConfig);
}

T_stConfigValues* loadConfigFromIni(char* strPath) {
    T_stConfigValues* pReturn;
    GKeyFile *key_file;
    GError *error;
    int nErrorFound;
    
    pReturn = NULL;
    nErrorFound = 0;
    
    if (DEBUG_CONFIG) printf("<%d>loadConfigFromIni - Init\n", m_nRankId);
    if (strPath != NULL) {
        error = NULL;
        key_file = g_key_file_new();
        if (!g_key_file_load_from_file(key_file,
                strPath,
                G_KEY_FILE_KEEP_COMMENTS |
                G_KEY_FILE_KEEP_TRANSLATIONS,
                &error)) {            
            printf("<%d>loadConfigFromIni - Error: %s\n", m_nRankId, error->message);
        } else {
            pReturn = (T_stConfigValues*) malloc(sizeof (T_stConfigValues));
            
            //Initialize values
            pReturn->strResultsFile = NULL;                       
            pReturn->nDebugMainMaster = pReturn->nDebugMainWorkers = 0;
            pReturn->nDebugMpiOps = pReturn->nDebugAux = 0;
            pReturn->nDebugAux = pReturn->nDebugDistCommon = 0;
            pReturn->nDebugDistMaster = pReturn->nDebugDistWorkers = 0;
            pReturn->nDebugDistUpdates = pReturn->nDebugTrans = 0;
                    
            pReturn->nDebugMainMaster = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_MAIN_MASTER_LOG,&error); 
            pReturn->nDebugMainWorkers = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_MAIN_WORKERS_LOG,&error); 
            pReturn->nDebugMpiOps = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_MPI_OPS_LOG,&error);
            pReturn->nDebugAux = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_AUX_LOG,&error); 
            pReturn->nDebugDistCommon = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_DIST_COMMON_LOG,&error); 
            pReturn->nDebugDistMaster = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_DIST_MASTER_LOG,&error); 
            pReturn->nDebugDistWorkers = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_DIST_WORKERS_LOG,&error); 
            pReturn->nDebugDistUpdates = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_DIST_UPDATE_LOG,&error); 
            pReturn->nDebugTrans = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_TRANS_LOG,&error); 
            pReturn->nDebugMainCommand = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_MAIN_COMMAND_LOG,&error); 
            pReturn->nDebugPrinters = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_PRINT_LOG,&error);  
            pReturn->nDebugConfig = g_key_file_get_integer(key_file,"logs",MALONE_DEBUG_CONFIG_LOG,&error);
            pReturn->strResultsFile = g_key_file_get_string(key_file, "misc", MALONE_REDIRECTION_FILE_LOG, &error);

            //=> TODO: Sacar de aqui, no queremos retroalimentación con AUX
            if (m_nRankId == 0 || DEBUG_CONFIG) {
                printf("<%d>INTERNAL CONFIGURATION: \n", m_nRankId);
                printf("<%d>Log:\n", m_nRankId);
                printf("<%d>MALONE_DEBUG_MAIN_MASTER_LOG: %d\n", m_nRankId, pReturn->nDebugMainMaster);
                printf("<%d>MALONE_DEBUG_MAIN_WORKERS_LOG: %d\n", m_nRankId, pReturn->nDebugMainWorkers);
                printf("<%d>MALONE_DEBUG_MPI_OPS_LOG: %d\n", m_nRankId, pReturn->nDebugMpiOps);
                printf("<%d>MALONE_DEBUG_AUX_LOG: %d\n", m_nRankId, pReturn->nDebugAux);
                printf("<%d>MALONE_DEBUG_DIST_COMMON_LOG: %d\n", m_nRankId, pReturn->nDebugDistCommon);
                printf("<%d>MALONE_DEBUG_DIST_MASTER_LOG: %d\n", m_nRankId, pReturn->nDebugDistMaster);
                printf("<%d>MALONE_DEBUG_DIST_WORKERS_LOG: %d\n", m_nRankId, pReturn->nDebugDistWorkers);
                printf("<%d>MALONE_DEBUG_PRINT_LOG: %d\n", m_nRankId, pReturn->nDebugPrinters);
                printf("<%d>MALONE_DEBUG_TRANS_LOG: %d\n", m_nRankId, pReturn->nDebugTrans);
                printf("<%d>MALONE_DEBUG_CONFIG_LOG: %d\n", m_nRankId, pReturn->nDebugConfig);
                printf("<%d>MALONE_DEBUG_MAIN_COMMAND_LOG: %d\n", m_nRankId, pReturn->nDebugMainCommand);
                printf("<%d>\n", m_nRankId);
                printf("<%d>Misc:\n", m_nRankId);
                printf("<%d>RedirectionFile: %s\n", m_nRankId, pReturn->strResultsFile);
                
                if (error != NULL)
                    printf("<%d>loadConfigFromIni - Error: %s\n", m_nRankId, error->message);
            }
        }
    } else {
        printf("<%d>loadConfigFromIni - Empty buffer", m_nRankId);
    }
    if (DEBUG_CONFIG) printf("<%d>loadConfigFromIni - End\n", m_nRankId);

    return pReturn;
}

T_stIniValues* loadEnvironmentFromIniFile(char* strPath) {
    T_stIniValues* pReturn;
    GKeyFile *key_file;
    GError *error;

    pReturn = NULL;
    
    if (strPath != NULL) {        
        if (DEBUG_CONFIG) printf("loadEnvironmentFromIniFile - Init | from %s\n", strPath);        
        error = NULL;
        key_file = g_key_file_new();
        if (!g_key_file_load_from_file(key_file,
                strPath,
                G_KEY_FILE_KEEP_COMMENTS |
                G_KEY_FILE_KEEP_TRANSLATIONS,
                &error)) {
            if (DEBUG_CONFIG)printf("loadEnvironmentFromIniFile - Error: %s\n", error->message);
        } else {            
            pReturn = loadEnvironmentKeys(key_file);
        }

    } else {
        printf("loadEnvironmentFromIniFile - Empty path!");
    }
    if (DEBUG_CONFIG) printf("loadEnvironmentFromIniFile - End\n");

    return pReturn;
}

/**
 * Load the .ini file getting the keys associated to the different values that comprises the file.
 */
T_stIniValues* loadEnvironmentKeys(GKeyFile *key_file) {
    T_stIniValues* pReturn;
    char *strFramework, *strAppPath, *strAppName, *strMutantsPath, *strCompScript, *strGenerationMutantsLine;
    char *strTestSuiteFile, *strMarkerToken, *strExecLineOriginal, *strExecLineMutants, *strCompLineOriginal, *strCompLineMutants;
    GError *error;
    int nErrorFound, nSize, nTotalTests, nTotalMutants, nStartingMutant, nDistributeOriginal, nSortTestSuite,
            nScatterWorkload, nClusterMutants, nStandalone, nParallelCompilation, nMultipleCoordinators,
            nDebugMaxOriginalTimeout, nDebugMaxMutantsTimeout, nDebugMutantsMinimumTimeout, nCompilationEnabled,
            nMutantGenerationEnabled, nCompilationWithScript, nCompilationNumWorkers;

    //Initialize integer variables
    nErrorFound = 0;
    nSize = nTotalTests = nTotalMutants = nStartingMutant = nDistributeOriginal = nSortTestSuite = 0;
    nScatterWorkload = nClusterMutants = nStandalone = nParallelCompilation = nMultipleCoordinators = 0;
    nDebugMaxOriginalTimeout = nDebugMaxMutantsTimeout = nDebugMutantsMinimumTimeout = nCompilationEnabled = 0;
    nMutantGenerationEnabled = nCompilationWithScript = nCompilationNumWorkers = 0;

    //Initialize char variables
    strFramework = strAppPath = strAppName = strCompLineOriginal = strCompLineMutants = strMutantsPath = NULL;
    strTestSuiteFile = strMarkerToken = strExecLineOriginal = strExecLineMutants = strCompScript = strGenerationMutantsLine = NULL;

    if (DEBUG_CONFIG) printf("loadEnvironmentKeys - Init\n");

    pReturn = (T_stIniValues*) malloc(sizeof (T_stIniValues));

    //General
    pReturn->strFramework = g_key_file_get_string(key_file, "general", MALONE_ENV_FRAMEWORK_PATH, &error); 
    pReturn->strAppPath = g_key_file_get_string(key_file, "general", MALONE_ENV_APPLICATION_PATH, &error); 
    pReturn->strMutantsPath = g_key_file_get_string(key_file, "general", MALONE_ENV_MUTANT_PATH, &error); 
    pReturn->strAppName = g_key_file_get_string(key_file, "general", MALONE_ENV_APP_NAME, &error); 
    pReturn->strExecLineOriginal = g_key_file_get_string(key_file, "general", MALONE_ENV_EXEC_LINE_ORIGINAL, &error);  
    pReturn->strExecLineMutants = g_key_file_get_string(key_file, "general", MALONE_ENV_EXEC_LINE_MUTANTS, &error);  
    pReturn->nTotalTests = g_key_file_get_integer(key_file, "general", MALONE_ENV_TOTAL_TESTS, &error); 
    pReturn->nTotalMutants = g_key_file_get_integer(key_file, "general", MALONE_ENV_TOTAL_MUTANTS, &error);
    pReturn->nStartingMutant = g_key_file_get_integer(key_file, "general", MALONE_ENV_STARTING_MUTANT, &error); 
    pReturn->strGenerationMutantsLine = g_key_file_get_string(key_file, "general", MALONE_ENV_GENERATION_MUTANTS_LINE, &error);  
    
    //Optimizations
    pReturn->nDistributeOriginal = g_key_file_get_integer(key_file, "optimizations", MALONE_ENV_DISTRIBUTE_ORIGINAL, &error); 
    pReturn->nSortTestSuite = g_key_file_get_integer(key_file, "optimizations", MALONE_ENV_SORT_TESTSUITE, &error); 
    pReturn->nScatterWorkload = g_key_file_get_integer(key_file, "optimizations", MALONE_ENV_SCATTER_WORKLOAD, &error); 
    pReturn->nClusterMutants = g_key_file_get_integer(key_file, "optimizations", MALONE_ENV_CLUSTER_MUTANTS, &error);
    pReturn->nParallelCompilation = g_key_file_get_integer(key_file, "optimizations", MALONE_ENV_PARALLEL_COMP, &error); 
    pReturn->nMultipleCoordinators = g_key_file_get_integer(key_file, "optimizations", MALONE_ENV_MULT_COORDINATORS, &error);  
    pReturn->nParallelMd5sum = g_key_file_get_integer(key_file, "optimizations", MALONE_ENV_PARALLEL_MD5, &error);  
    
    //Stand Alone mode
    pReturn->nStandalone = g_key_file_get_integer(key_file, "standalone", MALONE_ENV_STANDALONE, &error); 
    pReturn->strTestSuiteFile = g_key_file_get_string(key_file, "standalone", MALONE_ENV_TESTSUITE_FILE, &error); 
    
    //Compilation
    pReturn->nCompilationEnabled = g_key_file_get_integer(key_file, "compilation", MALONE_ENV_COMPILATION_ENABLED, &error);     
    pReturn->nCompilationWithScript = g_key_file_get_integer(key_file, "compilation", MALONE_ENV_COMPILATION_WITH_SCRIPT, &error); 
    pReturn->nCompilationNumWorkers = g_key_file_get_integer(key_file, "compilation", MALONE_ENV_COMPILATION_NUM_WORKERS, &error);     
    pReturn->strCompLineOriginal = g_key_file_get_string(key_file, "compilation", MALONE_ENV_COMPILATION_LINE_ORIGINAL, &error); 
    pReturn->strCompLineMutants = g_key_file_get_string(key_file, "compilation", MALONE_ENV_COMPILATION_LINE_MUTANTS, &error); 
    pReturn->strCompScript = g_key_file_get_string(key_file, "compilation", MALONE_ENV_COMPILATION_SCRIPT, &error); 
    
    //Timeouts
    pReturn->nDebugMaxOriginalTimeout = g_key_file_get_integer(key_file, "timeouts", MALONE_ENV_MAX_ORIGINAL_TIMEOUT, &error); 
    pReturn->nDebugMaxMutantsTimeout = g_key_file_get_integer(key_file, "timeouts", MALONE_ENV_MAX_MUTANTS_TIMEOUT_FACTOR, &error); 
    pReturn->nDebugMutantsMinimumTimeout = g_key_file_get_integer(key_file, "timeouts", MALONE_ENV_MAX_MUTANTS_MINIMUM_TIME, &error); 

    //Monitor mode
    pReturn->nMonitorEnabled = g_key_file_get_integer(key_file, "monitor", MALONE_ENV_MONITOR_ENABLED, &error);
    pReturn->nMonitorFrequency = g_key_file_get_integer(key_file, "monitor", MALONE_ENV_MONITOR_FREQ, &error);
    pReturn->strMonitorLines = g_key_file_get_string(key_file, "monitor", MALONE_ENV_MONITOR_LINES, &error); 
    pReturn->strMonitorOnceLines = g_key_file_get_string(key_file, "monitor", MALONE_ENV_MONITOR_ONCE_LINES, &error); 
    
    //Misc
    pReturn->strMarkerToken = g_key_file_get_string(key_file, "misc", MALONE_ENV_MARKER_TOKEN, &error);
    pReturn->nMutantGenerationEnabled = g_key_file_get_integer(key_file, "misc", MALONE_ENV_MUTANT_GENERATION_ENABLED, &error); 


    printf("loadEnvironmentKeys - Checking configuration errors\n");

    
    if (pReturn->strFramework == NULL)   nErrorFound++;
    if (pReturn->strAppPath == NULL)     nErrorFound++;
    if (pReturn->strMutantsPath == NULL) nErrorFound++;
    if (pReturn->strExecLineOriginal == NULL)    nErrorFound++;
    if (pReturn->strExecLineMutants == NULL)    nErrorFound++;
    if (pReturn->strMarkerToken == NULL) nErrorFound++;
    if (pReturn->strAppName == NULL)     nErrorFound++;

    if(pReturn->nMutantGenerationEnabled == 1 && pReturn->strGenerationMutantsLine == NULL)
    {
        nErrorFound++;
    }
    if (pReturn->nCompilationEnabled == 1 && pReturn->nCompilationWithScript == 0 &&
       (pReturn->strCompLineOriginal == NULL || pReturn->strCompLineMutants == NULL))
        nErrorFound++;  
    if(pReturn->nCompilationEnabled == 1 && pReturn->nCompilationWithScript == 1 && pReturn->strCompScript == NULL)
        nErrorFound++;              
    if (pReturn->nStandalone == 1 && (pReturn->strTestSuiteFile == NULL || strlen(pReturn->strTestSuiteFile) == 0))
        nErrorFound++;

    printf("loadEnvironmentKeys - Total errors found: %d\n", nErrorFound);

    
    if (DEBUG_CONFIG || m_nRankId == 0) {

        printf("<%d>----------\n", m_nRankId);
        printf("#General:\n");
        printf("Framework: %s\n", pReturn->strFramework);
        printf("AppPath: %s\n", pReturn->strAppPath);
        printf("MutantPath: %s\n", pReturn->strMutantsPath);
        printf("AppName: %s\n", pReturn->strAppName);
        printf("Compilation line original: %s\n", pReturn->strCompLineOriginal);
        printf("Compilation line mutants: %s\n", pReturn->strCompLineMutants);
        printf("Execution line original: %s\n", pReturn->strExecLineOriginal);
        printf("Execution line mutants: %s\n", pReturn->strExecLineMutants);
        printf("Total tests: %d\n", pReturn->nTotalTests);
        printf("Total mutants: %d\n", pReturn->nTotalMutants);
        printf("\n");
        printf("#Optimizations: \n");
        printf("DistributeOriginal: %d\n", pReturn->nDistributeOriginal);
        printf("SortTestSuite: %d\n", pReturn->nSortTestSuite);
        printf("ScatterWorkload: %d\n", pReturn->nScatterWorkload);
        printf("ClusterMutants: %d\n", pReturn->nClusterMutants);
        printf("ParallelCompilation: %d\n", pReturn->nParallelCompilation);
        printf("MultipleCoordinators: %d\n", pReturn->nMultipleCoordinators);
        printf("ParallelMd5: %d\n", pReturn->nParallelMd5sum);
        printf("\n");
        printf("#Standalone: \n");
        printf("Standalone: %d\n", pReturn->nStandalone);
        printf("Test suite file: %s\n", pReturn->strTestSuiteFile);
        printf("\n");
        printf("#Compilation: \n");
        printf("CompilationEnabled: %d\n", pReturn->nCompilationEnabled);
        printf("CompilationWithScript: %d\n", pReturn->nCompilationEnabled);
        printf("CompilationScript: %s\n", pReturn->strCompScript);
        
        printf("CompilationNumWorkers: %d\n", pReturn->nCompilationNumWorkers);
        printf("\n");        
        printf("#Timeouts: \n");
        printf("MALONE_MAX_ORIGINAL_TIMEOUT: %d\n", pReturn->nDebugMaxOriginalTimeout);
        printf("MALONE_MAX_MUTANTS_TIMEOUT_FACTOR: %d\n", pReturn->nDebugMaxMutantsTimeout);
        printf("MALONE_MAX_MUTANTS_MINIMUM_TIME: %d\n", pReturn->nDebugMutantsMinimumTimeout);
        printf("\n");
        printf("#Monitor: \n");
        printf("MALONE_ENV_MONITOR_ENABLED: %d\n", pReturn->nMonitorEnabled);
        printf("MALONE_ENV_MONITOR_FREQ: %d\n", pReturn->nMonitorFrequency);
        printf("MALONE_ENV_MONITOR_LINES: %s\n", pReturn->strMonitorLines);
        printf("MALONE_ENV_MONITOR_ONCE_LINES: %s\n", pReturn->strMonitorOnceLines);
        printf("\n");      
        printf("#Misc: \n");
        printf("MarkerToken: %s\n", pReturn->strMarkerToken);
        printf("MutantGenerationEnabled: %d\n", pReturn->nMutantGenerationEnabled);
        printf("----------\n");
        
        if(pReturn->strMonitorLines != NULL)
        {   
           if (DEBUG_CONFIG) printf("loadEnvironmentKeys - Analysing monitor lines\n");
           m_pListMonitorLines = str_split(pReturn->strMonitorLines, ',');

           printf("LEts invalidate the monitor lines temporally\n");
           if (NULL && m_pListMonitorLines)
           {
               int i;
               printf("1.1\n");
               
               for (i = 0; *(m_pListMonitorLines + i); i++)
               {
                   printf("commandLine=[%s]\n", *(m_pListMonitorLines + i));  
               }
               printf("\n");
           }
           
           if (DEBUG_CONFIG) printf("loadEnvironmentKeys - Analysing monitor line once!\n");
           m_pListMonitorOnceLines = str_split(pReturn->strMonitorOnceLines, ',');
           
           if (NULL && m_pListMonitorOnceLines)
            {
                int i;
                for (i = 0; *(m_pListMonitorOnceLines + i); i++)
                {
                    printf("commandLineOnce=[%s]\n", *(m_pListMonitorOnceLines + i));
                }
                printf("\n");
            }
        }
    }
    
    if (nErrorFound != 0) {
        pReturn = NULL;
        printf("loadEnvironmentKeys - Error found, some (%d) of the required values is empty, check the list!\n", nErrorFound);
    }

    if (DEBUG_CONFIG) printf("loadEnvironmentKeys - End\n");

    return pReturn;
}

T_stIniValues* loadEnvironmentFromIniMemory(const char* pBuffer, int nSize) {
    T_stIniValues* pReturn;
    GKeyFile *key_file;
    GError *error;


    pReturn = NULL;
    if (DEBUG_CONFIG) printf("loadFromIniMemory - Init\n");
    if (pBuffer != NULL) {
        error = NULL;
        pReturn = malloc(nSize);
        key_file = g_key_file_new();
        if (!g_key_file_load_from_data(key_file,
                pBuffer,
                nSize,
                G_KEY_FILE_KEEP_COMMENTS |
                G_KEY_FILE_KEEP_TRANSLATIONS,
                &error)) {
            printf("Error: %s", error->message);
        } else {
            pReturn = loadEnvironmentKeys(key_file);
        }

    } else {
        printf("loadFromIniMemory - Empty buffer");
    }
    if (DEBUG_CONFIG) printf("loadFromIniMemory - End\n");
    return pReturn;
}
int isEnabledConfigLog()
{   
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugConfig : 0;
}
//This function prints the values contained in an environment object
void printEnvFile(T_stIniValues* pValues)
{
    if(DEBUG_CONFIG)
    {
        printf("<%d>Printing environment file!!\n", m_nRank);
        if(pValues)
        {
            printf("----------\n");
            printf("Environment object:\n");
            printf("Framework: %s\n", pValues->strFramework);
            printf("AppPath: %s\n",pValues->strAppPath);
            printf("MutantPath: %s\n",pValues->strMutantsPath);
            printf("AppName: %s\n", pValues->strAppName);
            printf("Compilation line original: %s\n", pValues->strCompLineOriginal);
            printf("Compilation line mutants: %s\n", pValues->strCompLineMutants);
            printf("Execution line original: %s\n", pValues->strExecLineOriginal);        
            printf("Execution line mutants: %s\n", pValues->strExecLineMutants);        
            printf("Total tests: %d\n", pValues->nTotalTests);
            printf("Total mutants: %d\n", pValues->nTotalMutants);
            printf("Starting mutant: %d\n", pValues->nStartingMutant);
            printf("DistributeOriginal: %d\n", pValues->nDistributeOriginal);
            printf("Incomplete ...\n");
            printf("----------\n");
        }
        else
        {
            printf("<%d>WARNING, Empty environment file!!\n", m_nRank);
        }
    }    
}
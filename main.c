
/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file main.c
 *     Main program, exception handling and argument parser.
 * @par Purpose:
 *     Main program of MALONE.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     27 Oct 2015 - 11 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "Options.h"
#include "Malone.h"
#include "Auxiliars.h"
#include "executionMode.h"
#include "debugMode.h"

#define MAIN_DEBUG 1

//-----------------------------------
static void mySignalCatcher(int n);
void showMenu();
//-----------------------------------

static
void mySignalCatcher(int n) {
    fprintf(stderr, "<%d> Control-C or similar caught, quitting.\n", m_nRank);

    printf("<%d>  SIGNAL (%d) catched during the execution of the program", m_nRank, n);
    MPI_Abort(MPI_COMM_WORLD,33);
    malone_free();    
}

//TODO: Hora de borrarte amigo.
void writeLog(int nLogLevel, const char *fmt, ...) {
    /* va_list arg;
     gchar strCommand[10000];  
    
     va_start(arg, fmt);
     vsprintf(strCommand, fmt, arg);
     va_end(arg);
    
     (nLogLevel <= MALONE_GLOBAL_LOG) ? g_log ("Malone",G_LOG_LEVEL_MESSAGE, fmt, arg) : 0;
     * */
}

int parseArguments(int argc, char** argv) {
    int nOpt, nRet, nLen;
    char* strMaloneHome, *strEnvFile;

    nOpt = nRet = 0;

    if (MAIN_DEBUG) printf("parseArguments - Parsing arguments ...\n");

    while ((nOpt = getopt(argc, argv, "a:e:cdrz")) != -1) {
        switch (nOpt) {
            case 'a':
                m_nSelAlgorithm = atoi(optarg);
                break;
            case 'd':
                m_nDebugMode = 1;
                nRet =1;
                break;
            case 'c':
                m_nCompilationEnabled = 1;
                break;
            case 'r':
                m_nRedirectToDiskEnabled = 1;
                break;
            case 'e':
                strMaloneHome = getenv("MALONE_HOME");

                strEnvFile = optarg;
                if (strMaloneHome != NULL && strEnvFile != NULL) {
                    m_stEnvValues = NULL;
                    if (MAIN_DEBUG) printf("parseArguments - Malone environment variable: %s\n", strMaloneHome);
                    nLen = strlen(strMaloneHome) + strlen(PATH_ENV_DIR) + strlen(strEnvFile) + 1;
                    m_strEnvironment = malloc((nLen + 1) * sizeof (char));
                    sprintf(m_strEnvironment, "%s%s%s", strMaloneHome, PATH_ENV_DIR, strEnvFile);
                    if (MAIN_DEBUG) printf("parseArguments - Env file is: %s\n", m_strEnvironment);
                    
                    //Check if exists
                    nRet = file_exist(m_strEnvironment);
                            
                    if(nRet == 0)
                        printf("<%d> parseArguments - ERROR!! The selected environment file (.ini) does not exists!\n", m_nRank);

                } else {
                    printf("<%d> Malone environment variable is empty! Please insert $MALONE_HOME in your system\n", m_nRank);
                }
                break;
        }
    }
    return nRet;
}

/*
 * 
 */
int main(int argc, char** argv) {

    printf("MALONE: Manager for pAralleL mutatiON tEsting v1.1\n");

    //Configure signal catcher
    signal(SIGINT, mySignalCatcher);
    signal(SIGTERM, mySignalCatcher);
    signal(SIGHUP, mySignalCatcher);
    signal(SIGSEGV, mySignalCatcher);
    signal(SIGABRT, mySignalCatcher);
    
    showDebugOptions();
            
    if (parseArguments(argc, argv)) {
        
        //If autotest
        if(m_nDebugMode == 0)
        {
            //Initialise Malone
            if (malone_initialize()) {

                if (m_nRank == MALONE_MASTER)
                    printf("MALONE - Executing in normal mode\n");

                launchExecutionMode();           
            } else
                printf("MALONE - Problem whilst initializing in normal mode\n");                        
        }
        else
        {            
            //Initialise Malone in debug mode
            if (malone_initialize_debug_mode()) {
                
                if (m_nRank == MALONE_MASTER)
                    printf("MALONE - Executing in DEBUG mode\n");

                launchDebugMode();

            } else
                printf("MALONE - Problem whilst initializing in debug mode\n");
        }
            
        malone_free();
    } else {
        printf("MALONE - Invalid arguments!\n");

        printf("%d\n", argc);
        for (int i = 1; i < argc; i++) {
            printf("%s", argv[i]);
            
            //TODO: Hacer un menu que te enseñe las opciones
        }
        printf("-d      Debug mode\n");
        printf("-e <Environment_file>           Launches an specific environment\n");
        printf("-a <Distribution algorithm>     Uses an specific distirbution algorithm\n");
        printf("--autotest                      Automatically tests all the features of MALONE (future)\n");
    }

    return (EXIT_SUCCESS);
}


/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Options.h
 *     This file contains all the data structures necessaries to run MALONE. 
 * @par Purpose:
 *     Provide all the information necessary to configure and run MALONE.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     28 Oct 2015
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#ifndef OPTIONS_H
#define	OPTIONS_H

//#define DISABLELOGS
//#define PERFORMANCE_MODE
#define EXPERIMENTAL_MEM_SAFE

#define COMPATIBLE_MODE 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <assert.h>

//Server
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <glib.h>
#include <mpi.h>
#include "Vector.h"             //TODO: Posiblemente esto haya que quitarlo, era una prueba y no funciono bien.,
#include "configuration.h"


#define DEBUG 0
#define MAX_MUTANTS 85000       //A maximum of mutants, defined due to a structure limitation. 
#define MAX_MUTANTS_RED 100     //Maximum of reduced mutants, usually the mutant grain is lesser than this quantity. 
#define MAX_TESTS 1000          //Maximum size of tests. [Ojo, disminuyendo esto, no van algunas funcionalidades!!!!]
#define MAX_RESULT_SIZE 1000    //Maximum size of the text string (char*) resulting from executing a command. 
#define MAX_MD5_SIZE 100        //Maximum size of a MD5.
#define MAX_EXELINE 2000        //Maximum size of a command.
#define MAX_WORKERS 2048        //Maximum size of workers. 
#define RES_MUTANT_ALIVE -1     //Definition of mutant alive.
#define ALIVE 1                 //Definition of alive mutant.
#define DEAD 0                  //Definition of dead mutant.
//#define MAX_CONSEC_LINES 10     
//#define REQUIRED_ARGS 4

#define INITIAL_MONITOR_LINES 100       //Maximum lines of commands for monitoring the system. Let us remark that these commands are invoked several times during the execution
#define INITIAL_MONITOR_ONCE_LINES 100  //Maximum lines of commands for monitoring the system. Only invoked once time.
#define INITIAL_MAX_CLONES 10           //Maximum size of the initial set of clones. It is dynamically incremented by realloc in runtime.

#define MAX_ORIGINAL_TIMEOUT 3600       //Maximum execution time of a test case over the original program.
#define WORKER_TIME_MARGIN 25           //Maximum number of times that the execution of a mutant can be delayed with respect to the original program.
#define MINIMUM_WORKER_TIME_MARGIN 5    //Minimum number of times that the execution of a mutant can be delayed with respect to the original program.
#define DEFAULT_MARKER_TOKEN "#__#"     //Default token used to grep a result line.

#define LINE_SIZE 500                   //Maximum size of a test suite line.
#define TESTSUITE_SIZE 10000            //Maximum size of a test suite.

#define TOTAL_MUTANTS_ALIAS "[[NUMBER_MUTANT]]"     //Wildcard used to be replaced by the total number of mutants.
#define TOTAL_TESTS_ALIAS "[[TOTAL_TESTS]]"         //Wildcard used to be replaced by the total number of test cases.
#define INDEX_TEST_ALIAS "[[INDEX_TEST]]"           //Wildcard used to be replaced by a test case identifier.
#define INDEX_MUTANT_ALIAS "[[INDEX_MUTANT]]"       //Wildcard used to be replaced by a mutant identifier.
#define PATH_MUTANT_ALIAS "[[MUTANTS_PATH]]"        //Wildcard used to be replaced by the mutants path.
#define PATH_ORIGINAL_ALIAS "[[ORIGINAL_PATH]]"     //Wildcard used to be replaced by the original path.


//#define PATH_ENV_INI "/home/pablo/Frameworks/Milu/env_first.ini"

#define BUILD_LINE_ORIGINAL_MODE 1  //Definition to build a line for compiling the original program.
#define BUILD_LINE_MUTANT_MODE 0    //Definition to build a line for compiling a mutant.         

//TODO: Mover a ExecutionMapSaver
//Y cambiar el nombre del archivo a Malone_saver.c
#define PATH_RES_DIR "/Results/"                                        //Path for storing the results
#define EXECUTION_MAP_FILE "executionMap.txt"                           //Filename where the execution info is stored.
#define EXECUTION_TIMES_MAP_FILE "originalExecutionTimesMap.txt"        //Filename where the execution times of the original program are stored.
#define EXECUTION_RESULTS_MAP_FILE "originalResultsMap.txt"             //Filename where the results of the original program are stored.
#define EXECUTION_WHOLE_TIMES_MAP_FILE "totalExecutionTimesMap.txt"     //Filename where the all the execution time ticks are stored.
#define MUTATION_TESTING_TIMES_MAP_FILE "mutationTestingTicksMap.txt"   //Filename where the all the important ticks of the mutation testing process are stored.
#define MUTATION_TESTING_OVERVIEW "malone_overview.txt"                 //Filename where a overview of the mutation testing process is stored.
#define EQUIVALENCE_FILE "equivalence.txt"                              //Filename where the equivalence information is included.             
#define SAVE_CFG_FILE "Config.ini"                                      //Configuration filename.                
#define SAVE_ENV_FILE "Environment.ini"                                 //Default environment filename.        
#define MONITOR_FILE "monitor.txt"                                      //Default monitor filename.        

#define NORMAL_EQUIV_METHOD 1                                           //Identifier of the normal equivalent method
#define AGGRESIVE_EQUIV_METHOD 2                                        //Identifier of the aggressive equivalent method.

#define COMMAND_EXEC_HPC 1

//Log levels; 
//TODO: This enum status is not used at the present. Quitar
static gint MALONE_GLOBAL_LOG = 1;
static gint MALONE_MASTER = 0;
static gint LOG_ERROR = 1;
static gint LOG_INFO = 2;
static gint LOG_DEBUG = 3;
static gint LOG_VERBOSE = 4;

//This enum represents the 6 different distribution algorithms
typedef enum
{
    eSequential=0,      //Sequential mode
    eStaticMutants=1,   //Static mutants mode
    eStaticTests=2,     //Static tests mode:        
    eDynamic=3,         //PEDRO algorithm
    eFullDynamic=4,     //EMINENT
    eAdaptiveGrain=5,   //Adaptive grain
}T_eExecutionMode;

//TODO: This enum status is not used at the present.
typedef enum
{
    eIdle=1,
    eSendingEnvFile=2,            
    eExecuting=3,
}T_eStatus;

T_stConfigValues* m_stConfigValues;

//TODO: Esta parte se deberia mover a malone_testcase.h o algo asi
typedef struct T_stTestInfo{
    char res[MAX_RESULT_SIZE];  //Result of the test case execution
    double dTime;               //Execution time of the test
    double dInitTick;           //This time is ticked by the master
    double dEndTick;            //This time is ticked by the master
    int nKill;                  //Indicates if this test kills a mutant
    int nTest;                  //Id of the test case    
}T_stTestInfo;

struct TestInfo{
    char res[MAX_RESULT_SIZE];  //Result of the test case execution
    double dTime;               //Execution time of the test
    double dInitTick;           //This time is ticked by the master
    double dEndTick;            //This time is ticked by the master
    int nKill;                  //Indicates if this test kills a mutant
    int nTest;                  //Id of the test case    
};

typedef struct{
    char res[MAX_RESULT_SIZE]; 
    double dTime;
    int nKill;
    int nTest;
    int nMutant;       
}T_stTI;  //Reduced version

//TODO: Esta parte se deberia mover a malone_mutant.h o algo asi
typedef struct {
    int nNumber;
    int nState; //State of the mutant Dead=0, Alive =1
    int nTestKiller; //Number of test that kills the mutant
    int nTests;    
        
    double oTime[MAX_TESTS]; //Ojo, pensarnos si reducir esto a int solo
    int oKill[MAX_TESTS];
    int oTest[MAX_TESTS];    
} T_stM; // reduced version

typedef struct {
    int oMd5[MAX_TESTS];    
} T_stMutMd5; 

typedef struct
{
    int nEquivalent;
    int nIsClustered;
    int nManaged;
    int nFinished;
    int nIndexTest;
}T_stMutantExecution;

typedef struct
{
    int nIsEnabled;
    int nIsClusterHead;     
    int nMutantHeader;
    int nKillerTest;
    int nClones;
    
    int nMaxClones;
    int* pClones;
}T_stEquivalentInfo;

typedef struct
{
    int nRemainingMutants;
    int nCollapsed;
    int nIndexMutant;
    int nMutants;
    int nTests;
    int nEquivalents;
    int nDupped;
   
    char * oMd5Map[MAX_MUTANTS];    
    T_stEquivalentInfo* oEquivalentInfo[MAX_MUTANTS];
    T_stMutantExecution* oMutantMap[MAX_MUTANTS];
    int oMap[MAX_MUTANTS][MAX_TESTS];               // Indicates the worker that has executed an specific duple <mutant,test>

}T_stExecutionMap;


typedef struct
{
    int nElems;
    int nMax;
    T_stTestInfo* tests[MAX_TESTS];  
}T_stTestList;

#define NUM_DEPLOY_PARAMS 5
typedef struct
{
    int nExecutionMode; //Execution mode
    int nMutantInit;    //Mutant index init
    int nMutantEnd;     //Mutant index end
    int nTestInit;      //Test index init
    int nTestEnd;       //Test index end
    
    //These three last components are not sent to the worker!
    //This is not a problem, it is intended to do it for metric calculation
    int nWorker;
    double dInitTick;
    double dEndTick;
}T_stExecutionStructure;        //TODO: Dime donde se usa

typedef struct {
    int nNumber;
    int nState; //State of the mutant Dead=0, Alive =1
    int nTestKiller; //Number of test that kills the mutant
    char* strDescription; //Brief description: type of mutant, line of code ... 
    T_stTestList oTestList;
} T_stMutant;

typedef struct {
    int nElems;
    int nMax;
    int nDead;
    double dTime;
    T_stMutant* array[MAX_MUTANTS];     //TODO: En algun momento convertir esto en T_stMutant** array;
} MutantList;

T_stExecutionMap m_oTestExecMap;
T_stTI m_oTest[MAX_TESTS];   


#ifdef EXPERIMENTAL_MEM_SAFE        //This is the reduced version of a mutant, that is used to send it with datatypes. (The full version is MutantList)
T_stM m_oMutant[MAX_MUTANTS_RED];   //This change limits the static distribution, due to the maximum interval allowed is 100
#else
T_stM m_oMutant[MAX_MUTANTS];   
#endif

//DATATYPES
MPI_Datatype m_DeployType;
MPI_Datatype m_TestType;
MPI_Datatype m_MutantType;

int m_oReorderIndexTest[MAX_TESTS];
int m_oSortedIndexTest[MAX_TESTS];

T_stTestList* m_pResList;
T_stIniValues* m_stEnvValues;
T_eExecutionMode m_eExecutionMode;
MutantList m_oMutantList;
T_stTestList* m_pTestList;

typedef struct {
    int nAlarms;
    int nElems;    
    int nMax;
    char** array;
} MonitorLines;

//Monitor char lists
char** m_pListMonitorLines;
char** m_pListMonitorOnceLines;

MonitorLines m_oMonitorLines;
MonitorLines m_oMonitorOnceLines;
int m_nOnceLines;

//TODO: Mover a malone_timing.h
//Ticks
long int m_lInitTick;
long int m_lInitializeInitTick;
long int m_lInitializeEndTick;
long int m_lGenInitTick;
long int m_lGenEndTick;
long int m_lCompInitTick;
long int m_lCompEndTick;
long int m_lEquivInitTick;
long int m_lEquivEndTick;
long int m_lSortInitTick;
long int m_lSortEndTick;
long int m_lOriginalInitTick;
long int m_lOriginalEndTick;
long int m_lMutantInitTick;
long int m_lMutantEndTick;
long int m_lEndTick;
long int m_lInitializationTime;
long int m_lGenerationTime;
long int m_lCompTime;
long int m_lOriginalTime;
long int m_lMutantsTime;
long int m_lTotalTime;
long int m_lEquivTime;
long int m_lSortTime;

gint m_nRank;
gint m_nSize;

int m_nDebugMode;
int m_nSelAlgorithm;
int m_nCompilationEnabled;
int m_nRedirectToDiskEnabled;

char* m_strEnvironment;
char* m_strConfig;
char* m_strResFolder;

FILE* m_pp;

#endif	/* OPTIONS_H */

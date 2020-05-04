/* 
 * File:   Options.h
 * Author: Pablo C. Cañizares
 *
 * Created on October 28, 2015, 12:47 PM
 */

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
#include "Vector.h"
#include "configuration.h"


#define DEBUG 0
#define MAX_MUTANTS 85000
#define MAX_MUTANTS_RED 100
#define MAX_TESTS 1000         //Ojo, disminuyendo esto, no van algunas funcionalidades!!!!
#define MAX_RESULT_SIZE 1000
#define MAX_MD5_SIZE 100
#define MAX_EXELINE 2000
#define MAX_WORKERS 2048
#define RES_MUTANT_ALIVE -1
#define ALIVE 1
#define DEAD 0
#define MAX_CONSEC_LINES 10
#define REQUIRED_ARGS 4
#define INITIAL_MONITOR_LINES 100
#define INITIAL_MONITOR_ONCE_LINES 100
#define INITIAL_MAX_CLONES 10

#define MAX_ORIGINAL_TIMEOUT 3600
#define WORKER_TIME_MARGIN 25
#define MINIMUM_WORKER_TIME_MARGIN 5
#define DEFAULT_MARKER_TOKEN "#__#"

#define LINE_SIZE 500 /* can change the buffer size as well */
#define TESTSUITE_SIZE 10000 /* change to accomodate other sizes, change ONCE here */

#define TOTAL_MUTANTS_ALIAS "[[NUMBER_MUTANT]]"
#define TOTAL_TESTS_ALIAS "[[TOTAL_TESTS]]"
#define INDEX_TEST_ALIAS "[[INDEX_TEST]]"
#define INDEX_MUTANT_ALIAS "[[INDEX_MUTANT]]"
#define PATH_MUTANT_ALIAS "[[MUTANTS_PATH]]"
#define PATH_ORIGINAL_ALIAS "[[ORIGINAL_PATH]]"

#define PATH_ENV_INI "/home/pablo/Frameworks/Milu/env_first.ini"
#define PATH_RES_DIR "/Results/"

#define BUILD_LINE_ORIGINAL_MODE 1
#define BUILD_LINE_MUTANT_MODE 0

#define EXECUTION_MAP_FILE "executionMap.txt"
#define EXECUTION_TIMES_MAP_FILE "originalExecutionTimesMap.txt"
#define EXECUTION_RESULTS_MAP_FILE "originalResultsMap.txt"
#define EXECUTION_WHOLE_TIMES_MAP_FILE "totalExecutionTimesMap.txt"
#define MUTATION_TESTING_TIMES_MAP_FILE "mutationTestingTicksMap.txt"
#define MUTATION_TESTING_OVERVIEW "malone_overview.txt"
#define EQUIVALENCE_FILE "equivalence.txt"
#define SAVE_CFG_FILE "Config.ini"
#define SAVE_ENV_FILE "Environment.ini"
#define MONITOR_FILE "monitor.txt"

#define NORMAL_EQUIV_METHOD 1
#define AGGRESIVE_EQUIV_METHOD 2

#define COMMAND_EXEC_HPC 1

//Log levels;
static gint MALONE_GLOBAL_LOG = 1;
static gint MALONE_MASTER = 0;
static gint LOG_ERROR = 1;
static gint LOG_INFO = 2;
static gint LOG_DEBUG = 3;
static gint LOG_VERBOSE = 4;

typedef enum
{
    eSequential=0,
    eStaticMutants=1,
    eStaticTests=2,            
    eDynamic=3,
    eFullDynamic=4,   
    eAdaptiveGrain=5,
}T_eExecutionMode;

typedef enum
{
    eIdle=1,
    eSendingEnvFile=2,            
    eExecuting=3,
}T_eStatus;

T_stConfigValues* m_stConfigValues;

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
    int oMap[MAX_MUTANTS][MAX_TESTS];               // Indicates which worker has execute an specific duple <mutant,test>

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

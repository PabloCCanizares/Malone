/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_options.h
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

#ifndef MALONE_OPTIONS_H
#define	MALONE_OPTIONS_H

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

//#include "Operations/Aux/Malone_configuration.h"

#define INITIAL_MONITOR_LINES 100       //Maximum lines of commands for monitoring the system. Let us remark that these commands are invoked several times during the execution
#define INITIAL_MONITOR_ONCE_LINES 100  //Maximum lines of commands for monitoring the system. Only invoked once time.
#define INITIAL_MAX_CLONES 10           //Maximum size of the initial set of clones. It is dynamically incremented by realloc in runtime.

#define MAX_ORIGINAL_TIMEOUT 3600       //Maximum execution time of a test case over the original program.
#define WORKER_TIME_MARGIN 25           //Maximum number of times that the execution of a mutant can be delayed with respect to the original program.
#define MINIMUM_WORKER_TIME_MARGIN 5    //Minimum number of times that the execution of a mutant can be delayed with respect to the original program.
#define DEFAULT_MARKER_TOKEN "#__#"     //Default token used to grep a result line.

#define LINE_SIZE 1000                   //Maximum size of a test suite line.
#define TESTSUITE_SIZE 5000            //Maximum size of a test suite.

#define TOTAL_MUTANTS_ALIAS "[[NUMBER_MUTANT]]"     //Wildcard used to be replaced by the total number of mutants.
#define TOTAL_TESTS_ALIAS "[[TOTAL_TESTS]]"         //Wildcard used to be replaced by the total number of test cases.
#define INDEX_TEST_ALIAS "[[INDEX_TEST]]"           //Wildcard used to be replaced by a test case identifier.
#define INDEX_MUTANT_ALIAS "[[INDEX_MUTANT]]"       //Wildcard used to be replaced by a mutant identifier.
#define PATH_MUTANT_ALIAS "[[MUTANTS_PATH]]"        //Wildcard used to be replaced by the mutants path.
#define PATH_ORIGINAL_ALIAS "[[ORIGINAL_PATH]]"     //Wildcard used to be replaced by the original path.

#define BUILD_LINE_ORIGINAL_MODE 1  //Definition to build a line for compiling the original program.
#define BUILD_LINE_MUTANT_MODE 0    //Definition to build a line for compiling a mutant.         
 

#define NORMAL_EQUIV_METHOD 1                                           //Identifier of the normal equivalent method
#define AGGRESIVE_EQUIV_METHOD 2                                        //Identifier of the aggressive equivalent method.

#define COMMAND_EXEC_HPC 1
#define NUM_DEPLOY_PARAMS 5

#define MAX_COMMAND_SIZE 10000
static gint MALONE_MASTER = 0;


/*typedef enum
{
    eSequential=0,      //Sequential mode
    eStaticMutants=1,   //Static mutants mode
    eStaticTests=2,     //Static tests mode:        
    eDynamic=3,         //PEDRO algorithm
    eFullDynamic=4,     //EMINENT
    eAdaptiveGrain=5,   //Adaptive grain
}T_eExecutionMode;*/
//T_eExecutionMode m_eExecutionMode;

//DATATYPES
MPI_Datatype m_DeployType;          //Datatype used for the distribution algorithms, which allows to change the size of the grain (used for all the algorithms).
MPI_Datatype m_TestType;            //Datatype of test cases
MPI_Datatype m_MutantType;          //Datatype of mutants


gint m_nRank;                   //Id of the process in MPI
gint m_nSize;                   //Total processes involved in the MPI environment

int m_nDebugMode;               //Debug mode enabled.
int m_nAutoTest;                //Auto-test mode enabled.
int m_nSelAlgorithm;            //Distribution algorithm selected. 
int m_nCompilationEnabled;      //Shows if the compilation phase is enabled.
int m_nRedirectToDiskEnabled;   //Enables the redirection of the log to disk.

char* m_strEnvironment;         //Environment file to load.
//char* m_strConfig;              //Config file to load.
char* m_strResFolder;           //Path where the results of the MuT process will be stored.

FILE* m_pp;     //<- esto que es? [09/05/21] pabloSays: Sigo preguntandome que es.

#endif	/* MALONE_OPTIONS_H */

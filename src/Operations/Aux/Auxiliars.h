/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Auxiliars.h
 *     This file contains all the auxiliars methods necessaries to run MALONE. 
 * @par Purpose:
 *     Provide all the auxiliar methods of MALONE.
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

#ifndef AUXILIARS_H
#define AUXILIARS_H

#include "../../Malone_headers.h"
#include "../../Malone_options.h"
#include "../../Malone.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include "popen_noshell.h"
//#include "Malone_configuration.h"

#ifdef DISABLELOGS
#define DEBUG_AUX 0
#define MALONE_MAIN_COMMAND 0
#else
#define DEBUG_AUX isEnabledAuxLog()
#define MALONE_MAIN_COMMAND isEnabledDebugMainCommandLog()
#endif

int prettyPrint(int nIndex,int nTotal,int nAntProg);
int getOriginalTime(T_stTestList *pList,int nTest);
int checkResultOriginal(int nIndexTest,const char *strResult);
int checkTestResultOriginal(T_stTestInfo *pTest);
int testVector();
int readTestSuite();
int checkResult(int nIndexTest,const char *strResult);
int checkTestResult(T_stTestInfo *pTest);
void insertFullMutantInfo(int nMutant,int nMutantState,T_stTestList *pList);
int insertTestResult(int nMutant,int nTest,T_stTestInfo *pTest);
int insertTestResult_unsorted(int nMutant,int nTest,T_stTestInfo *pTest);
void insertMutant(T_stMutant *pMutant,int nIndexMutant);
void insertMutantTestByTest(T_stMutant* pMutant, int nIndexMutant, int nWorkerSource);
int allocMutant(int nIndexMutant);
int isMutantAllocated(int nIndexMutant);
char *buildExeLine_general(int nIndexMutant,int nIndexTest, int nOriginalMode);
char *buildExeLine_StandAlone(int nIndexMutant,int nIndexTest, int nOriginalMode);
void addResultIndex(T_stTestList *pTestList,int nIndex,const char *t,double dTime,int nKill);
void initializeEquivalentMap(int nMutants,int nTests);
void initializeExecutionMap(int nMutants,int nTests);
void deinitializeExecutionMap();
void initialize_auxiliars();

void free_auxiliars(); 
void free_test(T_stTestInfo* pTest);
void free_mutant(T_stMutant* pMutant);

int addResult(T_stTestList *pTestList,const char *t,double dTime,int nKill);
T_stTestInfo *createTest(int nIndexTest,char *strResult,double dTime,int nKill);
T_stTestInfo* copyTest(const T_stTestInfo* pTestIn);
char *buildExeLine(int nIndexMutant,int nIndexTest, int nOriginalMode);
char* buildEquivLine(int nIndexMutant, int nOriginalMode);
char* generateCompilationLine(int nIndexMutant, int nOriginalMode);
void showDebugOptions();
int isEnabledAuxLog();
int isEnabledDebugMainCommandLog();
int generateResFolder();
void saveConfigAndEnvironmentFiles();
MutantList* generateCleanMutantList(int nMutants);
void freeMutantList(MutantList* pList);
void free_mutant(T_stMutant* pMutant);
//void free_envfile(T_stIniValues* pEnvFile);

void createHeuristicStructures();
void createExecutionStructure();
void createTestRefP(int nIndexTest, char* strResult, double dTime, int nKill, T_stTestInfo** pTest);
char* getMarkerToken();
int existsMarkerToken();
double getOriginalTimeout();
double getMutantsTimeout(double dOriginalTime);

//Common
long int getTick();
int getenv_int(char *strKey);
char *replace(char const *const original,char const *const pattern,char const *const replacement);
char *replace_char(const char *s,char ch,const char *repl);
int file_exist (char *filename);
char* execCommandLine(const char *fmt, ...); 
char* execCommandLine_async(const char *fmt, ...);
char* execCommandLineHPC(const char *fmt, ...);
FILE *fopen_mkdir( char *path, char *mode );
void redirect_stdout(char *strFile);
void recursive_mkdir(char *path);
void copy(char *source, char *dest);
char** str_split(char* a_str, const char a_delim);
FILE * popen2(char* command, char* type, int * pid);
int pclose2(FILE * fp, pid_t pid);

#endif	// AUXILIARS_H


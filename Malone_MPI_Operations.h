/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file MPI_Operations.c
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

#ifndef MPI_OPERATIONS_H
#define	MPI_OPERATIONS_H

#include "Malone_headers.h"
#include "Malone_execution_info.h"

#ifdef __cplusplus
extern "C" {
#endif
    
//General
int isEnabledMpiOperations();
void initializeMPI();  

//DataTypes
void initializeMPI_Datatype();
void init_Datatype_mutant();
void init_Datatype_test();
void init_Datatype_deploy();

//Test operations    
T_stTestList *receiveOriginalTestResults_r();
T_stTestInfo *receiveTest(int nSource);
void receiveTestRefP(int nSource, T_stTestInfo** pTest);
void receiveOriginalTestResults(T_stTestList *pResList);
int sendOriginalTestResults(T_stTestList *pTestList);
int receiveSingleTestOriginal(T_stExecutionStructure pExeMode[MAX_WORKERS],int *pnWorkerSource);
int receiveSingleTestAndCheck(T_stExecutionStructure pExeMode[MAX_WORKERS],int *pnWorkerSource);
void sendTests(T_stExecutionStructure *pExeRetMode,int nDest);
void sendTest(T_stTestInfo *pTest,int nDest);

//Mutant operations
T_stMutant* receiveMutant(int nSource);
T_stMutant* receiveMutantAnySource(int* pnSource);
MutantList* receiveMutants_adaptive(T_stExecutionStructure pExeMode[MAX_WORKERS],int *pnWorkerSource);
int receiveMutants(T_stExecutionStructure pExeMode[MAX_WORKERS]);
void sendMutants(T_stExecutionStructure *pExeMode,int nDest);
void sendMutant(T_stMutant *pMutant,int nDest);
void resetMutant(T_stExecutionStructure* pExeMode);

//md5
int receiveMd5Mutants(T_stExecutionStructure pExeMode[MAX_WORKERS]);
void sendMd5Mutants(T_stExecutionStructure* pExeMode, char** strMd5List, int nTotalSizeInput, int nDest);

//Deploys
T_stExecutionStructure *receiveDeployMode();
void receiveDeployModeRefP(T_stExecutionStructure** exeMode);
void receiveDeployModeRef(T_stExecutionStructure* exeMode);
void sendDeployMode(T_stExecutionStructure *exeMode,int nDest);
         

#ifdef __cplusplus
}
#endif

#endif

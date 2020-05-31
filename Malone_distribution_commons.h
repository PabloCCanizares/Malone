/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_distribution_commons.h
 *     Header of the distribution algorithm commons.
 * @par Purpose:
 *    Provide the common methods to support the different distribution algorithms
 * of MALONE.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     15 Aug 2018
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef DISTRIBUTIONALGORITHMS_H
#define DISTRIBUTIONALGORITHMS_H

#include "Malone.h"
#include "Malone_options.h"
#include "Malone_headers.h"
#include "Malone_MPI_Operations.h"
#include "Malone_execution_info.h"
#include "Auxiliars.h"

#ifdef DISABLELOGS
#define MALONE_DEBUG_DIST_WORKERS 0
#define MALONE_DEBUG_DIST_MASTER 0  
#define MALONE_DEBUG_DIST_COMMON 0
#define MALONE_DEBUG_DIST_UPDATE 0
#else
#define MALONE_DEBUG_DIST_WORKERS isEnabledDistWorkersLog()
#define MALONE_DEBUG_DIST_MASTER isEnabledDistMasterLog()
#define MALONE_DEBUG_DIST_COMMON isEnabledDistCommonLog()
#define MALONE_DEBUG_DIST_UPDATE isEnabledDistUpdateLog()
#endif

#define SEARCH_INITIAL 5
#define SEARCH_SINGLEPHASE 2

int distribution_dynamic_mutants(T_eExecutionMode eMethod);
int distribution_static_mutants(T_eExecutionMode eMethod);
int distribution_adaptive_grain_scatter(T_eExecutionMode eMethod);
int distribution_full_dynamic_scatter(T_eExecutionMode eMethod);
int distribution_full_dynamic(T_eExecutionMode eMethod);

void incCounters(int *nIndexTest,int *nIndexMutant);
int updateCounters(T_stExecutionMap *exeMap,T_stExecutionStructure *exeWorker,int *nIndexTest,int *nIndexMutant);
int updateCounters_Equiv(T_stExecutionMap *exeMap,T_stExecutionStructure *exeWorker,int *nIndexTest,int *nIndexMutant);
int updateCounters_adaptive(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, int* nIndexTestInit, int* nIndexTestEnd, int* nIndexMutantInit, int* nIndexMutantEnd);
int updateCounters_adaptive_equiv_aggresive(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, int* nIndexTestInit, int* nIndexTestEnd, int* nIndexMutantInit, int* nIndexMutantEnd);
int checkEquivalence(T_stExecutionMap *exeMap,T_stExecutionStructure *exeWorker,int *nIndexTest,int *nIndexMutant);
void updateKillMutant(T_stExecutionMap *exeMap,T_stExecutionStructure *exeWorker);
void updateKillMutant_adaptive(T_stExecutionMap* exeMap, T_stExecutionStructure* exeWorker, MutantList* pMutantList);
int hasOriginalProgramRemainingWork(int nIndexTests);
int hasRemainingWork(int nIndexMutant,int nIndexTests);
int hasRemainingWork(int nIndexMutant,int nIndexTests);

int getNextWorkloadGrain_equiv(T_stExecutionMap** exeMap, T_stExecutionStructure** exeWorker, T_stMutantExecution** pMutantExec, int** nIndexTest, int** nIndexMutant);

int searchNextMutantNotProcessed(T_stExecutionMap* exeMap, int* nSearch, int* nIndex, T_stMutantExecution** pMutantExec);
int searchNextMutantNotProcessed_equiv_aggresive(T_stExecutionMap* exeMap, int* nSearch, int* nIndex, T_stMutantExecution** pMutantExec);

//Logs
int isEnabledDistWorkersLog();
int isEnabledDistMasterLog();
int isEnabledDistCommonLog();
int isEnabledDistUpdateLog();

int checkMutantsAlive(MutantList* pMutantList);
#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* DISTRIBUTIONALGORITHMS_H */
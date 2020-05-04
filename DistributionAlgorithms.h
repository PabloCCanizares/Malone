/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DistributionAlgorithms.h
 * Author: Pablo C. Cañizares
 *
 * Created on August 15, 2018, 12:54 PM
 */

#ifndef DISTRIBUTIONALGORITHMS_H
#define DISTRIBUTIONALGORITHMS_H

#include "Options.h"
#include "printers.h"
#include "MPI_Operations.h"
#include "Malone.h"
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
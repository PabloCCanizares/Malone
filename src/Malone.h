/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone.h
 *     Main functions of the framework, which provide the principal features of 
 *     MALONE 
 * @par Purpose:
 *     Provide the API of MALONE.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     25 Oct 2015 - 20 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef MALONE_H
#define	MALONE_H

#include "Malone_headers.h"
#include "Operations/Aux/Malone_configuration.h"
#include "Malone_options.h"
#include "DistributionAlgorithms/Malone_distribution_commons.h"
#include "common_headers/Malone_distributed_algorithm_types.h"

#include "Operations/Aux/Malone_saver.h"
#include "Operations/Aux/Auxiliars.h"
#include "Operations/Malone_MPI_Operations.h"
#include "Operations/Examples/tests.h"
#include "Operations/Aux/Malone_printers.h"
#include "Operations/Equivalence/Malone_equivalence.h"
#include "Operations/EnvFileSend.h"

int malone_initialize();
int malone_initialize_debug_mode();
int malone_execute_Original_Program_Distributed_workers();
int malone_distributed_mutation_testing_workers();
int malone_distributed_mutation_testing(T_eExecutionMode eMethod);
int malone_execute_mutants_single(int nMutantInit,int nMutantEnd);
int malone_execute_originalprogram_by_scheme(T_stExecutionStructure *pExeMode);
int malone_execute_mutants_by_scheme(T_stExecutionStructure *pExeMode);
int malone_distribute_workflow(T_eExecutionMode eMethod);
int malone_load_environment_debug();
int malone_load_environment_values(char *strPath);
int malone_receive_mode();
int malone_notificate_mode(T_eExecutionMode eMethod);
int malone_sequential();
int malone_execute_Original_Program_Distributed();
int malone_execute_Original_Program_Sequential();
int malone_execute_Original_Program();
int malone_compile_original();
int malone_compile_mutants();
int malone_save_results();
void malone_generate_mutants();
void malone_free();
void malone_check_Monitoring_Data();

void  MonitorAlarmhandler(int sig);
T_stTestInfo* createTestTrap2(int nIndexTest,char *strResult,double dTime,int nKill); //TODO: Mover a Auxiliars y cambiar nomrbe
int malone_isReady();
int isEnabledWorkersLog();
int isEnabledMasterLog();

#endif	/* MALONE_H */


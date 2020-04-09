/* 
 * File:   Malone.h
 * Author: Pablo C. Cañizares
 *
 * Created on October 28, 2015, 12:50 PM
 */

#ifndef MALONE_H
#define	MALONE_H

#include "Options.h"
#include "DistributionAlgorithms.h"

int malone_initialize();
int malone_execute_Original_Program_Distributed_workers();
int malone_distributed_mutation_testing_workers();
int malone_distributed_mutation_testing(T_eExecutionMode eMethod);
int malone_execute_mutants_single(int nMutantInit,int nMutantEnd);
int malone_execute_originalprogram_by_scheme(T_stExecutionStructure *pExeMode);
int malone_execute_mutants_by_scheme(T_stExecutionStructure *pExeMode);
int malone_distribute_workflow(T_eExecutionMode eMethod);
int malone_load_environment_values(char *strPath);
int malone_receive_mode();
int malone_notificate_mode(T_eExecutionMode eMethod);
int malone_sequential();
int malone_execute_Original_Program_Distributed();
int malone_execute_Original_Program_Sequential();
int malone_execute_Original_Program();
void malone_generate_mutants();
int malone_compile_original();
int malone_compile_mutants();
void malone_free();
int malone_save_results();
void malone_check_Monitoring_Data();

void  MonitorAlarmhandler(int sig);
double getOriginalTimeout();
double getMutantsTimeout(double dOriginalTime);
void createHeuristicStructures();   //TODO: Mover a Auxiliars
void initializeMPI();               //TODO: Mover a Auxiliars
T_stTestInfo *createTestTrap2(int nIndexTest,char *strResult,double dTime,int nKill); //TODO: Mover a Auxiliars y cambiar nomrbe
char* getMarkerToken();
int isReadyToRumble();
int isEnabledWorkersLog();
int isEnabledMasterLog();

#endif	/* MALONE_H */


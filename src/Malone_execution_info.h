/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_execution_info.h
 *     Information about the execution.
 * @par Purpose:
 *     This file provides the data structures necessaries to handle and store
 *     the MuT process using MALONE. 
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     28 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef MALONE_EXECUTION_INFO_H
#define MALONE_EXECUTION_INFO_H

#include "Malone_options.h"

#ifdef __cplusplus
extern "C" {
#endif

    
typedef struct
{
    int nEquivalent;        //Denotes if its equivalent
    int nIsClustered;       //Denotes if the mutant is a part of a cluster
    int nManaged;           //Denotes if it has been already managed
    int nFinished;          //Denotes if it has already been completely managed
    int nIndexTest;         //Shows the index of the test that kills this mutant
}T_stMutantExecution;

typedef struct
{
    int nIsEnabled;         //Shows if it mutant is enabled
    int nIsClusterHead;     //Shows if it is the representative mutant of a cluster    
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
    //int oMap[MAX_MUTANTS][MAX_TESTS];               // Indicates the worker that has executed an specific duple <mutant,test>
    int** pMap; 
}T_stExecutionMap;

T_stExecutionMap m_oTestExecMap;

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
}T_stExecutionStructure;        //TODO: Dime donde se usa, sendDeploy?



#ifdef __cplusplus
}
#endif

#endif /* MALONE_EXECUTION_INFO_H */


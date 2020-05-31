/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.                             //
/******************************************************************************/
/** @file Malone_testcases.h
 *     Data structures of the test cases handled in MALONE.
 * @par Purpose:
 *     The purpose is to provide the main structures to handle de testcases in 
 * MALONE
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

#ifndef MALONE_TESTCASES_H
#define MALONE_TESTCASES_H

#include "Malone_options.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct T_stTestInfo{
    char res[MAX_RESULT_SIZE];  //Result of the test case execution
    double dTime;               //Execution time of the test
    double dInitTick;           //This time is ticked by the master
    double dEndTick;            //This time is ticked by the master
    int nKill;                  //Indicates if this test kills a mutant
    int nTest;                  //Id of the test case    
}T_stTestInfo;

typedef struct{
    char res[MAX_RESULT_SIZE]; 
    double dTime;
    int nKill;
    int nTest;
    int nMutant;       
}T_stTI;  //Reduced version

typedef struct
{
    int nElems;
    int nMax;
    T_stTestInfo* tests[MAX_TESTS];  
}T_stTestList;

T_stTestList* m_pResList;               //Results of applying the test suite over the original program (only used by the Master process)
T_stTestList* m_pTestList;
T_stTI m_oRedTestList[MAX_TESTS];       //Reduced list of test cases (used for MPI_DATATYPES)
int m_oReorderIndexTest[MAX_TESTS];     //List used for reording the test suite following the strategie s2
int m_oSortedIndexTest[MAX_TESTS];


//TODO: Initialize tests
//free tests

#ifdef __cplusplus
}
#endif

#endif /* MALONE_TESTCASES_H */


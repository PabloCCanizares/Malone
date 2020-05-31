/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_printers.h
 *     Header of the printer functions.
 * @par Purpose:
 *     Print to screen some of the most important elements of MALONE, such as 
 *  tests, mutants, etc. It has been mainly included to debug tasks.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     23 Oct 2015 - 20 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef PRINTERS_H
#define PRINTERS_H

#include "Malone_options.h"
#include "Malone_execution_info.h"
#include "Malone_headers.h"

#ifdef __cplusplus
extern "C" {
#endif
    
void printTestResults(T_stTestList* pList);
void printTestResultsSort(T_stTestList* pList);
void printIndexVector(int  vector[], int nElems);
void printMutant(T_stMutant* pMutant);
void printMutantsInfo();
void printfMutantFull(T_stMutant* pMutant);
void printTest(T_stTestInfo* pTest);
void printTestRed(T_stTI* pTest);
void printResults(int nTotalMutants, int nDeadMutants, int nEquivalents, int nDupped, int nTotalTests, long int dCompTime, long int dOriginalTime, long int dTime);
void printResultsPartial();
void printDeployArray(int pIndex[], int nSize);
void printDeployStruct(T_stExecutionStructure* pDeploy);
void printEnvFile(T_stIniValues* pValues);
void printEquivInfo();
void printMutantEquiv(int nIndex);
int isEnabledPrinterLog();


#ifdef __cplusplus
}
#endif

#endif /* PRINTERS_H */


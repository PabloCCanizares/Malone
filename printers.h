/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   printers.h
 * Author: pablo
 *
 * Created on August 15, 2018, 12:54 PM
 */
#ifndef PRINTERS_H
#define PRINTERS_H

#include "Options.h"

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
int isEnabledPrinterLog();
#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* PRINTERS_H */


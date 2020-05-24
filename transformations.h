/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   printers.h
 * Author: Pablo C. Cañizares
 *
 * Created on August 16, 2018, 12:54 PM
 */


#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "Options.h"

T_stTestList* redtestList2TestList_r(T_stTI oTest[],int nItems);
void redtestList2TestList(T_stTI oTest[],T_stTestList *pList,int nItems);
void test2redTest(T_stTestInfo *pTest,T_stTI *pTestRed);
void test2redTest(T_stTestInfo *pTest,T_stTI *pTestRed);
void testList2redTestList(T_stTI *oTest,T_stTestList *pTestList);
void testList2redTestList(T_stTI *oTest,T_stTestList *pTestList);
T_stMutant *redMut2Mut(T_stM *pReducedMut);
T_stMutant *redMut2Mut(T_stM *pReducedMut);
T_stTestInfo *redtest2Test(T_stTI *pTestRed);

int isEnabledTransLog();
#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* TRANSFORMATIONS_H */

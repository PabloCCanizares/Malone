/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_transformations.h
 *     Header of transformation operations.
 *     MALONE 
 * @par Purpose:
 *     Provides the data structure and operations needed to perform the transformations
 * between tests/mutants and its reduced versions.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  27 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "../../Malone_options.h"
#include "../../Malone_headers.h"
#include "../Aux/Malone_printers.h"

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

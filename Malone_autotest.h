/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_autotest.h
 *     Header of the autotest.
 *     MALONE 
 * @par Purpose:
 *     Testing the different functionalities of MALONE. The test suite
 * starts with basic components, such as, just o name a few, command executor, datatypes, configuration,
 * and finishing with a combination of up to 60 distribution algorithms.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  26 Apr 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef AUTOTEST_H
#define AUTOTEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "Malone_options.h"
#include "test_command.h"
#include "test_envfile.h"
#include "test_conffile.h"    
#include "test_deploys.h"    
#include "test_testcases.h"    
#include "test_mutants.h"    
#include "test_dist_algorithm.h"    
#define TEST_DEBUG 1

typedef struct
{
    int nTotalTests;
    int nPass;
    int nFail;
    int nCategories;
}T_stAutoTests;

T_stAutoTests m_oAutoTests;

int main(int argc, char** argv);
static void mySignalCatcher(int n);
void autotest();
void autotest_print_results();
void autotest_initialise();
#ifdef __cplusplus
}
#endif

#endif /* AUTOTEST_H */


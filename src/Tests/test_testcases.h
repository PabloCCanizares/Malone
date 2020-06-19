/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file test_testcases.h
 *     Header of the test suite tests.
 *     MALONE 
 * @par Purpose:
 *     Testing the test case functionality.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  24 Apr 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef TEST_TESTCASES_H
#define TEST_TESTCASES_H

#include "Malone_autotest.h"
#include "../Malone_options.h"
#include "../Operations/Aux/Malone_printers.h"
#include "../Operations/Aux/randomElements.h"
#include "../Operations/Malone_MPI_Operations.h"
#include "../Operations/Aux/Auxiliars.h"

#ifdef __cplusplus
extern "C" {
#endif

void testTestcases();
void test_testcase_0();
void test_testcase_1();
void test_testcase_2();

#ifdef __cplusplus
}
#endif

#endif /* TEST_TESTCASES_H */


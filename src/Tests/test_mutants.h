/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file test_mutants.h
 *     Header of the mutants test suite.
 * @par Purpose:
 *     Testing the mutants functionality.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  10 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef TEST_MUTANTS_H
#define TEST_MUTANTS_H

#include "Malone_autotest.h"

#include "../Operations/Aux/Malone_printers.h"
#include "../Operations/Aux/randomElements.h"
#include "../Operations/Malone_MPI_Operations.h"
#include "../Operations/Aux/Auxiliars.h"

#ifdef __cplusplus
extern "C" {
#endif

void testMutants();
void test_mutants_1();

#ifdef __cplusplus
}
#endif

#endif /* TEST_MUTANTS_H */


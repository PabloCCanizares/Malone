/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file test_conffile.c
 *     Source code of the test suite for checking the usage of the configuration
 * file in MALONE.
 * @par Purpose:
 *     Testing the configuration file.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  28 Apr 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#ifndef TEST_COMMAND_H
#define TEST_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Malone_autotest.h"
#include "../Operations/Aux/popen_noshell.h"
    
void testCommand();
void test_command_1();
void test_command_2();
void test_command_3();   
void test_command_4();   
void test_command_5();   
void test_command_6();    

void test_command_waste();
void example_reading(int use_compat);

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMAND_H */


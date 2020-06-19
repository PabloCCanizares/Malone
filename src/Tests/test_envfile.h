/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file test_enfile.h
 *     Header of the environmental file test suite.
 * @par Purpose:
 *     Testing the environmental file functionality.
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
#ifndef TEST_ENVFILE_H
#define TEST_ENVFILE_H

#include "Malone_autotest.h"
#include "../Operations/Aux/Auxiliars.h"
#include "../Operations/Aux/Malone_configuration.h"

#ifdef __cplusplus
extern "C" {
#endif

void testEnvFile();
void test_envfile_1();
void test_envfile_2();        
void test_envfile_3();   
void test_envfile_4();
void test_envfile_5();
char* loadEnvFile_t(char* envValue);


#ifdef __cplusplus
}
#endif

#endif /* TEST_ENVFILE_H */


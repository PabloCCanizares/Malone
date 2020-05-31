/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file test_deploys.h
 *     Header of the test suite for checking the deployment of an execution 
 * structure used in the distributed algorithms in MALONE.
 * @par Purpose:
 *     Testing the execution structures.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  3 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef TEST_DEPLOYS_H
#define TEST_DEPLOYS_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "Malone_autotest.h"
    
void testDeploy();
void test_deploy_1();
void test_deploy_2();
void test_deploy_3();
void test_deploy_4();


#ifdef __cplusplus
}
#endif

#endif /* TEST_DEPLOYS_H */


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
 *  * @date  3 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "Malone_autotest.h"

void testConfFile()
{
    printf("testConfFile - Init\n");
    
    printf("testConfFile - End\n");
    m_oAutoTests.nPass++;
    m_oAutoTests.nCategories++;
}
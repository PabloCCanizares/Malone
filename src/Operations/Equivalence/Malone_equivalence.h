/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_equivalence.h
 *     Header of equivalence operations.
 * @par Purpose:
 *     This file provides the functionality to operate with equivalent mutants. 
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     28 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#ifndef EQUIVALENCECHECKING_H
#define	EQUIVALENCECHECKING_H

#include "../../Malone_headers.h"
#include "../Aux/Auxiliars.h"
#include "../Malone_MPI_Operations.h"
#include "Malone_md5Parallel.h"

#define EQUIVALENCE_PRINT 0
#define EQUIVALENCE_PRINT_DEBUG 0


void createClusters();
void checkMutantsEquivalence();
int areEquals(int nMutantSource,int nMutantDest);

#endif	/* EQUIVALENCECHECKING_H */
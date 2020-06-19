/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_md5Parallel.h
 *     Header of the parallel chechsum.
 * @par Purpose:
 *     Provide all the methods to conduct the parallel md5 to detect equivalent
 * mutants.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     3 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef MD5PARALLEL_H
#define MD5PARALLEL_H

#include "../../Malone_options.h"
#include "../Aux/Auxiliars.h"
#include "../../Malone.h"
#include "../Aux/Malone_printers.h"
#include "../Malone_MPI_Operations.h"
#include "Malone_equivalence.h"

#ifdef __cplusplus
extern "C" {
#endif

int testEquivalenceChecking();
int parallelEquivalenceChecking();


#ifdef __cplusplus
}
#endif

#endif /* MD5PARALLEL_H */


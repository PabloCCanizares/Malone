/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_distributed_algorithm_types.h
 *     Definition of the different distribution algorithm types.
 * @par Purpose:
 *     Provide the enum to select the different distibution algorithms.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     19 June 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef DISTRIBUTED_ALGORITHM_ENUM_H
#define DISTRIBUTED_ALGORITHM_ENUM_H

#ifdef __cplusplus
extern "C" {
#endif

//This enum represents the 6 different distribution algorithms
typedef enum
{
    eSequential=0,      //Sequential mode
    eStaticMutants=1,   //Static mutants mode
    eStaticTests=2,     //Static tests mode:        
    eDynamic=3,         //PEDRO algorithm
    eFullDynamic=4,     //EMINENT
    eAdaptiveGrain=5,   //Adaptive grain
}T_eExecutionMode;


#ifdef __cplusplus
}
#endif

#endif /* DISTRIBUTED_ALGORITHM_ENUM_H */


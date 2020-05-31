/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_timing.h
 *     Main ticks of MALONE.
 *     MALONE 
 * @par Purpose:
 *     Used for measure the timing of all the stages of the MuT performed by 
 *  MALONE.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  27 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#ifndef MALONE_TIMING_H
#define MALONE_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

//Ticks
long int m_lInitTick;
long int m_lInitializeInitTick;
long int m_lInitializeEndTick;
long int m_lGenInitTick;
long int m_lGenEndTick;
long int m_lCompInitTick;
long int m_lCompEndTick;
long int m_lEquivInitTick;
long int m_lEquivEndTick;
long int m_lSortInitTick;
long int m_lSortEndTick;
long int m_lOriginalInitTick;
long int m_lOriginalEndTick;
long int m_lMutantInitTick;
long int m_lMutantEndTick;
long int m_lEndTick;
long int m_lInitializationTime;
long int m_lGenerationTime;
long int m_lCompTime;
long int m_lOriginalTime;
long int m_lMutantsTime;
long int m_lTotalTime;
long int m_lEquivTime;
long int m_lSortTime;


#ifdef __cplusplus
}
#endif

#endif /* MALONE_TIMING_H */


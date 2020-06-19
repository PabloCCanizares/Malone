/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.                             //
/******************************************************************************/
/** @file Malone_saver.h
 *     Main functions to save the results of the MuT process.
 * @par Purpose:
 *     Save the results of the MuT process, such as MS, equivalence map, times,
 *  in different files.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date     25 Oct 2015 - 20 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef EXECUTIONMAPSAVER_H
#define EXECUTIONMAPSAVER_H

#include "../../Malone_options.h"
#include "../../Malone_headers.h"

//Y cambiar el nombre del archivo a Malone_saver.c
#define PATH_RES_DIR "/Results/"                                        //Path for storing the results
#define EXECUTION_MAP_FILE "executionMap.txt"                           //Filename where the execution info is stored.
#define EXECUTION_TIMES_MAP_FILE "originalExecutionTimesMap.txt"        //Filename where the execution times of the original program are stored.
#define EXECUTION_RESULTS_MAP_FILE "originalResultsMap.txt"             //Filename where the results of the original program are stored.
#define EXECUTION_WHOLE_TIMES_MAP_FILE "totalExecutionTimesMap.txt"     //Filename where the all the execution time ticks are stored.
#define MUTATION_TESTING_TIMES_MAP_FILE "mutationTestingTicksMap.txt"   //Filename where the all the important ticks of the mutation testing process are stored.
#define MUTATION_TESTING_OVERVIEW "malone_overview.txt"                 //Filename where a overview of the mutation testing process is stored.
#define EQUIVALENCE_FILE "equivalence.txt"                              //Filename where the equivalence information is included.             
#define SAVE_CFG_FILE "Config.ini"                                      //Configuration filename.                
#define SAVE_ENV_FILE "Environment.ini"                                 //Default environment filename.        
#define MONITOR_FILE "monitor.txt"                                      //Default monitor filename.       

void saveWholeExecutionTimesMap();
void saveOriginalProgramExecutionTimes();
void saveOriginalProgramResults();
void saveExecutionMap();
void saveMutationTestingTickMap();
void saveOverview();
void saveEquivalenceList();
void saveMonitorResults();


#endif	// EXECUTIONMAPSAVER_H


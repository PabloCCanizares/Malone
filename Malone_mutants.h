/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file Malone_mutants.h
 *     This file contains all the data structures necessaries to represent a mutant in MALONE. 
 * @par Purpose:
 *     Provide all the information necessary to handle mutants in MALONE.
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

#ifndef MALONE_MUTANT_H
#define MALONE_MUTANT_H

#include "Malone_options.h"

#ifdef __cplusplus
extern "C" {
#endif

//Struct that containts all the info neccesary to handle and represent a mutant in MALONE
typedef struct {
    int nNumber;        //Id of the mutant
    int nState;         //State of the mutant Dead=0, Alive =1
    int nTestKiller;    //Number of test that kills the mutant
    char* strDescription; //Brief description: type of mutant, line of code ... 
    T_stTestList oTestList; //List of tests associated to this mutant
} T_stMutant;

//Mutant List
typedef struct {
    int nElems;             //Number of mutants of the list        
    int nMax;               //Max number of elements of the list
    int nDead;              //Number of dead mutants
    double dTime;               
    T_stMutant* array[MAX_MUTANTS];     //TODO: En algun momento convertir esto en T_stMutant** array;
} MutantList;

//Reduced version of a mutant, used to send it through the communication networks using datatypes
typedef struct {
    int nNumber;        //Identifier of the mutant
    int nState;         //State of the mutant Dead=0, Alive =1
    int nTestKiller;    //Number of test that kills the mutant
    int nTests;    
        
    double oTime[MAX_TESTS]; //Ojo, pensarnos si reducir esto a int solo
    int oKill[MAX_TESTS];
    int oTest[MAX_TESTS];    
} T_stM; // reduced version

typedef struct {
    int oMd5[MAX_TESTS];    
} T_stMutMd5; 


MutantList m_oMutantList;       //Mutant list

#ifdef EXPERIMENTAL_MEM_SAFE        //This is the reduced version of a mutant, that is used to send it with datatypes. (The full version is MutantList)
T_stM m_oRedMutantList[MAX_MUTANTS_RED];   //This change limits the static distribution, due to the maximum interval allowed is 100
#else
T_stM m_oRedMutantList[MAX_MUTANTS];   
#endif

#ifdef __cplusplus
}
#endif

#endif /* MALONE_MUTANT_H */


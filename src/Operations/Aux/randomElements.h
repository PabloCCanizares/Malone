/* This file was automatically generated.  Do not edit! */
//TODO: Completar como un H convencional (resto del programa)
#include "../../Malone_options.h"
#include "Auxiliars.h"

T_stMutant *generateRandomMutant(int nTests);
void generateRandomTestList(T_stTestList *pTestList,int nTests);
T_stTestList *generateRandomTestList_r(int nTests);
void doingTraps();
T_stTestInfo *generateTestTrap(int nTest,char *strResult);
T_stTestInfo *createTestTrap(int nIndexTest,char *strResult,double dTime,int nKill);
T_stTestInfo *generateRandomTest(int nTest);

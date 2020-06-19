
#include "../../Malone_options.h"
#include "../../Malone_headers.h"
#include "../Malone_MPI_Operations.h"
#include "../Aux/randomElements.h"

#include "../Aux/Malone_printers.h"

/* This file was automatically generated.  Do not edit! */
void reorderTestSuite(T_stTestList **pResList);
void createSortIndexVector(T_stTestList **pResList,int vector[]);
void createIndexVector(T_stTestList **pResList,int vector[]);
void testSendRecv();
void mutantDataType_w();
void mutantDataType(int choice);
void testDataType_w();
void testDataType(int choice);
char* createFileToSend(char* strEnvFile);
#ifndef MPI_OPERATIONS_H
#define	MPI_OPERATIONS_H

#ifdef __cplusplus
extern "C" {
#endif
    
//General
int isEnabledMpiOperations();
void initializeMPI();  

//DataTypes
void initializeMPI_Datatype();
void init_Datatype_mutant();
void init_Datatype_test();
void init_Datatype_deploy();

//Test operations    
T_stTestList *receiveOriginalTestResults_r();
T_stTestInfo *receiveTestList(int nSource,int nTests);
T_stTestInfo *receiveTest(int nSource);
void receiveTestRefP(int nSource, T_stTestInfo** pTest);
void receiveOriginalTestResults(T_stTestList *pResList);
int sendOriginalTestResults(T_stTestList *pTestList);
int receiveSingleTestOriginal(T_stExecutionStructure pExeMode[MAX_WORKERS],int *pnWorkerSource);
int receiveSingleTestAndCheck(T_stExecutionStructure pExeMode[MAX_WORKERS],int *pnWorkerSource);
void sendTests(T_stExecutionStructure *pExeRetMode,int nDest);
void sendTest(T_stTestInfo *pTest,int nDest);

//Mutant operations
T_stMutant* receiveMutant(int nSource);
T_stMutant* receiveMutantAnySource(int* pnSource);
MutantList* receiveMutants_adaptive(T_stExecutionStructure pExeMode[MAX_WORKERS],int *pnWorkerSource);
int receiveMutants(T_stExecutionStructure pExeMode[MAX_WORKERS]);
void sendMutants(T_stExecutionStructure *pExeMode,int nDest);
void sendMutant(T_stMutant *pMutant,int nDest);
void resetMutant(T_stExecutionStructure* pExeMode);

//md5
int receiveMd5Mutants(T_stExecutionStructure pExeMode[MAX_WORKERS]);
void sendMd5Mutants(T_stExecutionStructure* pExeMode, char** strMd5List, int nTotalSizeInput, int nDest);

//Deploys
T_stExecutionStructure *receiveDeployMode();
void receiveDeployModeRefP(T_stExecutionStructure** exeMode);
void receiveDeployModeRef(T_stExecutionStructure* exeMode);
void sendDeployMode(T_stExecutionStructure *exeMode,int nDest);
         

#ifdef __cplusplus
}
#endif

#endif

/* 
 * File:   debugMode.c
 * Author: Pablo C. Cañizares
 *
 * Created on January 13, 2016, 12:56 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Malone.h"
#include "printers.h"
#include "debugMode.h"
#include "tests.h"
#include "Auxiliars.h"
#include "EnvFileSend.h"
#include "MPIDataTypes.h"
#include "MonteCarlo.h"
#include "randomElements.h"
#include "particles.h"

void doStaticMT()
{
    printf("Single MT process\n"); 
    m_lTotalTime = getTick();
    
    //It is possible that this values are already loaded
    if(m_stEnvValues ==NULL)        
    {
        m_stEnvValues = (T_stIniValues*) loadEnvironmentFromIniFile(m_strEnvironment);
    }
        
    if(m_stEnvValues != NULL)
    {                
        printEnvFile(m_stEnvValues);                
        malone_generate_mutants();
        if(malone_execute_Original_Program_Sequential())
        {
            printTestResults(m_pResList);
            malone_execute_mutants_single(1,m_stEnvValues->nTotalMutants);
        }
        
        m_lTotalTime = getTick()-m_lTotalTime;
        printResults(m_oMutantList.nElems, m_oMutantList.nDead, m_oTestExecMap.nEquivalents, m_oTestExecMap.nDupped, 0, 0, 0,m_lTotalTime);        
    }   
    else
    {
        printf("Error, INVALID environment file!\n");
    }
            
}

void doPirata()//TODO: Quitar esta mierda de aqui.
{
    printf("Doing it pirata!!\n"); 
    m_lTotalTime = getTick();
    m_stEnvValues = (T_stIniValues*) loadEnvironmentFromIniFile("/home/pablo/NetBeansProjects/Malone/Environments/env_first.ini");
    
    //Start with a trap
    T_stTestList* pList = generateRandomTestList_r(10000);
    
    if(m_stEnvValues != NULL)
    {                
        printEnvFile(m_stEnvValues);                
        malone_generate_mutants();
        malone_execute_Original_Program();
        printTestResults(m_pResList);
        malone_execute_mutants_single(1,m_stEnvValues->nTotalMutants);
        printMutantsInfo();
        printResults(m_oMutantList.nElems, m_oMutantList.nDead, m_oTestExecMap.nEquivalents, m_oTestExecMap.nDupped, 0, 0, 0,m_lTotalTime);
    }   
    else
    {
        printf("Error, INVALID environment file!\n");
    }
            
}


void masterDebugOperations()
{
    int choice, nAlgorithm, nRes;
    
    do
    {
     MPI_Barrier(MPI_COMM_WORLD);
     printf("MALONE says:\n\n");
     printf("0. Basic Send/Recv test (PiReduce)\n");
     printf("1. Monte-Carlo test\n");     
     printf("2. Send a EnvFile to all nodes\n");     
     printf("3. Perform the sequential mutation testing process\n");
     printf("4. Check Generic MPIDataTypes!!\n");
     printf("5. Check Test MPIDataTypes!!\n");
     printf("6. Check Mutants MPIDataTypes!!\n");
     printf("7. Distributed mutation testing!\n");
     printf("9. Exit\n");
     nRes = scanf("%d",&choice);

     if(nRes)
     {
        switch (choice)
        {
           case 0:
               printf("Sending PiReduce choice: %d\n", choice);
               MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
               testSendRecv();
               break;
           case 1: /*Call function here to do the required operation*/
               printf("Sending MonteCarlo choice: %d\n", choice);
               MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
               doMonteCarlo();            
               break;
           case 2: 
               printf("Sending SendFile choice: %d\n", choice);
               MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
               
               char* strFileSend = createFileToSend("test.ini");
               doSendEnvFile(strFileSend);               
               break;
           case 3: 
               doStaticMT();

               break;
           case 4:
               printf("MPI_DataTypes!! \n");
               MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
               //doDataTypesComplex();
               T_stExecutionStructure exe;
               exe.nExecutionMode = 0;
               exe.nMutantInit=1;
               exe.nMutantEnd =2;            
               exe.nTestInit=3;
               exe.nTestEnd=4;            
               //sendDeployMode(exe);
               severalParticles();
               doA();
               break;
           case 5:
               testDataType(choice);
               break; 
           case 6:
               mutantDataType(choice);
               break;    

            case 7:

                MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
                do{
                   printf("Mutation testing parallel selected!\n"); 
                   printf("Please select distribution workflow method: \n"); 
                   printf("1 - Distribute Mutants Algorithm (Static - DM)\n"); 
                   printf("2 - Distribute Test Cases Algorithm (Static DTC)\n"); 
                   printf("3 - Give Mutants On Demand (Dynamic - GMOD)\n"); 
                   printf("4 - F.U.L.L. \n"); 
                   printf("5 - Adaptive grain ...\n");
                   printf("6 - Back ...\n");
                   nRes = scanf("%d",&nAlgorithm);
                   if(nAlgorithm<1 || nAlgorithm>6)
                       printf("Wrong selection!!\n");
               }while(nAlgorithm<1 || nAlgorithm>6);

               if(nAlgorithm != 6)
               {
                   T_eExecutionMode eMode = (T_eExecutionMode) nAlgorithm;
                   malone_distributed_mutation_testing(eMode);
               }
               break;
           case 9:
               printf("Exit received!\n");
               MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
               break;             
            default:
                printf("Wrong Choice. Enter again\n");
           break;
        } 

       } 
    }while (choice != 9); 
        
}
char* createFileToSend(char* strEnvFile)
{
    char* strMaloneHome, *strRet;
    int nLen;
    strMaloneHome = strRet = NULL; 
    
    //Obtain the MALONE_HOME env value
     strMaloneHome = getenv("MALONE_HOME");
     
     if(strMaloneHome != NULL)
     {
        nLen = strlen(strMaloneHome) + strlen(PATH_ENV_DIR) + strlen(strEnvFile) + 1;
        strRet = malloc((nLen + 1) * sizeof (char));
        sprintf(strRet, "%s%s%s", strMaloneHome, PATH_ENV_DIR, strEnvFile);
        
        printf("Created file: %s", strRet);
     }
     else
         printf("ERROR! Insert the MALONE_HOME env variable");
     
     return strRet;
}
void workersDebugOperations()
{
    int number;
    do
    {
        number=9;
        printf("<%d> Waiting for Master operations!\n", m_nRank);
        MPI_Barrier(MPI_COMM_WORLD);

        //Receiving number
        MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Received test: %d\n", number);
         switch (number)
         {
            case 0:                
                testSendRecv();
                //comm_operations_test1();
                break;
            case 1: /*Call function here to do the required operation*/
                doMonteCarlo();  
                break;
            case 2: 
                  printf("sendFile !!\n");
                  doSendEnvFile("/home/pablo/Frameworks/Milu/env.ini"); 
                  break;
            case 3: printf("Goodbye\n"); 
            
                 break;
            case 4:
                printf("MPI_DataTypes!! \n");
                T_stExecutionStructure exe;
                //sendDeployMode(exe);
                severalParticles();
                doA();
                break;  
            case 5:
                testDataType_w();
                 break;
            case 6:
                mutantDataType_w();
                break;   
             case 7:
                printf("Complete distributed Mutation testing!!\n"); 
                malone_distributed_mutation_testing_workers();
                break;
             case 9:
                 printf("Exit received!\n");
                 break;
             default: printf("Wrong Choice. Enter again\n");
                 break;
         } 
    }while(number != 9);
}
void launchDebugMode()
{    
    if(m_nRank == MALONE_MASTER)
    {                
        masterDebugOperations();
    }
    else
    {
        workersDebugOperations();
    }
}
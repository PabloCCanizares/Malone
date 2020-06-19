/* 
 * File:   executionMode.c
 * Author: pablo
 *
 * Created on January 13, 2016, 12:55 PM
 */

#include "executionMode.h"

#define DEBUG_EXECUTIONMODE 0

void masterExecutionOperations()
{
    T_eExecutionMode eMode;
    int nValidDistribution;
    
    printf("<%d> Selmode: %d\n", m_nRank, m_nSelAlgorithm);
    nValidDistribution = 1;
    eMode = (T_eExecutionMode) m_nSelAlgorithm;
    switch(eMode)
    {
        case eSequential:
            printf("masterExecutionOperations - Selected mode: Sequential mode!\n");
            doStaticMT();
            nValidDistribution = 0;
            break;
        case eStaticMutants:
            printf("masterExecutionOperations - Selected mode: static distribution by mutants\n");
            break;
        case eStaticTests:
            printf("masterExecutionOperations - Selected mode: static distribution by tests\n");
            break;
        case eDynamic:
            printf("masterExecutionOperations - Selected mode: dynamic distribution by mutants\n");
            break;
        case eFullDynamic:
            printf("masterExecutionOperations - Selected mode: FULL dynamic distribution\n");
            break;
        case eAdaptiveGrain:
            printf("masterExecutionOperations - Selected mode: Adaptive distribution\n");
            break;            
        default:
            nValidDistribution = 0;
            printf("masterExecutionOperations - The selected methods does not exists\n");
            break;
    }
    
    if(nValidDistribution)
        malone_distributed_mutation_testing(eMode);
}
void workersExecutionOperations()
{
    if (DEBUG_EXECUTIONMODE) printf("workersExecutionOperation - Init\n");
    
    malone_distributed_mutation_testing_workers();
    
    if (DEBUG_EXECUTIONMODE) printf("workersExecutionOperation - End\n");
}
void launchExecutionMode()
{
    if(m_nRank == MALONE_MASTER)
    {                
        masterExecutionOperations();
    }
    else
    {
        workersExecutionOperations();
    } 
}
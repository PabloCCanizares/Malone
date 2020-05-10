/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "test_dist_algorithm.h"

void testDistributionAlgorithms()
{
    test_dist_algorithm_1();
}
void test_dist_algorithm_1()
{
    char* strPathComp;
    T_stIniValues*stEnvValueComp;
    int nTCs;
    
    strPathComp = loadEnvFile_t("autotest/test_autotest_add_stand.ini");
    m_stEnvValues = (T_stIniValues*) loadEnvironmentFromIniFile(strPathComp);
    
    m_nSelAlgorithm = 4;
    
    if(malone_isReady())
    {
        if(m_stEnvValues->nStandalone == 1)
            nTCs = readTestSuite();
                
        launchExecutionMode();
    }
}
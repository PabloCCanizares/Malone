/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "test_dist_algorithm.h"
#include "test_envfile.h"

void testDistributionAlgorithms()
{
    //test_dist_algorithm_1();        
    test_dist_algorithm_2();  //OK      
    test_dist_algorithm_3();  //OK  
    test_dist_algorithm_4();  //OK
    test_dist_algorithm_5();  //OK
}
void test_dist_algorithm_1()
{  
    printf("Test [test_dist_algorithm_1] - Init\n");
    
    m_nSelAlgorithm = 1;
    test_dist_base();
    free_auxiliars();    
    initialize_auxiliars();
    loadConfig();
    printf("Test [test_dist_algorithm_1] - End\n");
}
void test_dist_algorithm_2()
{  
    printf("Test [test_dist_algorithm_2] - Init\n");
    
    m_nSelAlgorithm = 2;
    test_dist_base();    
    free_auxiliars();
    initialize_auxiliars();
    loadConfig();
    printf("Test [test_dist_algorithm_2] - End\n");
}
void test_dist_algorithm_3()
{  
    printf("Test [test_dist_algorithm_3] - Init\n");
    
    m_nSelAlgorithm = 3;
    test_dist_base();
    free_auxiliars();    
    initialize_auxiliars();
    loadConfig();
    printf("Test [test_dist_algorithm_3] - End\n");
}
void test_dist_algorithm_4()
{  
    printf("Test [test_dist_algorithm_4] - Init\n");
    
    m_nSelAlgorithm = 4;
    test_dist_base();    
    free_auxiliars();
    initialize_auxiliars();
    loadConfig();
    printf("Test [test_dist_algorithm_4] - End\n");
}
void test_dist_algorithm_5()
{  
    printf("Test [test_dist_algorithm_5] - Init\n");
    
    m_nSelAlgorithm =5;
    test_dist_base();
    free_auxiliars();
    initialize_auxiliars();
    loadConfig();
    printf("Test [test_dist_algorithm_5] - End\n");
}

void test_dist_base()
{
    char* strPathComp;
    int nTCs;
    
    printf("Test [test_dist_algorithm_1] - Init\n");
    
    strPathComp = loadEnvFile_t("autotest/test_autotest_add_stand.ini");
    m_stEnvValues = (T_stIniValues*) loadEnvironmentFromIniFile(strPathComp);
    
    if(m_stConfigValues == NULL)
        printf("Config null!\n");
    
    if(m_stEnvValues != NULL)
    {
        if(malone_isReady())
        {
            if(m_stEnvValues->nStandalone == 1)
                nTCs = readTestSuite();

            launchExecutionMode();
        }
        else
        {
            printf("Test [test_dist_algorithm_1] - There exist some error in the provided configuration\n");
        }
    }
    else
    {
        printf("Test [test_dist_algorithm_1] - The environment file has not been loaded!\n");
    }

    printf("Test [test_dist_algorithm_1] - End\n");
}

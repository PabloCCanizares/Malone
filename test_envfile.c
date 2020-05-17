/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <assert.h>

#include "autotest.h"
#include "configuration.h"
#include "test_envfile.h"
void testEnvFile()
{           
    printf("Test [testEnvFile] - Loading environment files start\n");
       
    //Test 1
    test_envfile_1();
    
    //Test 2  
    test_envfile_2();
    
    //Test 3
    test_envfile_3();
    
    //Test 4
    test_envfile_4();
    
    //Test 5
    test_envfile_5();
    
    printf("testEnvFile - End\n");
}
void test_envfile_1()
{
    char* strPathIncomp;
    T_stIniValues* stEnvValueIncomp;
            
    //Load an incomplete file
    strPathIncomp = loadEnvFile_t("test/test_local_incomplete.ini");    
    stEnvValueIncomp = (T_stIniValues*) loadEnvironmentFromIniFile(strPathIncomp);
    if(stEnvValueIncomp != NULL)
    {
        free(stEnvValueIncomp);
        printf("Test [testEnvFile] - Ini_1 sucessfully processed\n");
    }
}
/**
 * Loads an existing environmental file, analyses a part of its content and delete it.
 */
void test_envfile_2()
{
    char *strPathComp;
    T_stIniValues*stEnvValueComp;
    
    strPathComp = loadEnvFile_t("test/test_local_complete.ini");
    stEnvValueComp = (T_stIniValues*) loadEnvironmentFromIniFile(strPathComp);
    if(stEnvValueComp != NULL)
    {
        //Here is possible to check some values
        assert(stEnvValueComp->nDistributeOriginal == 1);
        assert(stEnvValueComp->nSortTestSuite == 0);
        assert(stEnvValueComp->nScatterWorkload == 1);
        assert(stEnvValueComp->nParallelCompilation == 0);
        assert(stEnvValueComp->nParallelMd5sum == 1);
        assert(stEnvValueComp->nMultipleCoordinators == 0);
        
        free(stEnvValueComp);
        printf("Test [testEnvFile] - Test_2 OK\n");
    }
    else
        printf("Test [testEnvFile] - Test_2 KO\n");
    
}
/**
 * Test_3: Tries to load an inexistent file.
 */
void test_envfile_3()
{    
    T_stIniValues* stNotExisting;
    //Load a file that not exists
    stNotExisting = (T_stIniValues*) loadEnvironmentFromIniFile(loadEnvFile_t("local/test_local_complete.ini"));

    assert(stNotExisting == NULL);
    
    if(stNotExisting == NULL)
        printf("Test [testEnvFile] - Test_3 OK\n");
    else
        printf("Test [testEnvFile] - Test_3 KO\n");
}
/**
 * Test_4: Loads an existing environmental file and free it for 1000 times.
 */
void test_envfile_4()
{
    char *strPathComp;
    T_stIniValues*stEnvValueComp;
    
    //Loop for malloc and free the env file
    int n=100;
    do
    {
        strPathComp = loadEnvFile_t("test/test_local_complete.ini");
        stEnvValueComp = (T_stIniValues*) loadEnvironmentFromIniFile(strPathComp);
        
        assert(stEnvValueComp != NULL);
        
        if(stEnvValueComp != NULL)
        {
            free(stEnvValueComp);
        }
    
        n--;
    }while(n>0);
    printf("Test [testEnvFile] - Ini_5 sucessfully processed\n");
}

void test_envfile_5()
{
    T_stIniValues* stNotExisting;
    //Load a file that not exists
    stNotExisting = (T_stIniValues*) loadEnvironmentFromIniFile(loadEnvFile_t("autotest/test_autotest_add_stand.ini"));

    assert(stNotExisting != NULL);
    
    if(stNotExisting != NULL)
        printf("Test [testEnvFile] - Test_5 OK\n");
    else
        printf("Test [testEnvFile] - Test_5 KO\n");    
}
char* loadEnvFile_t(const char* envValue)
{
    char* strEnvFile, *strMaloneHome, *fileRet;
    int nRet, nLen;
    
    fileRet = NULL;
    strEnvFile = envValue;
    
    if (TEST_DEBUG) printf("parseArguments - Init\n");
    strMaloneHome = getenv("MALONE_HOME");
    if (TEST_DEBUG)  printf("parseArguments - MaloneHome: %s |  File: %s", strMaloneHome, strEnvFile);
    if (strMaloneHome != NULL && strEnvFile != NULL) {
        m_stEnvValues = NULL;
        if (TEST_DEBUG) printf("parseArguments - Malone environment variable: %s\n", strMaloneHome);
        nLen = strlen(strMaloneHome) + strlen(PATH_ENV_DIR) + strlen(strEnvFile) + 1;
        fileRet = malloc((nLen + 1) * sizeof (char));
        sprintf(fileRet, "%s%s%s", strMaloneHome, PATH_ENV_DIR, strEnvFile);
        if (TEST_DEBUG) printf("parseArguments - Env file is: %s\n", fileRet);

        //Check if exists
        nRet = file_exist(fileRet);

        if(nRet == 0)
            printf("<%d> loadEnvFile - ERROR!! The selected environment file (.ini) does not exists!\n", m_nRank);

    } else {
        printf("<%d> Malone environment variable is empty! Please insert $MALONE_HOME in your system\n", m_nRank);
    }
    
    if (TEST_DEBUG) printf("parseArguments - End\n");
    return fileRet;
}
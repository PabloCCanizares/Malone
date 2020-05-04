/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "autotest.h"
#include "test_command.h"    
void testCommand()
{
    char* strPath, *strPathIncomp, *strPathComp;   
    printf("Test [testCommand] - Init\n");
   
    test_command_1();
    test_command_2();
    test_command_3();   
    test_command_4();   
    test_command_5();   
    test_command_6();   
    printf("Test [testCommand] - End\n");
}

void test_command_1()
{
    char* strCommandRet;
    int nLoops;
    
    printf("Test [testCommand] - Test 1 ...\n");
    nLoops = 10;
    
    prettyPrint(0, nLoops, 0);
    for(int i=0;i<nLoops;i++)
    {
        if(i>0)
            prettyPrint(i, nLoops, i-1);
        
        for(int j=0;j<1000;j++)
        {
            strCommandRet = execCommandLine("echo $MALONE_HOME");

            if(strCommandRet != NULL)
                free(strCommandRet);
        }
        
    }
    printf(" OK\n");
}

void test_command_2()
{
    char* strCommandRet;
    int nLoops, nIndex;
    
    printf("Test [testCommand] - Test 2 ...\n");
    nLoops = 70000;
    
    prettyPrint(0, nLoops, 0);
    for(int i=0;i<nLoops;i++)
    {
        if(i>0)
            prettyPrint(i, nLoops, i-1);
        
        for(int j=0;j<1000;j++)
        {
            strCommandRet = execCommandLineHPC("echo $MALONE_HOME");
        
            if(strCommandRet != NULL)
                free(strCommandRet);        
        }
     
    }
    
    printf(" OK\n");
}

void test_command_3()
{
    char* strCommandRet;
    int nLoops;
    
    nLoops = 10;
    
    printf("Test [testCommand] - Test 3 ...\n");
    
    prettyPrint(0, nLoops, 0);
    for(int i=0;i<nLoops;i++)
    {    
        if(i>0)
            prettyPrint(i, nLoops, i-1);
            
        for(int j=0;j<1000;j++)
        {
            strCommandRet = execCommandLine("/localSpace/mutomvo/project_fft/mutants/0/fft_master -a 2 -m 1 -v 2 | md5sum");

            if(strCommandRet != NULL)
                free(strCommandRet);
        }
    }
    printf(" OK\n");
}

void test_command_4()
{
        
    char* strCommandRet;
    int nLoops;
    
    nLoops = 10;
    
    printf("Test [testCommand] - Test 4 ...\n");
    
    prettyPrint(0, nLoops, 0);
    for(int i=0;i<nLoops;i++)
    {    
        if(i>0)
            prettyPrint(i, nLoops, i-1);
            
        for(int j=0;j<1000;j++)
        {
            strCommandRet = execCommandLine("md5sum /localSpace/mutomvo/project_fft/mutants/0/fft_master | cut -f 1 -d ' '");

            if(strCommandRet != NULL)
                free(strCommandRet);
        }
    }
    printf(" OK\n");
}

void test_command_5()
{
    char* strCommandRet;
    int nLoops;
    
    nLoops = 10;
    
    printf("Test [testCommand] - Test 5 ...\n");
    
    prettyPrint(0, nLoops, 0);
    for(int i=0;i<nLoops;i++)
    {    
        if(i>0)
            prettyPrint(i, nLoops, i-1);
            
        for(int j=0;j<1000;j++)
        {
            strCommandRet = execCommandLineHPC("/localSpace/mutomvo/project_fft/mutants/0/fft_master -a 2 -m 1 -v 2 | md5sum");

            if(strCommandRet != NULL)
                free(strCommandRet);
        }
    }
    printf(" OK\n");
}
/**
 * Executing commands using popen_noshell, faster, but not provide the response to the user.
 */
void test_command_6()
{
        
    char* strCommandRet;
    int nLoops;
    
    nLoops = 10;
    
    printf("Test [testCommand] - Test 6 ...\n");
    
    prettyPrint(0, nLoops, 0);
    for(int i=0;i<nLoops;i++)
    {    
        if(i>0)
            prettyPrint(i, nLoops, i-1);
            
        for(int j=0;j<1000;j++)
        {
            strCommandRet = execCommandLineHPC("md5sum /localSpace/mutomvo/project_fft/mutants/0/fft_master | cut -f 1 -d ' '");
            //strCommandRet = execCommandLineHPC("touch /home/j0hn/Desktop/ehlo.ini");
            
            printf("> %s\n", strCommandRet);
            if(strCommandRet != NULL)
                free(strCommandRet);
        }
    }
    printf(" OK\n");
}
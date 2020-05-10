/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "autotest.h"
#include "test_command.h"   
#include "popen_noshell.h"

void testCommand()
{
    printf("Test [testCommand] - Init\n");
   
    test_command_waste();
    //test_command_1();
    test_command_2();
    //test_command_3();   
    //test_command_4();   
    //test_command_5();   
    //test_command_6();   
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
            {             
                free(strCommandRet);        
            }   
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

void example_reading(int use_compat) {
	FILE *fp;
	char buf[256];
	int status;
	struct popen_noshell_pass_to_pclose pclose_arg;

	char *cmd = "ls -la /proc/self/fd"; /* used only by popen_noshell_compat(), we discourage this type of providing a command */

	/* the command arguments used by popen_noshell() */
	char *exec_file = "ls";
	char *arg1 = "-la";
	char *arg2 = "/proc/self/fd";
	char *arg3 = (char *) NULL; /* last element */
	char *argv[] = {exec_file, arg1, arg2, arg3}; /* NOTE! The first argv[] must be the executed *exec_file itself */
	
	if (use_compat) {
		fp = popen_noshell_compat(cmd, "r", &pclose_arg);
		if (!fp) {
			err(EXIT_FAILURE, "popen_noshell_compat()");
		}
	} else {
		fp = popen_noshell(exec_file, (const char * const *)argv, "r", &pclose_arg, 0);
		if (!fp) {
			err(EXIT_FAILURE, "popen_noshell()");
		}
	}

	while (fgets(buf, sizeof(buf)-1, fp)) {
		printf("Got line: %s", buf);
	}

	status = pclose_noshell(&pclose_arg);
	if (status == -1) {
		err(EXIT_FAILURE, "pclose_noshell()");
	} else {
		printf("The status of the child is %d. Note that this is not only the exit code. See man waitpid().\n", status);
	}
}
//TODO: El problema que encontramos aqui, es que popen_noshell ofrece modo lectura y escritura, pero no tiene un pipe bidireccional
//Esto quiere decir que si ponemos un pipe | la respuesta ya no llega. Se ve en los ejemplos de debajo

//Hay que seguir explorando esta opcion, porque el framework no puede seguir con el metodo general.
//https://blog.famzah.net/2018/12/19/posix_spawn-performance-benchmarks-and-usage-examples/
//Basicamente parte de la escalabilidad del sistema se nos va por aqui.

//Estamos siguiendo un ejemplo en malone_lab
//La idea es leer esto y entender: https://stackoverflow.com/questions/3884103/can-popen-make-bidirectional-pipes-like-pipe-fork
void test_command_waste()
{
   char* strPath, *strPathIncomp, *strPathComp, *strRet;   
   
   example_reading(1);
   strRet = execCommandLineHPC("ls -la /proc/self/fd");
   
   if(strRet != NULL)
       printf("Command ret: %s", strRet);
   else
       printf("Command ret: null");

   
   strRet = execCommandLineHPC("md5sum /localSpace/mutomvo/project_fft/mutants/0/fft_master");
    
   if(strRet != NULL)
       printf("Command ret: %s", strRet);
   else
       printf("Command ret: null");
   
   strRet = execCommandLineHPC("md5sum /localSpace/mutomvo/project_fft/mutants/0/fft_master | cut -f 1 -d ' '");

   if(strRet != NULL)
       printf("Command ret: %s", strRet);
   else
       printf("Command ret: null");
}
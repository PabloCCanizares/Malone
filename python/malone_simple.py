#!/usr/bin/python

import subprocess
import shlex
import time
import sys
sys.path.append('/lustre/home/dc010/pcerro/Malone/python')
sys.path.insert(0,'/lustre/home/dc010/pcerro/Malone/python')

from Environment import malone_environment
from MDeploy import malone_deploy

class malone_streaming:
                    
    ORIGINAL_PATH = "[[ORIGINAL_PATH]]"    
    MUTANTS_PATH = "[[MUTANTS_PATH]]"   
    INDEX_MUTANT = "[[INDEX_MUTANT]]"
    INDEX_TEST = "[[INDEX_TEST]]"
    def __init__(self, nAlgorithm):
        self.nAlgorithm = nAlgorithm
        self.nLoadedTcs = 0  
        self.originalResults = list()
        self.originalTimes = list()
        self.nMutantsKilled = 0
        self.nAliveMutants = 0
        self.environment = malone_environment()
        
        self.m_stConfig = None
        self.testSuite = None
        
        #Timing        
        self.timeInitializing = 0;
        self.timeCompilingOriginal = 0;
        self.timeExecutingOriginal = 0;
        self.timeExecutingMutants = 0;
        self.nOriginalTimeoutFactor = 0
      
    def setOriginalResults(self, originalResIn):
        self.originalResults = originalResIn
    
    def getOriginalResults(self):
        return self.originalResults

    def getOriginalTimes(self):
        return self.originalTimes
        
    def setOriginalTimes(self, originalTimesIn):
        self.originalTimes = originalTimesIn
        
    def buildOriginalExecLine(self, nIndex):
        #linePattern = self.m_stEnv['general']['ExecutionLineOriginal']
        linePattern = self.environment.ExecutionLineOriginal
        line = self.testSuite[nIndex]
        if line:
            #linePattern = linePattern.replace(self.ORIGINAL_PATH, self.m_stEnv['general']['ApplicationPath'])     
            linePattern = linePattern.replace(self.ORIGINAL_PATH, self.environment.ApplicationPath)     
            #linePattern = linePattern.replace(self.MUTANTS_PATH, self.m_stEnv['general']['MutantPath'])             
            linePattern = linePattern.replace(self.MUTANTS_PATH, self.environment.MutantPath)             
            linePattern = linePattern.replace(self.INDEX_TEST, str(nIndex))   
            linePattern = linePattern.replace(self.INDEX_MUTANT, str(0))              
            
            nFactorTime = self.environment.OriginalTimeoutFactor
            
            if nFactorTime == 0:
                nFactorTime = 10
            
            line = "timeout "+str(float(nFactorTime))+" "+linePattern + line;
            
        return line   
        
    def buildMutantExecLine(self, nIndexTc, nMutantTc):
        #linePattern = self.m_stEnv['general']['ExecutionLineMutants']
        linePattern = self.environment.ExecutionLineMutants
        line = self.testSuite[nIndexTc]
        nFactorTime = 10
        if line: 
            linePattern = linePattern.replace(self.ORIGINAL_PATH,  self.environment.ApplicationPath)        
            linePattern = linePattern.replace(self.MUTANTS_PATH, self.environment.MutantPath)             
            linePattern = linePattern.replace(self.INDEX_TEST, str(nIndexTc))   
            linePattern = linePattern.replace(self.INDEX_MUTANT, str(nMutantTc))  
            
            nFactorTime = self.originalTimes[nIndexTc]*self.environment.TimeoutFactor
            if nFactorTime < 1:
                nFactorTime = 7
            
            line = "timeout "+str(float(nFactorTime))+" "+linePattern + line;
        else:
            line = "##";
        return line 
        
        
    def executeCommand(self, command):

        #subprocess.run(command)
        ## call date command ##
        p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
         
        ## Talk with date command i.e. read data from stdout and stderr. Store this info in tuple ##
        ## Interact with process: Send data to stdin. Read data from stdout and stderr, until end-of-file is reached.  ##
        ## Wait for process to terminate. The optional input argument should be a string to be sent to the child process, ##
        ## or None, if no data should be sent to the child.
        (output, err) = p.communicate()
         
        ## Wait for date to terminate. Get return returncode ##
        p_status = p.wait()
       # if self.m_stConfig['logs']['MALONE_DEBUG_MAIN_COMMAND'] != "0":    
        if 1:       
            print ("Executing command: ", command)            
            print ("Command output : ", output)            
        return output
        
    def setPaths(self, configPath, envPath):
        self.configPath = configPath;
        self.envPath = envPath
        
    def loadConfig(self, configPath):
        #Load the config values
        self.m_stConfig = configparser.ConfigParser()
        self.m_stConfig.read(configPath)
        print("Config values!")     # -> "/path/name/"
        print({section: dict(self.m_stConfig[section]) for section in self.m_stConfig.sections()})
            
    def loadEnvFile(self, envPath):
        #Load the environment values
        #self.m_stEnv = configparser.ConfigParser()        
        self.environment.loadEnvFile(envPath)
            
    def loadTestSuite(self):
        #tsPath = self.m_stEnv['standalone']['TestSuiteFile']
        tsPath = self.environment.TestSuiteFile
        with open(tsPath) as f:
            self.testSuite = f.read().splitlines()
            self.nLoadedTcs = len(self.testSuite)
           # if self.m_stConfig['logs']['MALONE_DEBUG_PRINT'] != "0":            
           #     print(self.testSuite, sep='\n')
        print("Total tcs loaded: ", self.nLoadedTcs)

    def initialize(self):
        print("Initialising malone")
        #self.loadConfig(self.configPath)
        self.loadEnvFile(self.envPath)
        self.loadTestSuite()
        print("Initialised malone")

    def compile_original(self):
        #TODO
        print("compile_original - Init")
        bSuccess = True
        if self.environment.CompilationEnabled != "0": 
            #originalComp = self.m_stEnv['compilation']['CompilationLineOriginal']
            originalComp = self.environment.CompilationLineOriginal
            originalComp = originalComp.replace(self.ORIGINAL_PATH, self.environment.ApplicationPath)
            command = shlex.split(originalComp)
            print("Executing command ", command)
            self.executeCommand(originalComp)
        
        print("compile_original - End: ", bSuccess)
        return bSuccess;
    
    def execute_original_program(self):
        nIndex = 0
        bContinue = True        
        print("execute_original_program - Executing tests ", self.environment.TotalTests)
        self.timeExecutingOriginal = time.time()
        bContinue = self.execute_original_program_by_scheme(0, self.environment.TotalTests)
        self.timeExecutingOriginal = time.time() - self.timeExecutingOriginal
        print("execute_original_program - Total execution time: ", self.timeExecutingOriginal )
        return bContinue
    
    
    def execute_original_program_by_scheme(self, nTestInit, nTestEnd):
        nIndex = nTestInit
        bContinue = True        
        print("Executing tests ", (nTestEnd-nTestInit)+1)
        self.timeExecutingOriginal = time.time()
        while ((nIndex < nTestEnd) and bContinue):   
            tickInit = time.time()
            command = self.buildOriginalExecLine(nIndex)            
            output = self.executeCommand(command)            
            self.originalResults.insert(nIndex, output)
            totalTime =time.time() - tickInit
            self.originalTimes.insert(nIndex, totalTime)
            nIndex= nIndex+1
        self.timeExecutingOriginal = time.time() - self.timeExecutingOriginal
        print("Total execution time: ", self.timeExecutingOriginal )
        return bContinue
        
    def compile_mutants(self):
        #TODO:
        nIndex=0
        return True
    
    def execute_mutants(self):              
        print("Executing", self.environment.TotalTests," tests over ", self.environment.TotalMutants," mutants ")
        self.timeExecutingMutants = time.time()

        self.execute_mutants_by_scheme(self.environment.StartingMutant, self.environment.TotalMutants, 0, self.environment.TotalTests)    
        
        self.timeExecutingMutants = time.time() - self.timeExecutingMutants
        print("Total mutants execution time: ", self.timeExecutingMutants )
        return True
    
    def execute_mutants_by_scheme(self, nInitMutant, nEndMutant, nTestInit, nTestEnd):
        nIndexTc = 0
        nIndexMutant =nInitMutant
        bContinue = True        
        print("execute_mutants_by_scheme - [MI :", nInitMutant," - ME:  ", nEndMutant,"] [TI: ", nTestInit, "- TE: ", nTestEnd, "]")
        while nIndexMutant <= nEndMutant:
            bContinue = True
            nIndexTc = nTestInit
            print("Executing mutant", nIndexMutant)
            while ((nIndexTc < nTestEnd) and bContinue):            
                command = self.buildMutantExecLine(nIndexTc, nIndexMutant)            
                output = self.executeCommand(command)            
                
                #Check the output obtained from executing the mutant, comparing it with the original res
                if output and output == self.originalResults[nIndexTc]:
                    nIndexTc= nIndexTc+1
                else:
                    bContinue = False
                    self.nMutantsKilled +=1
            if bContinue:
                self.nAliveMutants +=1
            nIndexMutant  +=1
    
    def execute_mutants_by_scheme_and_res(self, nInitMutant, nEndMutant, nTestInit, nTestEnd):
        nIndexTc = 0
        nIndexMutant =nInitMutant
        bContinue = True      
        resList = list()
        
        print("execute_mutants_by_scheme - [MI :", nInitMutant," - ME:  ", nEndMutant,"] [TI: ", nTestInit, "- TE: ", nTestEnd, "]")
        while nIndexMutant <= nEndMutant:
            mutantTestList = list()            
            bContinue = True
            nIndexTc = nTestInit
            print("Executing mutant", nIndexMutant)
            while ((nIndexTc < nTestEnd) and bContinue):    
                singleTest = list()
                tickInit = time.time()
                command = self.buildMutantExecLine(nIndexTc, nIndexMutant)            
                output = self.executeCommand(command)            
                totalTime =time.time() - tickInit
                #Check the output obtained from executing the mutant, comparing it with the original res
                if output and output == self.originalResults[nIndexTc]:
                    nIndexTc= nIndexTc+1
                    singleTest.insert(0,1)
                else:
                    bContinue = False
                    singleTest.insert(0,0)
                    self.nMutantsKilled +=1
                singleTest.insert(1,totalTime)
                mutantTestList.append(singleTest)
            if bContinue:
                self.nAliveMutants +=1
            nIndexMutant  +=1
            resList.append(mutantTestList)
        return [malone_deploy(nInitMutant, nEndMutant, nTestInit, nTestEnd), resList]
            
    def print_stats(self):
        print("Total original time: ", self.timeExecutingOriginal )
        print("Total mutants execution time: ", self.timeExecutingMutants )
        print("Total mutants alive: ", self.nAliveMutants)
        print("Total mutants killed: ", self.nMutantsKilled)
        
        if self.nMutantsKilled >0 and self.environment.TotalMutants >0:
            print("MS: ", self.nMutantsKilled/self.environment.TotalMutants)
        else:
            print("MS: 0");
          
print("MaloneStreaming - Init")
malone = malone_streaming(1);
         
##Load config
malone.setPaths("/lustre/home/dc010/pcerro/Malone/Config.ini", "/lustre/home/dc010/pcerro/Malone/python/Environments/test_FFT_stand.ini")
malone.initialize()

if malone.compile_original():
    if malone.execute_original_program():
        if malone.compile_mutants():
            if malone.execute_mutants():
                #TODO: save results
                malone.print_stats()
print("MaloneStreaming - End")

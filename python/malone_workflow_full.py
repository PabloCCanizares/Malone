# -*- coding: utf-8 -*-

from dispel4py.base import ProducerPE
from dispel4py.base import IterativePE
from dispel4py.workflow_graph import WorkflowGraph

import configparser
import subprocess
import shlex
import time
       
class malone_environment:
    
    def __init__(self):
        self._FrameworkPath=""
        self._ApplicationPath=""
        self._MutantPath=""
        self._ApplicationName=""
        self._ExecutionLineOriginal=""
        self._ExecutionLineMutants=""
        self._GenerationLineMutants=""
        self._TotalTests=0
        self._TotalMutants=0
        self._StartingMutant=0        
        self._OriginalMaxTimeout=0        
        self._OriginalTimeoutFactor=0
                    
    def loadEnvFile(self, envPath):
        print("malone_environment::loadEnvFile - Loading environment: ", envPath)
        envFile = configparser.ConfigParser()
        envFile.read(envPath)        
        self._FrameworkPath=envFile['general']['FrameworkPath']
        self._ApplicationPath=envFile['general']['ApplicationPath']
        self._MutantPath=envFile['general']['MutantPath']
        self._ApplicationName=envFile['general']['ApplicationName']
        self._ExecutionLineOriginal=envFile['general']['ExecutionLineOriginal']
        self._ExecutionLineMutants=envFile['general']['ExecutionLineMutants']
        self._GenerationLineMutants=envFile['general']['GenerationLineMutants']        
        self._TotalMutants = int(envFile['general']['TotalMutants'])
        self._TotalTests = int(envFile['general']['TotalTests'])
        self._StartingMutant = int(envFile['general']['StartingMutant'])
        self._TimeoutFactor = int(envFile['timeouts']['MALONE_MAX_MUTANTS_TIMEOUT_FACTOR'])
        self._OriginalTimeoutFactor = int(envFile['timeouts']['MALONE_MAX_ORIGINAL_TIMEOUT'])
        self._TestSuiteFile = envFile['standalone']['TestSuiteFile']
        self._CompilationEnabled =  envFile['compilation']['CompilationEnabled']
        self._CompilationLineOriginal =  envFile['compilation']['CompilationLineOriginal']
        print({section: dict(envFile[section]) for section in envFile.sections()})    
        
        
    @property
    def FrameworkPath(self):
        return self._FrameworkPath
        
    @property
    def ApplicationPath(self):
        return self._ApplicationPath

    @property
    def MutantPath(self):
        return self._MutantPath
        
    @property
    def ApplicationName(self):
        return self._ApplicationName 
        
    @property
    def ExecutionLineOriginal(self):
        return self._ExecutionLineOriginal         
        
    @property
    def ExecutionLineMutants(self):
        return self._ExecutionLineMutants 
        
    @property
    def GenerationLineMutants(self):
        return self._GenerationLineMutants         
        
    @property
    def TotalMutants(self):
        return self._TotalMutants           
        
    @property
    def StartingMutant(self):
        return self._StartingMutant
        
    @property
    def TotalTests(self):
        return self._TotalTests   
        
    @property
    def TimeoutFactor(self):
        return self._TimeoutFactor        
        
    @property
    def OriginalTimeoutFactor(self):
        return self._OriginalTimeoutFactor    
    
    @property
    def TestSuiteFile(self):
        return self._TestSuiteFile    
        
    @property
    def CompilationEnabled(self):
        return self._CompilationEnabled
        
    @property
    def CompilationLineOriginal(self):
        return self._CompilationLineOriginal
        
class malone_deploy:
    def __init__(self, nMutantInit, nMutantEnd, nTestInit, nTestEnd):
        self._MutantInit = nMutantInit
        self._MutantEnd = nMutantEnd
        self._TestInit = nTestInit
        self._TestEnd = nTestEnd       
        
    def initDeploy(self, nMutantInit, nMutantEnd, nTestInit, nTestEnd):
        self._MutantInit = nMutantInit
        self._MutantEnd = nMutantEnd
        self._TestInit = nTestInit
        self._TestEnd = nTestEnd
        
    @property
    def MutantInit(self):
        return self._MutantInit
        
    @property
    def MutantEnd(self):
        return self._MutantEnd        
        
    @property
    def TestInit(self):
        return self._TestInit
        
    @property
    def TestEnd(self):
        return self._TestEnd       
        

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
            
class ResultsProcess(IterativePE):

    def __init__(self, strConfig, strEnv, nAlgorithm):
        IterativePE.__init__(self)
        self.malone = malone_streaming(nAlgorithm)
        self.malone.setPaths(strConfig, strEnv)
        self.malone.initialize()    
        self.nKilledMutants=0
        self.mutantRes = list()
        
        print("ResultsProcess initialized!")
        for i in range (1, self.malone.environment.TotalMutants):     
            self.mutantRes.append(0)
        
    def _process(self, data):
        print("ResultsProcess::_process - Init")
        deploy = data[0]
        results = data[1]
        mutantList = results[1]
        
        #print("Deploy: ", deploy)
        #print("Results: ", results)
        #print("Mutants: ", mutantList)
        
        print("Deploy: [MI: ", deploy.MutantInit, " | ME: ", deploy.MutantEnd, " ]")
                
        totalMutants = len(mutantList)    
        print("Total mutants received: ", totalMutants)
        for i in range(0, totalMutants):
            mutant = mutantList[i]
            print("Mutant: ", deploy.MutantInit+i)
            totalTests = len(mutant)
            #print(mutant)
            for j in range(0, totalTests):
                test = mutant[j]
                print("Test ", j, " >", test)
            
        #malonecp = copy.deepcopy(self.malone)    
        #malonecp.print_stats()        
        self.malone.print_stats()    
        
        print("ResultsProcess::_process - End")    
            
        
class WorkerProcess(IterativePE):

    def __init__(self, strConfig, strEnv, nAlgorithm):
        IterativePE.__init__(self)
        self.malone = malone_streaming(nAlgorithm)
        self.malone.setPaths(strConfig, strEnv)
        self.malone.initialize()
        self.divisor = 5
        print("WorkerProcess initialized!")
        
    def _process(self, data):
        print("WorkerProcess::_process - Init")
        originalRes = data[0]       
        originalTimes = data[1] 
        deploy = data[2]
                
        print("Deploy: [MI: ", deploy.MutantInit, " | ME: ", deploy.MutantEnd, " ]")
        
        print("Original Res: ", originalRes)
        print("Original Times: ", originalTimes)
        
        self.malone.setOriginalResults(originalRes)
        self.malone.setOriginalTimes(originalTimes)
        result = self.malone.execute_mutants_by_scheme_and_res(deploy.MutantInit, deploy.MutantEnd, deploy.TestInit, deploy.TestEnd)
        print("WorkerProcess::_process - End")        
        return [deploy, result]


class MasterProcess(ProducerPE):
    def __init__(self, strConfig, strEnv, nAlgorithm):
        ProducerPE.__init__(self)
        self.malone = malone_streaming(nAlgorithm)
        self.malone.setPaths(strConfig, strEnv)
        self.malone.initialize()
        print("Master process initialized!")
        
    def _process(self, inputs):
        print("MasterProcess::_process - Init")
        if self.malone.compile_original():
            if self.malone.execute_original_program():
                if self.malone.compile_mutants():
                    
                    self.malone.environment.TotalMutants
                    for i in range(self.malone.environment.StartingMutant, self.malone.environment.TotalMutants):
                        deploy= malone_deploy(i,i+1,0,self.malone.environment.TotalTests)
                        #print("Sending deploy: ", deploy.MutantInit," - ", deploy.MutantEnd)
                        #print("Original times: ", self.malone.getOriginalTimes())
                        #print("Original res: ", self.malone.getOriginalResults())
                        self.write('output', [self.malone.getOriginalResults(), self.malone.getOriginalTimes(), deploy])
        
        print("Master process end _processing!")


strConfig = "/lustre/home/dc010/pcerro/Malone/Config.ini"
strEnv =  "/lustre/home/dc010/pcerro/Malone/python/Environments/test_FFT_stand.ini"
#strEnv =  "/localSpace/research_edinburgh/dispel4py_training_material/mutationTesting/Environments/test_FFT_stand.ini"
#strConfig = "/localSpace/research_edinburgh/dispel4py_training_material/mutationTesting/Config.ini"

nAlgorithm = 4

master = MasterProcess(strConfig, strEnv, nAlgorithm)
workers = WorkerProcess(strConfig, strEnv, nAlgorithm)
results = ResultsProcess(strConfig, strEnv, nAlgorithm)

graph = WorkflowGraph()
graph.connect(master, 'output', workers, 'input')
graph.connect(workers, 'output', results, 'input')
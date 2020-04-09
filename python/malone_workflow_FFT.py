# -*- coding: utf-8 -*-

import sys
sys.path.append('/lustre/home/dc010/pcerro/Malone/python')
sys.path.insert(0,'/lustre/home/dc010/pcerro/Malone/python')

from malone_sequential import malone_streaming
from MDeploy import malone_deploy
from dispel4py.base import ProducerPE
from dispel4py.base import IterativePE
from dispel4py.workflow_graph import WorkflowGraph


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
                
        #print("Deploy: [MI: ", deploy.MutantInit, " | ME: ", deploy.MutantEnd, " ]")        
        #print("Original Res: ", originalRes)
        #print("Original Times: ", originalTimes)
        
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
        
        print("MasterProcess::_process - End")


strConfig = "/lustre/home/dc010/pcerro/Malone/Config.ini"
strEnv =  "/lustre/home/dc010/pcerro/Malone/python/Environments/test_FFT_stand.ini"
nAlgorithm = 4

master = MasterProcess(strConfig, strEnv, nAlgorithm)
workers = WorkerProcess(strConfig, strEnv, nAlgorithm)
results = ResultsProcess(strConfig, strEnv, nAlgorithm)

graph = WorkflowGraph()
graph.connect(master, 'output', workers, 'input')
#graph.connect(workers, 'output', results, 'input')

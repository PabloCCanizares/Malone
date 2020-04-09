# -*- coding: utf-8 -*-
"""
Created on Tue Oct  9 13:05:09 2018

@author: Pablo C. Cañizares
"""
try:
    import configparser
except:
    import ConfigParser    
       
class malone_environment:
    
    def __init__(self):
        self._FrameworkPath=""
        self._ApplicationPath=""
        self._MutantPath=""
        self._ApplicationName=""
        self._ExecutionLineOriginal=""
        self._ExecutionLineMutants=""
        self._GenerationLineMutants=""
        self._MarkerToken=""
        self._TotalTests=0
        self._TotalMutants=0
        self._StartingMutant=0        
        self._OriginalMaxTimeout=0        
        self._OriginalTimeoutFactor=0
                    
    def loadEnvFile(self, envPath):
        print("malone_environment::loadEnvFile - Loading environment: ", envPath)
        
        try:
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
            self._MarkerToken =  envFile['misc']['MarkerToken']
            
            print({section: dict(envFile[section]) for section in envFile.sections()})
        except:
            envFile = ConfigParser.ConfigParser()    
            envFile.read(envPath)        
            self._FrameworkPath=envFile.get('general','FrameworkPath')
            self._ApplicationPath=envFile.get('general','ApplicationPath')
            self._MutantPath=envFile.get('general','MutantPath')
            self._ApplicationName=envFile.get('general','ApplicationName')
            self._ExecutionLineOriginal=envFile.get('general','ExecutionLineOriginal')
            self._ExecutionLineMutants=envFile.get('general','ExecutionLineMutants')
            self._GenerationLineMutants=envFile.get('general','GenerationLineMutants')        
            self._TotalMutants = int(envFile.get('general','TotalMutants'))
            self._TotalTests = int(envFile.get('general','TotalTests'))
            self._StartingMutant = int(envFile.get('general','StartingMutant'))
            self._TimeoutFactor = int(envFile.get('timeouts','MALONE_MAX_MUTANTS_TIMEOUT_FACTOR'))
            self._OriginalTimeoutFactor = int(envFile.get('timeouts','MALONE_MAX_ORIGINAL_TIMEOUT'))
            self._TestSuiteFile = envFile.get('standalone','TestSuiteFile')
            self._CompilationEnabled =  envFile.get('compilation','CompilationEnabled')
            self._CompilationLineOriginal =  envFile.get('compilation','CompilationLineOriginal')
            self._MarkerToken =  envFile.get('misc','MarkerToken')
        
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
        
    @property
    def MarkerToken(self):
        return self._MarkerToken        
# -*- coding: utf-8 -*-
"""
Created on Wed Oct 10 12:34:16 2018

@author: pablo
"""


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
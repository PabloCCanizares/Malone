
#include "Options.h"
#include "Auxiliars.h"
#include "Malone.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include "popen_noshell.h"
#include "configuration.h"
#define SLEEP_FOR_DEBUG 0


//TODO: No me mola que archivo tenga 1400 lineas. PArtir en trozos: tests, mutantes, etc.
//Dejar common lo que de verdad sea common.
//#define TEMP_DEBUG_AUX 1    
//-------------------------------------------------------

void free_test(T_stTestInfo* pTest)
{

    if(pTest != NULL)
    {
        pTest->dEndTick = pTest->dInitTick = pTest->dTime = -1.0;

        free(pTest);
        pTest = NULL;
    }
}
void free_mutant(T_stMutant* pMutant)
{
    if (DEBUG_AUX) printf("<%d>free_mutant - Init\n", m_nRank);
    if(pMutant != NULL)
    {
        pMutant->nNumber=pMutant->nState=pMutant->nTestKiller = -1;
        if(pMutant->strDescription != NULL)
        {
            free(pMutant->strDescription);  
            pMutant->strDescription = NULL;
        }        
        for(int i=0;i<pMutant->oTestList.nElems;i++)
        {
            T_stTestInfo* pTest;
            
            pTest = pMutant->oTestList.tests[i];
            //free_test(pTest);  TODO: Buscar posible doble free de tests          
            pMutant->oTestList.tests[i] = NULL;
        }        
        
        //TODO: Falta liberar los tests
        free(pMutant);
        pMutant = NULL;           
    } 
    if (DEBUG_AUX) printf("<%d>free_mutant - End\n", m_nRank);
}
/**
 * Free the auxiliar variables
 */
void free_auxiliars()
{
    if (DEBUG_AUX) printf("<%d>free_auxiliars - Init\n", m_nRank);
    //Free the mutant list
    for (int i = 0; i < m_oMutantList.nElems; i++) {
        T_stMutant* pMutant;
                
        pMutant =  m_oMutantList.array[i];
        free_mutant(pMutant); 
        m_oMutantList.array[i] = NULL;                 
    }
    
    //Free the env values.
    free_envfile(m_stEnvValues);     
    
    if(m_pResList != NULL)
    {
        for (int i = 0; i < m_pResList->nElems; i++) 
        {
            T_stTestInfo* pTest;
            
            pTest = m_pResList->tests[i];
            if(pTest != NULL)
                free_test(pTest);
            
            m_pResList->tests[i] = NULL;
        }
        free(m_pResList);
        m_pResList = NULL;    
    }
    
    if(m_pTestList != NULL)
    {
        for (int i = 0; i < m_pTestList->nElems; i++) 
        {
            T_stTestInfo* pTest;
            
            pTest = m_pTestList->tests[i];
            if(pTest != NULL)
                free_test(pTest);
            
            m_pTestList->tests[i] = NULL;
        }
        free(m_pTestList);
        m_pTestList = NULL;    
    }  
    
    //TODO:
    //free_conffile
    
    if (DEBUG_AUX) printf("<%d>free_auxiliars - End\n", m_nRank);
}

void initialize_auxiliars() {
    int i, j;

    m_nDebugMode = 0;
    m_nCompilationEnabled = 0;
    m_nRedirectToDiskEnabled = 0;

    m_lGenInitTick = m_lGenEndTick = m_lCompInitTick = m_lCompEndTick = 0;
    m_lOriginalInitTick = m_lOriginalEndTick = m_lMutantInitTick = m_lMutantEndTick = m_lEndTick = 0;
    m_lInitializeEndTick = 0;
    m_lEquivInitTick = m_lEquivEndTick = m_lSortInitTick = m_lSortEndTick=0;
    m_stEnvValues = NULL;
    m_stConfigValues = NULL;
    m_strResFolder = NULL;

    m_oMonitorLines.nElems = m_oMonitorLines.nAlarms = 0;
    m_oMonitorOnceLines.nElems = m_oMonitorOnceLines.nAlarms = 0;
    m_oMonitorLines.nMax = INITIAL_MONITOR_LINES;
    m_oMonitorOnceLines.nMax = INITIAL_MONITOR_ONCE_LINES;

    m_oMutantList.nElems = 0;
    m_oMutantList.nMax = 0;
    m_oMutantList.nDead = 0;
    m_oMutantList.dTime = 0.0;

    for (i = 0; i < MAX_MUTANTS; i++) {
        m_oMutantList.array[i] = NULL;
    }

    m_pTestList = malloc(sizeof (T_stTestList));
    m_pTestList->nElems = 0;
    m_pTestList->nMax = MAX_TESTS;
    for (i = 0; i < MAX_TESTS; i++) {
        m_pTestList->tests[i] = NULL;
    }

    if (m_nRank == MALONE_MASTER) {
        m_pResList = malloc(sizeof (T_stTestList));
        m_pResList->nElems = 0;
        m_pResList->nMax = MAX_TESTS;
        for (i = 0; i < MAX_TESTS; i++) {
            m_pResList->tests[i] = NULL;
        }
    } else
        m_pResList = NULL;

    for (i = 0; i < MAX_TESTS; i++) {
        m_oReorderIndexTest[i] = -1;
    }
    for (i = 0; i < MAX_TESTS; i++) {
        m_oSortedIndexTest[i] = -1;
    }

    //Initialize execution map
    m_oTestExecMap.nMutants = 0;
    m_oTestExecMap.nTests = 0;
    m_oTestExecMap.nCollapsed = 0;
    for (i = 0; i < MAX_MUTANTS; i++) {
        for (j = 0; j < MAX_TESTS; j++) {
            m_oTestExecMap.oMap[i][j] = -1;
        }
    }

    if (DEBUG_AUX) printf("initialize_auxiliars - Initialized!\n");
}

void initializeExecutionMap(int nMutants, int nTests) {
    int i;
    m_oTestExecMap.nMutants = nMutants;
    m_oTestExecMap.nTests = nTests;
    m_oTestExecMap.nRemainingMutants = nMutants;

    for (i = 0; i <= nMutants; i++) {
        T_stMutantExecution* pExe = (T_stMutantExecution*) malloc(sizeof (T_stMutantExecution));
        pExe->nFinished = 0;
        pExe->nIndexTest = 0;
        pExe->nManaged = 0;
        pExe->nEquivalent = 0;
        pExe->nIsClustered = 0;
        m_oTestExecMap.oMutantMap[i] = pExe;
        m_oTestExecMap.oMd5Map[i] = NULL;
    }
}

void initializeEquivalentMap(int nMutants, int nTests) {
    int i, j;

    if (DEBUG_AUX) printf("initializeEquivalentMap - Initializing equivalent map\n");
    m_oTestExecMap.nEquivalents = 0;
    m_oTestExecMap.nDupped = 0;
    m_oTestExecMap.nCollapsed = 0;
    for (i = 0; i <= nMutants; i++) {
        T_stEquivalentInfo* pEq = (T_stEquivalentInfo*) malloc(sizeof (T_stEquivalentInfo));

        pEq->nIsClusterHead = 0;
        pEq->nIsEnabled = 0;
        pEq->nKillerTest = -1;
        pEq->nMutantHeader = -1;
        pEq->nClones = 0;
        pEq->nMaxClones = 0;
        pEq->pClones = NULL;

        m_oTestExecMap.oEquivalentInfo[i] = pEq;
        //m_oTestExecMap.oEquivalentClusterHead[i] = -1;
    }
    if (DEBUG_AUX) printf("initializeEquivalentMap - End\n");
}

void redirect_stdout(char* strFile) {
    int nFile;

    if (DEBUG_AUX) printf("redirect_stdout - Init\n");
    if (strFile != NULL) {
        nFile = open(strFile, O_RDWR | O_CREAT | O_APPEND, 0600);
        if (nFile != -1) {
            if (DEBUG_AUX) printf("redirect_stdout - Redirecting log to file %s\n", strFile);
            dup2(nFile, 1); // make stdout go to file
        } else
            if (DEBUG_AUX) printf("ERROR opening log file!!%s\n", strFile);
    } else
        if (DEBUG_AUX) printf("redirect_stdout - Redirect variable is empty!!\n");

    if (DEBUG_AUX) printf("redirect_stdout - End\n");
}

int getenv_int(char* strKey) {
    int nRet;
    char* strResult;

    nRet = 0;
    strResult = getenv(strKey);
    if (strResult != NULL)
        nRet = atoi(strResult);

    if (DEBUG_AUX) printf("getenv_int - The key %s, has the value %d\n", strKey, nRet);
    return nRet;
}

long int getTick() {
    struct timespec ts;
    unsigned theTick = 0U;
    clock_gettime(CLOCK_REALTIME, &ts);
    theTick = ts.tv_nsec / 1000000;
    theTick += ts.tv_sec * 1000;
    return theTick;
}

void addResultIndex(T_stTestList* pTestList, int nIndex, const char *t, double dTime, int nKill) {
    T_stTestInfo* pInfo;
    if (pTestList != NULL && nIndex < MAX_TESTS) {
        pInfo = malloc(sizeof (T_stTestInfo));
        pInfo->nKill = nKill;
        pInfo->dTime = dTime;
        pInfo->nTest = nIndex;
        bzero(pInfo->res, MAX_RESULT_SIZE);
        strcpy(pInfo->res, t);
        pTestList->tests[nIndex] = pInfo;
        pTestList->nElems++;
    } else {
        if (pTestList == NULL)
            printf("<%d>addResult - WARNING!! List NULL pointer error\n", m_nRank);

        else if (pTestList->nElems >= MAX_TESTS) {
            printf("<%d>addResult - WARNING!! Limit of tests exceeded (%d vs %d)\n", m_nRank, pTestList->nElems, MAX_TESTS);
        }
    }
}

int addResult(T_stTestList* pTestList, const char *t, double dTime, int nKill) {
    T_stTestInfo* pInfo;
    int nRet;

    nRet = 0;
    if (pTestList != NULL && pTestList->nElems < MAX_TESTS) {
        pInfo = malloc(sizeof (T_stTestInfo));
        pInfo->nKill = nKill;
        pInfo->dTime = dTime;
        pInfo->nTest = pTestList->nElems;
        bzero(pInfo->res, MAX_RESULT_SIZE);
        strcpy(pInfo->res, t);
        pTestList->tests[pTestList->nElems] = pInfo;
        pTestList->nElems++;
        nRet = 1;
    } else {
        if (pTestList == NULL)
            printf("<%d>addResult - WARNING!! List NULL pointer error\n", m_nRank);

        else if (pTestList->nElems >= MAX_TESTS) {
            printf("<%d>addResult - WARNING!! Limit of tests exceeded (%d vs %d)\n", m_nRank, pTestList->nElems, MAX_TESTS);
        } else {
            printf("<%d>addResult - WARNING!! Unknown behavior\n", m_nRank);
        }
    }
    return nRet;
}
// return a new string with every instance of ch replaced by repl //

char* replace_char(const char *s, char ch, const char *repl) {
    int count = 0;
    const char *t;
    for (t = s; *t; t++)
        count += (*t == ch);

    size_t rlen = strlen(repl);
    char *res = malloc(strlen(s) + (rlen - 1) * count + 1);
    char *ptr = res;
    for (t = s; *t; t++) {
        if (*t == ch) {
            memcpy(ptr, repl, rlen);
            ptr += rlen;
        } else {
            *ptr++ = *t;
        }
    }
    *ptr = 0;
    return res;
}

char * replace(char const * const original, char const * const pattern, char const * const replacement) {
    size_t const replen = strlen(replacement);
    size_t const patlen = strlen(pattern);
    size_t const orilen = strlen(original);

    size_t patcnt = 0;
    const char * oriptr;
    const char * patloc;

    // find how many times the pattern occurs in the original string
    for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen) {
        patcnt++;
    }

    {
        // allocate memory for the new string
        size_t const retlen = orilen + patcnt * (replen - patlen);
        char * const returned = (char *) malloc(sizeof (char) * (retlen + 1));

        if (returned != NULL) {
            // copy the original string, 
            // replacing all the instances of the pattern
            char * retptr = returned;
            for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen) {
                size_t const skplen = patloc - oriptr;
                // copy the section until the occurence of the pattern
                strncpy(retptr, oriptr, skplen);
                retptr += skplen;
                // copy the replacement 
                strncpy(retptr, replacement, replen);
                retptr += replen;
            }
            // copy the rest of the string.
            strcpy(retptr, oriptr);
        }
        return returned;
    }
}
//TODO: Mejorar linea!

char* buildExeLine_StandAlone(int nIndexMutant, int nIndexTest, int nOriginalMode) {
    char* strExe, *strAux, *strAux2, *strPathMutantAlias, *strIndexMutantAlias, *strIndexTestAlias, *strPathOriginalAlias;
    char* strTest;
    char strMutantId[1000];
    char strTestId[1000];
    int nLen, nPathMutantAliasLen, nIndexMutantAliasLen, nIndexTestAliasLen, nOriginalAliasLen;
    T_stTestInfo* pTestInfo;

    //DANGER: Uno de los sitios que he tocado el dia 3/09/18 es aqui, si hay segmentation faults, mirar aqui. Tambien hemos tocado la config
    //generando una nueva lista de environments, basicamente partiendo los command line y execution line en dos, etc.
    nLen = nPathMutantAliasLen = nIndexMutantAliasLen = nIndexTestAliasLen = 0;
    strAux = strAux2 = NULL;
    if (m_pTestList && (nIndexTest < m_pTestList->nElems)) {
        //Convert mutant and test id to text
        sprintf(strMutantId, "%d", nIndexMutant);
        sprintf(strTestId, "%d", nIndexTest);

        nPathMutantAliasLen = strlen(PATH_MUTANT_ALIAS);
        nIndexMutantAliasLen = strlen(INDEX_MUTANT_ALIAS);
        nIndexTestAliasLen = strlen(INDEX_TEST_ALIAS);
        nOriginalAliasLen = strlen(PATH_ORIGINAL_ALIAS);

        strPathMutantAlias = malloc(sizeof (char)*nPathMutantAliasLen + 1);
        strIndexMutantAlias = malloc(sizeof (char)*nIndexMutantAliasLen + 1);
        strIndexTestAlias = malloc(sizeof (char)*nIndexTestAliasLen + 1);
        strPathOriginalAlias = malloc(sizeof (char)*nOriginalAliasLen + 1);

        sprintf(strPathMutantAlias, "%s", PATH_MUTANT_ALIAS);
        sprintf(strIndexMutantAlias, "%s", INDEX_MUTANT_ALIAS);
        sprintf(strIndexTestAlias, "%s", INDEX_TEST_ALIAS);
        sprintf(strPathOriginalAlias, "%s", PATH_ORIGINAL_ALIAS);

        pTestInfo = m_pTestList->tests[nIndexTest];

        if (pTestInfo != NULL) {
            strTest = malloc(sizeof (char)*strlen(&pTestInfo->res[0]) + 1);
            strcpy(strTest, &pTestInfo->res[0]);

            if (nOriginalMode) {
                nLen = strlen(m_stEnvValues->strExecLineOriginal) + strlen(strTest);
                strAux2 = malloc(sizeof (char)*nLen + 1);
                sprintf(strAux2, "%s%s", m_stEnvValues->strExecLineOriginal, strTest);
            } else {
                nLen = strlen(m_stEnvValues->strExecLineMutants) + strlen(strTest);
                strAux2 = malloc(sizeof (char)*nLen + 1);
                sprintf(strAux2, "%s%s", m_stEnvValues->strExecLineMutants, strTest);
            }

            //total size of strAux and strAux2:            
            strAux = replace(strAux2, strPathMutantAlias, m_stEnvValues->strMutantsPath);
            free(strAux2);
            strAux2 = replace(strAux, strPathOriginalAlias, m_stEnvValues->strAppPath);
            free(strAux);
            strAux = replace(strAux2, strIndexMutantAlias, strMutantId);
            free(strAux2);
            strAux2 = replace(strAux, strIndexTestAlias, strTestId);
            free(strAux);
            if (DEBUG_AUX) printf("Build line: %s\n", strAux2);
        }

        //free variables
        free(strPathMutantAlias);
        free(strIndexMutantAlias);
        free(strIndexTestAlias);
        free(strTest);
    }
    else
    {
        printf("buildExeLine_StandAlone - Error creating line %d, there exist some problem in the TESTSUITE\n", nIndexTest);
        
        if(m_pTestList == NULL)
        {
            printf("buildExeLine_StandAlone - ERROR, The test suite is NULL\n");
        }
        else if(nIndexTest >= m_pTestList->nElems)
        {
            printf("buildExeLine_StandAlone - ERROR, the requested index is greater or equal than the maximum [%d >= %d]", nIndexTest, m_pTestList->nElems);
        }
        else
        {
            printf("buildExeLine_StandAlone - ERROR, pero ni idea de cual \n");
        }
    }

    return strAux2;
}
char* buildExeLine_general(int nIndexMutant, int nIndexTest, int nOriginalMode) {
    char* strExe, *strAux, *strAux2;
    char strIndexMutant[200];
    char strIndexTest[200];
    char buffer[2000];
    int nLen;

    strAux2 = NULL;
    if (m_stEnvValues != NULL) {
        bzero(buffer, MAX_EXELINE);
        nLen = sprintf(buffer, "%s/%d/bin/mut.exe %d", m_stEnvValues->strMutantsPath, nIndexMutant, nIndexTest);

        strAux2 = malloc(sizeof (char)*nLen);
        strcpy(strAux2, buffer);
    }

    return strAux2;
}

char* buildExeLine(int nIndexMutant, int nIndexTest, int nOriginalMode) {
    char* strRet;

    strRet = NULL;

    if (m_stEnvValues != NULL) {
        if (m_stEnvValues->nStandalone != 1) {
            strRet = buildExeLine_general(nIndexMutant, nIndexTest, nOriginalMode);
        } else {
            strRet = buildExeLine_StandAlone(nIndexMutant, nIndexTest, nOriginalMode);
        }
    } else
        printf("<%d> buildExeLine - WARNING! Environment values NULL\n", m_nRank);

    return strRet;
}

char* buildEquivLine(int nIndexMutant, int nOriginalMode) {
    char *strAux, *strAux2, *strPathMutantAlias, *strIndexMutantAlias, *strPathOriginalAlias;
    char strMutantId[1000];
    int nLen, nPathMutantAliasLen, nIndexMutantAliasLen, nOriginalAliasLen;

    nLen = nPathMutantAliasLen = nIndexMutantAliasLen = 0;
    strAux = strAux2 = NULL;

    if (nIndexMutant <= m_stEnvValues->nTotalMutants) {
        //Convert mutant and test id to text
        sprintf(strMutantId, "%d", nIndexMutant);
        nPathMutantAliasLen = strlen(PATH_MUTANT_ALIAS);
        nIndexMutantAliasLen = strlen(INDEX_MUTANT_ALIAS);
        nOriginalAliasLen = strlen(PATH_ORIGINAL_ALIAS);

        strPathMutantAlias = malloc(sizeof (char)*nPathMutantAliasLen + 1);
        strIndexMutantAlias = malloc(sizeof (char)*nIndexMutantAliasLen + 1);
        strPathOriginalAlias = malloc(sizeof (char)*nOriginalAliasLen + 1);

        sprintf(strPathMutantAlias, "%s", PATH_MUTANT_ALIAS);
        sprintf(strIndexMutantAlias, "%s", INDEX_MUTANT_ALIAS);
        sprintf(strPathOriginalAlias, "%s", PATH_ORIGINAL_ALIAS);

        if (nOriginalMode) {
            nLen = strlen(m_stEnvValues->strAppPath) + sizeof ('/') + strlen(m_stEnvValues->strAppName);
            strAux = malloc(sizeof (char)*nLen + 1);
            sprintf(strAux, "%s/%s", m_stEnvValues->strAppPath, m_stEnvValues->strAppName);
            
        } else {
            nLen = strlen(m_stEnvValues->strMutantsPath) + sizeof ('/') + strlen(strIndexMutantAlias) + sizeof ('/') + strlen(m_stEnvValues->strAppName);
            strAux2 = malloc(sizeof (char)*nLen + 1);
            sprintf(strAux2, "%s/%s/%s", m_stEnvValues->strMutantsPath, strIndexMutantAlias, m_stEnvValues->strAppName);
            
            //total size of strAux and strAux2:            
            strAux = replace(strAux2, strPathMutantAlias, m_stEnvValues->strMutantsPath);
            free(strAux2);
            strAux2 = replace(strAux, strPathOriginalAlias, m_stEnvValues->strAppPath);
            free(strAux);
            strAux = replace(strAux2, strIndexMutantAlias, strMutantId);
            free(strAux2);
        }

        if (DEBUG_AUX) printf("Build line with %d mode: %s\n", nOriginalMode, strAux);

        //free variables
        if (strPathMutantAlias != NULL)
            free(strPathMutantAlias);
        if (strIndexMutantAlias != NULL)
            free(strIndexMutantAlias);
    }

    if (DEBUG_AUX) printf("buildEquivLine - End (%d mode)\n", nOriginalMode);
    
    return strAux;
}

char* generateCompilationLine(int nIndexMutant, int nOriginalMode) {
    char* strExe, *strAux, *strAux2, *strPathMutantAlias, *strIndexMutantAlias, *strPathOriginalAlias;
    char* strTest;
    char strMutantId[1000];
    int nLen, nPathMutantAliasLen, nIndexMutantAliasLen, nOriginalAliasLen;

    //DANGER: Uno de los sitios que he tocado el dia 3/09/18 es aqui, si hay segmentation faults, mirar aqui. Tambien hemos tocado la config
    //generando una nueva lista de environments, basicamente partiendo los command line y execution line en dos, etc.
    nLen = nPathMutantAliasLen = nIndexMutantAliasLen = 0;
    strAux = strAux2 = NULL;

    if (nIndexMutant < m_stEnvValues->nTotalMutants) {
        //Convert mutant and test id to text
        sprintf(strMutantId, "%d", nIndexMutant);
        nPathMutantAliasLen = strlen(PATH_MUTANT_ALIAS);
        nIndexMutantAliasLen = strlen(INDEX_MUTANT_ALIAS);
        nOriginalAliasLen = strlen(PATH_ORIGINAL_ALIAS);

        strPathMutantAlias = malloc(sizeof (char)*nPathMutantAliasLen + 1);
        strIndexMutantAlias = malloc(sizeof (char)*nIndexMutantAliasLen + 1);
        strPathOriginalAlias = malloc(sizeof (char)*nOriginalAliasLen + 1);

        sprintf(strPathMutantAlias, "%s", PATH_MUTANT_ALIAS);
        sprintf(strIndexMutantAlias, "%s", INDEX_MUTANT_ALIAS);
        sprintf(strPathOriginalAlias, "%s", PATH_ORIGINAL_ALIAS);

        if (nOriginalMode) {
            nLen = strlen(m_stEnvValues->strCompLineOriginal);
            strAux2 = malloc(sizeof (char)*nLen + 1);
            sprintf(strAux2, "%s", m_stEnvValues->strCompLineOriginal);
        } else {
            nLen = strlen(m_stEnvValues->strCompLineMutants);
            strAux2 = malloc(sizeof (char)*nLen + 1);
            sprintf(strAux2, "%s", m_stEnvValues->strCompLineMutants);
            printf("Initial destroy: %s\n", strAux2);
        }

        //total size of strAux and strAux2:            
        strAux = replace(strAux2, strPathMutantAlias, m_stEnvValues->strMutantsPath);
        free(strAux2);
        strAux2 = replace(strAux, strPathOriginalAlias, m_stEnvValues->strAppPath);
        free(strAux);
        strAux = replace(strAux2, strIndexMutantAlias, strMutantId);
        free(strAux2);

        if (DEBUG_AUX) printf("Build line with %d mode: %s\n", nOriginalMode, strAux);

        //free variables
        if (strPathMutantAlias != NULL)
            free(strPathMutantAlias);
        if (strIndexMutantAlias != NULL)
            free(strIndexMutantAlias);
    }

    return strAux;
}

int isMutantAllocated(int nIndexMutant) {
    int nRet;

    if (DEBUG_AUX) printf("<%d>isMutantAllocated - Checking allocation of mutant %d\n", m_nRank, nIndexMutant);

    if (nIndexMutant >= sizeof(m_oMutantList.array) || m_oMutantList.array[nIndexMutant] == NULL)
        nRet = 0;
    else {
        T_stMutant* pMutant;

        pMutant = m_oMutantList.array[nIndexMutant];
        if (pMutant) {
            nRet = 1;
        }
        else
            printf("<%d>isMutantAllocated - The mutant (%d) stored in the array is NULL!\n", m_nRank, nIndexMutant);
    }

    if (DEBUG_AUX) printf("<%d>isMutantAllocated - Checking allocation End, code ret: %d\n", m_nRank, nRet);
    return nRet;
}

int allocMutant(int nIndexMutant) {
    int nRet;
    T_stMutant* pMutantInfo;

    if (!isMutantAllocated(nIndexMutant)) {
        if (DEBUG_AUX) printf("<%d>allocMutant - Allocating mutant: %d at pos : %d\n", m_nRank, nIndexMutant, nIndexMutant);

        pMutantInfo = (T_stMutant*) malloc(sizeof (T_stMutant));
        pMutantInfo->nNumber = nIndexMutant;
        pMutantInfo->nState = ALIVE;
        pMutantInfo->nTestKiller = -1;
        pMutantInfo->oTestList.nElems = 0;
        pMutantInfo->strDescription = NULL;
        
        for (int i = 0; i < MAX_TESTS; i++) {
            pMutantInfo->oTestList.tests [i] = NULL;
        }
        m_oMutantList.array[nIndexMutant] = pMutantInfo;
        m_oMutantList.nElems++;
        if (DEBUG_AUX) printf("<%d>allocMutant - Total mutants allocated: %d\n", m_nRank, m_oMutantList.nElems);
    }

    return nRet;
}

void insertMutant(T_stMutant* pMutant, int nIndexMutant) {
    if (DEBUG_AUX) printf("<%d>insertMutant - Inserting mutant at pos: %d\n", m_nRank, nIndexMutant);
    if (pMutant && nIndexMutant >= 0) {
        pMutant->nNumber = nIndexMutant;
        if (pMutant->nState == 0)
            m_oMutantList.nDead++;

        m_oMutantList.array[nIndexMutant] = pMutant;
        m_oMutantList.nElems++;
    }
}

void insertMutantTestByTest(T_stMutant* pMutant, int nIndexMutant, int nWorkerSource) {

    int nTests, nIndexTest;
    T_stTestInfo* pTest;
    if (pMutant != NULL) {
        if (DEBUG_AUX) printf("<%d>insertMutantTestByTest - Inserting mutant %d at pos: %d\n", m_nRank, pMutant->nNumber, nIndexMutant);

        //If the mutants is not allocated, do it!
        if (!isMutantAllocated(nIndexMutant))
            allocMutant(nIndexMutant);

        nTests = nIndexTest = 0;
        if (pMutant && nIndexMutant >= 0) {
            pMutant->nNumber = nIndexMutant;

            nTests = pMutant->oTestList.nElems;
            if (DEBUG_AUX) printf("<%d>insertMutantTestByTest - The inserting mutant %d has %d tests\n", m_nRank, nIndexMutant, nTests);

            if (DEBUG_AUX) printf("<%d>---->\n", m_nRank);
            for (int i = 0; i < nTests; i++) {
                pTest = pMutant->oTestList.tests[i];

                if (pTest != NULL) {

                    nIndexTest = pTest->nTest;
                    printTest(pTest);
                    if (DEBUG_AUX) printf("<%d>insertMutantTestByTest - Inserting test (%d) -> at(%d)\n", m_nRank, i, nIndexTest);
                    
                    if(insertTestResult(nIndexMutant, nIndexTest, pTest))
                    {
                        //Update the execution Map
                        m_oTestExecMap.oMap[nIndexMutant][nIndexTest] = nWorkerSource;
                        if (DEBUG_AUX) printf("<%d>insertMutantTestByTest - Associating duple [Mutant: %d - Test: %d] to worker %d\n", m_nRank, nIndexMutant, nIndexTest, nWorkerSource);
                    }
                    else
                    {
                        printf("<%d>insertMutantTestByTest - ERROR inserting test %d into mutant %d\n", m_nRank, nIndexTest, nIndexMutant);
                    }
                }
            }
            if (DEBUG_AUX) printf("<%d><----\n", m_nRank);
        }
    }
    if (DEBUG_AUX) printf("<%d>insertMutantTestByTest - End\n", m_nRank);
}

int insertTestResult(int nMutant, int nTest, T_stTestInfo* pTest) {
    int nRet;
    T_stMutant* pCheckMutant;
    nRet = 0;
    if (DEBUG_AUX) printf("<%d>insertTestResult - Init\n", m_nRank);
    //If the mutant is allocated in memory
    if (pTest != NULL && isMutantAllocated(nMutant) &&  nMutant < MAX_MUTANTS && nTest < MAX_TESTS && nTest>=0) {
        //Ensure that the mutant is allocated ...
        
        pCheckMutant = m_oMutantList.array[nMutant];
        if (pCheckMutant != NULL) {

            pTest->nTest = nTest;
            if (DEBUG_AUX) printf("<%d>insertTestResult - Inserting test number %d  on mutant %d\n", m_nRank, nTest, nMutant);

            //Check if the test already exist!
            if (pCheckMutant->oTestList.tests[nTest] != NULL)
                if (DEBUG_AUX) printf("<%d>insertTestResult - The inserting test number %d already exists  on mutant %d\n", m_nRank, nTest, nMutant);

            pCheckMutant->oTestList.tests[nTest] = pTest;
            pCheckMutant->oTestList.nElems++;
            m_oMutantList.dTime += pTest->dTime;
            if (DEBUG_AUX) printf("<%d>insertTestResult - Total list: %d\n", m_nRank, m_oMutantList.nElems);
            if (pTest->nKill) {
                if (pCheckMutant->nState != DEAD) {
                    pCheckMutant->nState = DEAD;
                    pCheckMutant->nTestKiller = nTest;
                    m_oMutantList.nDead++;
                }

                if (DEBUG_AUX) printf("<%d>insertTestResult - Mutant %d Killed by test %d!!\n", m_nRank, nMutant, nTest);
            }
            
            //Mark as pass
            nRet =1;
        }

    } else {
        if(pTest == NULL)
            printf("<%d> Warning!! The input test is null!! Mutant (M:%d|T:%d) data not inserted!\n", m_nRank, nMutant, nTest);
        else if(nMutant > sizeof(m_oMutantList.array))
            printf("<%d> Warning!! The mutant index (%d) is greater than the capacity of the array (%d)\n", m_nRank, nMutant, sizeof(m_oMutantList.array));
        else            
            printf("<%d> Warning!! Mutant (M:%d|T:%d) data not inserted!\n", m_nRank, nMutant, nTest);
    }
    if (DEBUG_AUX) printf("<%d>insertTestResult - End\n", m_nRank);

    return nRet;
}

/**
 * This function is designed to insert a test in the first free position in the vector. The main diference with insertResult
 * is related with the order. In this case we can have a test vector such [t2,t3,t5] since with insertResult we'd have [null,null,t2,t3,null,t5].
 * We have designed it to achieve a better performance
 * @param nMutant: Mutant where the test will be inserted
 * @param pTest: Test to insert
 * @return 0 if the mutant is not allocated
 */
int insertTestResult_unsorted(int nMutant, int nTest, T_stTestInfo* pTest) {
    int nRet;
    T_stMutant* pCheckMutant;
    nRet = 0;
    if (DEBUG_AUX) printf("<%d>insertTestResult_unsorted - Init\n", m_nRank);
    //If the mutant is allocated in memory
    if (pTest != NULL && isMutantAllocated(nMutant) && nMutant < MAX_MUTANTS && nTest < MAX_TESTS && nTest>=0) {
        //Ensure that the mutant is allocated ...
        pCheckMutant = m_oMutantList.array[nMutant];
        if (pCheckMutant != NULL) {

            pTest->nTest = nTest;
            if (DEBUG_AUX) printf("<%d>insertTestResult_unsorted - Inserting test number %d  on mutant %d\n", m_nRank, nTest, nMutant);

            pCheckMutant->oTestList.tests[pCheckMutant->oTestList.nElems] = pTest;
            pCheckMutant->oTestList.nElems++;
            m_oMutantList.dTime += pTest->dTime;
            if (DEBUG_AUX) printf("<%d>insertTestResult_unsorted - Total list: %d\n", m_nRank, m_oMutantList.nElems);
            if (pTest->nKill) {
                if (pCheckMutant->nState != DEAD) {
                    pCheckMutant->nState = DEAD;
                    pCheckMutant->nTestKiller = nTest;
                    m_oMutantList.nDead++;
                }

                if (DEBUG_AUX) printf("<%d>insertTestResult_unsorted - Mutant %d Killed by test %d!!\n", m_nRank, nMutant, nTest);
            }
            nRet = 1;
        }
    } else {
        printf("<%d> insertTestResult_unsorted - Warning!! Mutant data not inserted!\n", m_nRank);
    }
    if (DEBUG_AUX) printf("<%d>insertTestResult_unsorted - End\n", m_nRank);

    return nRet;
}
//Deprecated

void insertFullMutantInfo(int nMutant, int nMutantState, T_stTestList* pList) {
    T_stMutant* pMutantInfo;
    char* strDescription;

    if (DEBUG_AUX)
        printf("insertFullMutantInfo - Inserting mutant: %d at pos : %d | State: %d", nMutant, m_oMutantList.nElems, nMutantState);

    pMutantInfo = malloc(sizeof (T_stMutant));
    pMutantInfo->nNumber = nMutant;

    if (nMutantState == RES_MUTANT_ALIVE)
        pMutantInfo->nState = ALIVE;
    else {
        pMutantInfo->nState = DEAD;
        pMutantInfo->nTestKiller = nMutantState;
    }

    //En este punto habría que extraer la última línea del código del mutante
    //donde hay una breve descripción del mutante.

    strcpy(pMutantInfo->strDescription, strDescription);
    m_oMutantList.array[m_oMutantList.nElems] = pMutantInfo;
    m_oMutantList.nElems++;
}

int checkTestResult(T_stTestInfo* pTest) {
    int nRet, nTest;

    if (DEBUG_AUX) printf("<%d>checkTestResult - Init\n", m_nRank);
    nRet = nTest = 0;
    if (pTest) {
         if (DEBUG_AUX) printf("<%d>checkTestResult - 1\n", m_nRank);
        if (m_stEnvValues && m_stEnvValues->nSortTestSuite != 0) {
            nTest = m_oReorderIndexTest[pTest->nTest];
            if (DEBUG_AUX) printf("Receiving test %d - Sorted in position %d\n", pTest->nTest, nTest);
        } else
            nTest = pTest->nTest;

         if (DEBUG_AUX) printf("<%d>checkTestResult - 2\n", m_nRank);
        nRet = checkResult(nTest, pTest->res);
        if (DEBUG_AUX) printf("<%d>checkTestResult - 3\n", m_nRank);
    }

    if (DEBUG_AUX) printf("<%d>checkTestResult - End\n", m_nRank);
    return nRet;
}

//TODO: pabloSays: Considero que es innecesario leer esto de disco en una estructura intermedia
//y mas cuando es estatica, hay que eliminar lo estatico comos sea.
int readTestSuite() {

    char line[TESTSUITE_SIZE][LINE_SIZE];
    char* strFile;
    FILE *plist = NULL;
    int nRet, i, total;

    nRet = i = total = 0;

    if (DEBUG_AUX) printf("<%d>readTestSuite - Init\n", m_nRank);

    if (m_stEnvValues != NULL) {
        strFile = m_stEnvValues->strTestSuiteFile;
        if (strFile != NULL && strlen(strFile) > 0) {
            plist = fopen(strFile, "r");
            if (plist != NULL) {
                while (fgets(line[i], LINE_SIZE, plist) && i < m_stEnvValues->nTotalTests) {
                    // get rid of ending \n from fgets 
                    line[i][strlen(line[i]) - 1] = '\0';
                    i++;
                }
            }
        }

        if (DEBUG_AUX) printf("readTestSuite - Total lines readed: %d\n", i);
        total = i;
        nRet = 1;
        i = 0;
        while (i < total && i < m_stEnvValues->nTotalTests && nRet) {
            if (DEBUG_AUX && m_nRank != MALONE_MASTER)
                printf("<%d> Tc: %d - %s\n", m_nRank, i, line[i]);
            nRet = addResult(m_pTestList, line[i], 0, 0);
            i++;
        }

        if (nRet == 0) {
            printf("<%d>readTestSuite - Error doing malloc to test %d \n", m_nRank, (i - 1));
        } else
            nRet = i;

    }

    if (DEBUG_AUX) printf("readTestSuite - End with code %d \n", nRet);

    if (DEBUG_AUX) printf("readTestSuite - End\n");
    return nRet;
}

int checkTestResultOriginal(T_stTestInfo* pTest) {
    int nRet;

    nRet = 0;
    if (pTest && pTest->nTest != -1) {
        nRet = checkResultOriginal(pTest->nTest, pTest->res);
    }

    return nRet;
}

int checkResultOriginal(int nIndexTest, const char* strResult) {
    int nRet;

    if (DEBUG_AUX) printf("<%d>checkResultOriginal - Init\n", m_nRank);
    nRet = 0;

    if (m_pResList) {
        if (strResult != NULL) {
            if (strlen(strResult) > 0 && !strstr(strResult, "Error") && !strstr(strResult, "N!L") && strstr(strResult, getMarkerToken()) != NULL) {
                nRet = 1;
                if (DEBUG_AUX) printf("checkResultOriginal - Result obtained sucessfully\n");
            } else {
                if (DEBUG_AUX && strlen(strResult) == 0)
                    printf("checkResultOriginal - Result is empty: %s!!\n", strResult);
                else
                    printf("<%d>checkResultOriginal - Result strange: %s!! Returning code: %d\n", m_nRank, strResult, nRet);
            }
        } else
            if (DEBUG_AUX) printf("checkResultOriginal - Rank: %d | <T:%d> Res: %d - NULL value\n", m_nRank, nIndexTest, nRet);
    } else
        if (DEBUG_AUX) printf("checkResultOriginal - Test results list is NULL!!!\n");

    if (DEBUG_AUX) printf("checkResultOriginal - End\n");

    return nRet;
}

/**
 * Get the execution time needed to apply an specific test over the original program
 * @param nTest Identifier of the test whose execution time is requested.
 * @return 
 */
int getOriginalTime(T_stTestList* pList, int nTest) {
    int nRet;

    nRet = 1;

    if (pList && nTest < pList->nElems) {
        T_stTestInfo* pTest = pList->tests[nTest];

        if (pTest != NULL) {
            if (pTest->dTime > 0)
                nRet = ceil(pTest->dTime / 1000.0);
            else
                nRet = 0;
        }
    }
    return nRet;
}

int checkResult(int nIndexTest, const char* strResult) {
    int nRet;
    char* strOriginalRes;
    if (DEBUG_AUX) printf("checkResult - Init\n");
    nRet = 0;

    if (m_pResList) {
        if (strResult != NULL) {
            if (strlen(strResult) >= 0 && nIndexTest < m_pResList->nElems) {
                strOriginalRes = m_pResList->tests[nIndexTest]->res;
                if (strOriginalRes != NULL) {
                    nRet = (strcmp(strOriginalRes, strResult)) == 0;
                    if (DEBUG_AUX) {
                        printf("checkResult - Rank: %d | <T:%d> Res: %d - O: %s vs M: %s\n", m_nRank, nIndexTest, nRet, m_pResList->tests[nIndexTest]->res, strResult);
                        printf("checkResult: sizeO: %zd vs sizeM: %zd\n", strlen(m_pResList->tests[nIndexTest]->res), strlen(strResult));
                    }
                } else
                    printf("WARNING!!! Original res is empty!!");

            } else {
                if (DEBUG_AUX) printf("checkResult - Res len: %zd | %d vs %d \n", strlen(strResult), nIndexTest, m_pResList->nElems);
            }
        } else
            if (DEBUG_AUX) printf("checkResult - Rank: %d | <T:%d> Res: %d - NULL value\n", m_nRank, nIndexTest, nRet);
    } else
        if (DEBUG_AUX) printf("checkResult - Test results list is NULL!!!\n");

    if (DEBUG_AUX) printf("checkResult - End\n");

    return nRet;
}

T_stTestInfo* createTest(int nIndexTest, char* strResult, double dTime, int nKill) {
    T_stTestInfo* pTest;
    if (DEBUG_AUX) printf("createTest - Init\n");

    pTest = malloc(sizeof (T_stTestInfo));
    pTest->dTime = dTime;
    pTest->nKill = nKill;
    pTest->nTest = nIndexTest;
    bzero(pTest->res, MAX_RESULT_SIZE);

    if (strResult != NULL)
        strcpy(pTest->res, strResult);
    
    return pTest;
}

int prettyPrint(int nIndex, int nTotal, int nAntProg) {
    float progress = 0.0;
    int i, nPrintProgress;


    if (nTotal != 0) {
        progress = (float) nIndex / nTotal;
        if (progress != nAntProg) {
            //while (progress < 1.0) {
            int barWidth = 70;

            printf("[");
            int pos = barWidth * progress;
            for (i = 0; i < barWidth; ++i) {
                if (i < pos) printf("=");
                else if (i == pos) printf(">");
                else printf(" ");
            }
            nPrintProgress = progress * 100;
            printf("] %d perc | %d of %d Tests\r", nPrintProgress, nIndex, nTotal);
            //std::cout.flush();
            fflush(stdout);
        }
    }

    return progress;
}

void showDebugOptions() {
    char hostname[256];
    gethostname(hostname, sizeof (hostname));
    printf("<%d> Malone process initialized with PID %d on %s ready for attach\n", m_nRank, getpid(), hostname);
    fflush(stdout);

    if (SLEEP_FOR_DEBUG > 0)
        sleep(SLEEP_FOR_DEBUG);
}

int isEnabledAuxLog() {
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugAux : 0;
}

int file_exist(char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

/* test that dir exists (1 success, -1 does not exist, -2 not dir) */
int dir_exist(const char *d) {
    DIR *dirptr;
    int nRet;

    nRet = 1;
    if (access(d, F_OK) != -1) {
        // file exists
        if ((dirptr = opendir(d)) != NULL) {
            closedir(dirptr);
        } else {
            nRet = 0; // d exists, but not dir 
        }
    } else {
        nRet = 0; // d does not exist 
    }

    return nRet;
}

void recursive_mkdir(char *path) {
    char *sep = strrchr(path, '/');
    if (sep != NULL) {
        *sep = 0;
        recursive_mkdir(path);
        *sep = '/';
    }
    if (mkdir(path, 0755) && errno != EEXIST)
        printf("recursive_mkdir - error while trying to create '%s'\n", path);
}

FILE *fopen_mkdir(char *path, char *mode) {
    char *sep = strrchr(path, '/');
    if (sep) {
        char *path0 = strdup(path);
        path0[ sep - path ] = 0;
        recursive_mkdir(path0);
        free(path0);
    }
    return fopen(path, mode);
}

int generateResFolder() {
    int nRet, nLen;
    char* strMaloneHome;
    char filename[60], folderName[200];

    struct tm *timenow;
    time_t now = time(NULL);

    nRet = 1;

    //Get actual time
    timenow = gmtime(&now);
    strftime(filename, sizeof (filename), "malone___%Y-%m-%d_%H:%M:%S", timenow);
    sprintf(folderName, "_%s_m%d_t%d_w%d_a%d_%d%d%d%d%d%d",
            m_stEnvValues->strAppName, m_stEnvValues->nTotalMutants, m_stEnvValues->nTotalTests, m_nSize,
            m_nSelAlgorithm, m_stEnvValues->nDistributeOriginal, m_stEnvValues->nSortTestSuite, m_stEnvValues->nScatterWorkload,
            m_stEnvValues->nClusterMutants, m_stEnvValues->nParallelCompilation, m_stEnvValues->nParallelMd5sum);
    strMaloneHome = getenv("MALONE_HOME");
    nLen = strlen(strMaloneHome) + strlen(folderName) + strlen(PATH_RES_DIR) + strlen(filename) + 1;
    m_strResFolder = malloc((nLen + 1) * sizeof (char));
    sprintf(m_strResFolder, "%s%s%s%s/", strMaloneHome, PATH_RES_DIR, filename, folderName);

    printf("parseArguments - Res file is: %s\n", m_strResFolder);

    recursive_mkdir(m_strResFolder);
    if (m_strResFolder != NULL)
        nRet = 1;

    return nRet;
}

void copy(char *source, char *dest) {
    int childExitStatus;
    pid_t pid;
    int status;
    if (!source || !dest) {
        /* handle as you wish */
    }

    pid = fork();

    if (pid == 0) { /* child */
        execl("/bin/cp", "/bin/cp", source, dest, (char *) 0);
    } else if (pid < 0) {
        /* error - couldn't start process - you decide how to handle */
    } else {
        /* parent - wait for child - this has all error handling, you
         * could just call wait() as long as you are only expecting to
         * have one child process at a time.
         */
        pid_t ws = waitpid(pid, &childExitStatus, WNOHANG);
        if (ws == -1) { /* error - handle as you wish */
        }

        if (WIFEXITED(childExitStatus)) /* exit code in childExitStatus */ {
            status = WEXITSTATUS(childExitStatus); /* zero is normal exit */
            /* handle non-zero as you wish */
        } else if (WIFSIGNALED(childExitStatus)) /* killed */ {
        } else if (WIFSTOPPED(childExitStatus)) /* stopped */ {
        }
    }
}

void saveConfigAndEnvironmentFiles() {
    char* strConfigFile, *strEnvFile;
    int nLen;

    strConfigFile = strEnvFile = NULL;

    //Create the string for copying the config file
    nLen = strlen(m_strResFolder) + strlen(SAVE_CFG_FILE) + 1;
    strConfigFile = malloc((nLen + 1) * sizeof (char));
    sprintf(strConfigFile, "%s%s", m_strResFolder, SAVE_CFG_FILE);
    if (DEBUG_AUX) printf("saveConfigAndEnvironmentFiles - Config file to be saved: %s\n", strConfigFile);

    //Create the string for copying the config file
    nLen = strlen(m_strResFolder) + strlen(SAVE_ENV_FILE) + 1;
    strEnvFile = malloc((nLen + 1) * sizeof (char));
    sprintf(strEnvFile, "%s%s", m_strResFolder, SAVE_ENV_FILE);
    if (DEBUG_AUX) printf("saveConfigAndEnvironmentFiles - Config file to be saved: %s\n", strEnvFile);

    //Copy the files
    copy(m_strConfig, strConfigFile);
    copy(m_strEnvironment, strEnvFile);

    if (strConfigFile)
        free(strConfigFile);
    if (strEnvFile)
        free(strEnvFile);
}

char* execCommandLine(const char *fmt, ...) {
    va_list arg;
    char strCommand[1000];
    char* strRet, *line;
    int nAcc, nLineLen, nResLen, nNewLen;
    FILE *pp;
    #ifndef PERFORMANCE_MODE 
    long lInitTick, lEndTick, lTotal;    
    #endif   

    //Initialise
    nAcc = nNewLen = nResLen = 0;
    strRet = NULL;

    
    #ifndef PERFORMANCE_MODE 
    lInitTick = getTick();
    if (MALONE_MAIN_COMMAND && DEBUG_AUX)
        printf("<%d> execCommandLine - Init\n", m_nRank);
    #endif        

    va_start(arg, fmt);
    #ifndef PERFORMANCE_MODE 
    if (MALONE_MAIN_COMMAND && DEBUG_AUX)
        printf("<%d> execCommandLine - FMT: %s\n", m_nRank, fmt);
    #endif     
    vsprintf(strCommand, fmt, arg);
    va_end(arg);

    #ifndef PERFORMANCE_MODE     
    if (MALONE_MAIN_COMMAND)
        printf("<%d> execCommandLine - Executing command: %s\n", m_nRank, strCommand);
    #endif     

    pp = popen(strCommand, "r");

    if (pp != NULL) {
        char buf[100];

        //Redirect errors to null
        dup2(fileno(pp), 2);
        do {
            line = fgets(buf, sizeof (buf), pp);

            if (line != NULL) {
                #ifndef PERFORMANCE_MODE                     
                if (MALONE_MAIN_COMMAND && DEBUG_AUX)
                    printf("<%d> +line [%s] (%zd) bytes\n", m_nRank, line, strlen(line)); /* line includes '\n' */
                #endif                
                if (strRet == NULL)
                    nResLen = 0;
                else
                    nResLen = strlen(strRet);

                nLineLen = strlen(line);
                nNewLen = nResLen + nLineLen + 1;
                #ifndef PERFORMANCE_MODE                     
                if (MALONE_MAIN_COMMAND && DEBUG_AUX)
                    printf("> retLen: %d | lineLen: %d | newLen: %d\n", nResLen, nLineLen, nNewLen);
                #endif                
                if (strRet == NULL)
                    strRet = malloc(nNewLen);
                else
                    strRet = realloc(strRet, nNewLen);

                memcpy(strRet + nResLen, line, nLineLen + 1);
                #ifndef PERFORMANCE_MODE     
                if (MALONE_MAIN_COMMAND && DEBUG_AUX) printf("<%d> exeCommand - size: %d | Result: %s\n", m_nRank, nNewLen, strRet);
                #endif                
            } else {
                #ifndef PERFORMANCE_MODE                     
                if (strRet == NULL) {
                    if (MALONE_MAIN_COMMAND) printf("<%d> exeCommand - Executing: %s Result: null line!!\n", m_nRank, strCommand);
                }
                #endif                
            }

        } while (line != NULL);


        pclose(pp);
    } else {
    #ifndef PERFORMANCE_MODE          
        if (MALONE_MAIN_COMMAND) printf("<%d> exeCommand - pp null\n", m_nRank);
    #endif        
    }

    #ifndef PERFORMANCE_MODE    
    if (MALONE_MAIN_COMMAND) {
        lEndTick = getTick();
        lTotal = lEndTick - lInitTick;
        printf("<%d> exeCommand - size: %d | TotalTime: %ld |Result: %s \n", m_nRank, nNewLen, lTotal, strRet);
        printf("<%d> exeCommand - end\n", m_nRank);
    }
    #endif        
    return strRet;
}

char* execCommandLineHPC(const char *fmt, ...) {
    va_list arg;
    char strCommand[1000];
    char* strRet, *line;
    int nAcc, nLineLen, nResLen, nNewLen;
    FILE *pp;
    int status;
    struct popen_noshell_pass_to_pclose pclose_arg;
    #ifndef PERFORMANCE_MODE 
    long lInitTick, lEndTick, lTotal;    
    #endif   
    
    //Initialise
    nAcc = nNewLen = nResLen = 0;
    strRet = NULL;     

    va_start(arg, fmt);   
    vsprintf(strCommand, fmt, arg);
    va_end(arg);   

    pp = popen_noshell_compat(strCommand, "r", &pclose_arg);

    if (pp != NULL) {
        char buf[100];

        //Redirect errors to null
        dup2(fileno(pp), 2);
        do {
            line = fgets(buf, sizeof (buf), pp);

            if (line != NULL) {
                #ifndef PERFORMANCE_MODE                     
                if (MALONE_MAIN_COMMAND && DEBUG_AUX)
                    printf("<%d> +line [%s] (%zd) bytes\n", m_nRank, line, strlen(line)); /* line includes '\n' */
                #endif                
                if (strRet == NULL)
                    nResLen = 0;
                else
                    nResLen = strlen(strRet);

                nLineLen = strlen(line);
                nNewLen = nResLen + nLineLen + 1;
                #ifndef PERFORMANCE_MODE                     
                if (0 && DEBUG_AUX)
                    printf("> retLen: %d | lineLen: %d | newLen: %d\n", nResLen, nLineLen, nNewLen);
                #endif                
                if (strRet == NULL)
                    strRet = malloc(nNewLen);
                else
                    strRet = realloc(strRet, nNewLen);

                memcpy(strRet + nResLen, line, nLineLen + 1);
                #ifndef PERFORMANCE_MODE     
                if (MALONE_MAIN_COMMAND && DEBUG_AUX) printf("<%d> exeCommand - size: %d | Result: %s\n", m_nRank, nNewLen, strRet);
                #endif                
            } else {
                #ifndef PERFORMANCE_MODE                     
                if (strRet == NULL) {
                    if (MALONE_MAIN_COMMAND) printf("<%d> exeCommand - Executing: %s Result: null line!!\n", m_nRank, strCommand);
                }
                #endif                
            }

        } while (line != NULL);


        status = pclose_noshell(&pclose_arg);
    } else {
    #ifndef PERFORMANCE_MODE          
        if (MALONE_MAIN_COMMAND) printf("<%d> exeCommand - pp null\n", m_nRank);
    #endif        
    }

    #ifndef PERFORMANCE_MODE    
    if (MALONE_MAIN_COMMAND) {
        lEndTick = getTick();
        lTotal = lEndTick - lInitTick;
        printf("<%d> exeCommand - size: %d | TotalTime: %ld |Result: %s \n", m_nRank, nNewLen, lTotal, strRet);
        printf("<%d> exeCommand - end\n", m_nRank);
    }
    #endif        
    return strRet;
}

char** str_split(char* a_str, const char a_delim) {
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    if (a_str != NULL) {
        /* Count how many elements will be extracted. */
        while (*tmp) {
            if (a_delim == *tmp) {
                count++;
                last_comma = tmp;
            }
            tmp++;
        }

        /* Add space for trailing token. */
        count += last_comma < (a_str + strlen(a_str) - 1);

        /* Add space for terminating null string so caller
           knows where the list of returned strings ends. */
        count++;

        result = malloc(sizeof (char*) * count);

        if (result) {
            size_t idx = 0;
            char* token = strtok(a_str, delim);

            while (token) {
                *(result + idx++) = strdup(token);
                token = strtok(0, delim);
            }
            *(result + idx) = 0;
        }
    }
    return result;
}

MutantList* generateCleanMutantList(int nMutants) {
    MutantList* pRet;

    pRet = malloc(sizeof (MutantList));
    pRet->nDead = pRet->nElems = 0;

    for (int i = 0; i < nMutants && i < MAX_MUTANTS; i++) {
        pRet->array[i] = NULL;
    }

    return pRet;
}

void freeMutantList(MutantList* pList) {
    T_stMutant* pMutant;
    if (pList != NULL) {
        for (int i = 0; i < pList->nElems && i < MAX_MUTANTS; i++) {
            pMutant = pList->array[i];

            if (pMutant != NULL)
                free(pMutant);
        }
        free(pList);
    }
}

int isEnabledDebugMainCommandLog() {
    return m_stConfigValues != NULL ? m_stConfigValues->nDebugMainCommand : 0;
}
/* CUARENTENA DE USO (borrar si esto sigue funcionando durante un tiempo)
 * //No se de donde sale esto. 03/05/20 
#define READ   0
#define WRITE  1

FILE * popen2(char* command, char* type, int * pid) {
    pid_t child_pid;
    int fd[2];
    pipe(fd);

    if ((child_pid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    // child process//
    if (child_pid == 0) {
        if (type == "r") {
            close(fd[READ]); //Close the READ end of the pipe since the child's fd is write-only
            dup2(fd[WRITE], 1); //Redirect stdout to pipe
        } else {
            close(fd[WRITE]); //Close the WRITE end of the pipe since the child's fd is read-only
            dup2(fd[READ], 0); //Redirect stdin to pipe
        }
        printf("I am child process my ID is   =  %d\n", getpid());
        setpgid(child_pid, child_pid); //Needed so negative PIDs can kill children of /bin/sh
        execl("/bin/sh", "/bin/sh", "-c", command, NULL);
        exit(0);
    } else {
        if (type == "r") {
            close(fd[WRITE]); //Close the WRITE end of the pipe since parent's fd is read-only
        } else {
            close(fd[READ]); //Close the READ end of the pipe since parent's fd is write-only
        }
    }

    pid = (int) child_pid;

    if (type == "r") {
        return fdopen(fd[READ], "r");
    }

    return fdopen(fd[WRITE], "w");
}
*/
int pclose2(FILE * fp, pid_t pid) {
    int stat;

    fclose(fp);
    while (waitpid(pid, &stat, 0) == -1) {
        if (errno != EINTR) {
            stat = -1;
            break;
        }
    }

    return stat;
}

void createTestRefP(int nIndexTest, char* strResult, double dTime, int nKill, T_stTestInfo** pTest) {
    
    if(*pTest == NULL)
        *pTest = (T_stTestInfo*) malloc(sizeof (T_stTestInfo));
    
    (*pTest)->dTime = dTime;
    (*pTest)->nKill = nKill;
    (*pTest)->nTest = nIndexTest;
    bzero((*pTest)->res, MAX_RESULT_SIZE);

    if (strResult != NULL)
        strcpy((*pTest)->res, strResult);    
}
//TODO: Cambiar nombre.
T_stTestInfo* createTestTrap2(int nIndexTest, char* strResult, double dTime, int nKill) {
    T_stTestInfo* pTest;

    pTest = (T_stTestInfo*) malloc(sizeof (T_stTestInfo));
    pTest->dTime = dTime;
    pTest->nKill = nKill;
    pTest->nTest = nIndexTest;
    bzero(pTest->res, MAX_RESULT_SIZE);

    if (strResult != NULL)
        strcpy(pTest->res, strResult);

    return pTest;
}

double getOriginalTimeout() {
    double dRet;

    if (m_stEnvValues != NULL && m_stEnvValues->nDebugMaxOriginalTimeout >= 0) {
        dRet = m_stEnvValues->nDebugMaxOriginalTimeout;
    } else
        dRet = MAX_ORIGINAL_TIMEOUT;

    return dRet;
}

double getMutantsTimeout(double dOriginalTime) {
    double dRet;

    if (m_stEnvValues != NULL && m_stEnvValues->nDebugMaxMutantsTimeout >= 0) {
        dRet = dOriginalTime * m_stEnvValues->nDebugMaxMutantsTimeout;
        if (dRet < 1 && m_stEnvValues->nDebugMutantsMinimumTimeout >= 0)
            dRet = m_stEnvValues->nDebugMutantsMinimumTimeout;
    } else
        dRet = MINIMUM_WORKER_TIME_MARGIN;

    return dRet;
}

char* getMarkerToken() {
    char* strRet;

    if (m_stEnvValues != NULL && m_stEnvValues->strMarkerToken != NULL) {
        strRet = m_stEnvValues->strMarkerToken;
    } else
        strRet = DEFAULT_MARKER_TOKEN;

    return strRet;
}
/**
 * Determine if exists a marker token
 * @return 
 */
int existsMarkerToken()
{
   return m_stEnvValues != NULL && m_stEnvValues->strMarkerToken != NULL&& strlen(m_stEnvValues->strMarkerToken)>0;
}
/**
 * Initialises the structures necessaries to carry out the different heuristics and optimisations
 */
void createHeuristicStructures() 
{
    int nMutants, nTests;

    //Initialize some information related with execution map and equivalent mutants
    if (m_stEnvValues != NULL) {
        printf("<%d> distribution_full_dynamic_scatter - Updating structures\n", m_nRank);
        nMutants = m_stEnvValues->nTotalMutants;
        nTests = m_stEnvValues->nTotalTests;
        printf("<%d> distribution_full_dynamic_scatter - Mutants: %d Tests :%d \n", m_nRank, nMutants, nTests);
        initializeExecutionMap(nMutants, nTests);
        if (m_stEnvValues->nClusterMutants != 0)
            initializeEquivalentMap(nMutants, nTests);
    }
}
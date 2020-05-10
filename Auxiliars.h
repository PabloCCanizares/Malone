
#ifndef AUXILIARS_H
#define AUXILIARS_H

#ifdef DISABLELOGS
#define DEBUG_AUX 0
#define MALONE_MAIN_COMMAND 0
#else
#define DEBUG_AUX isEnabledAuxLog()
#define MALONE_MAIN_COMMAND isEnabledDebugMainCommandLog()
#endif

int prettyPrint(int nIndex,int nTotal,int nAntProg);
int getOriginalTime(T_stTestList *pList,int nTest);
int checkResultOriginal(int nIndexTest,const char *strResult);
int checkTestResultOriginal(T_stTestInfo *pTest);
int testVector();
int readTestSuite();
int checkResult(int nIndexTest,const char *strResult);
int checkTestResult(T_stTestInfo *pTest);
void insertFullMutantInfo(int nMutant,int nMutantState,T_stTestList *pList);
int insertTestResult(int nMutant,int nTest,T_stTestInfo *pTest);
int insertTestResult_unsorted(int nMutant,int nTest,T_stTestInfo *pTest);
void insertMutant(T_stMutant *pMutant,int nIndexMutant);
void insertMutantTestByTest(T_stMutant* pMutant, int nIndexMutant, int nWorkerSource);
int allocMutant(int nIndexMutant);
int isMutantAllocated(int nIndexMutant);
char *buildExeLine_general(int nIndexMutant,int nIndexTest, int nOriginalMode);
char *buildExeLine_StandAlone(int nIndexMutant,int nIndexTest, int nOriginalMode);
void addResultIndex(T_stTestList *pTestList,int nIndex,const char *t,double dTime,int nKill);
void initializeEquivalentMap(int nMutants,int nTests);
void initializeExecutionMap(int nMutants,int nTests);
void initialize_auxiliars();
int addResult(T_stTestList *pTestList,const char *t,double dTime,int nKill);
T_stTestInfo *createTest(int nIndexTest,char *strResult,double dTime,int nKill);
struct TestInfo* createTestST(int nIndexTest, char* strResult, double dTime, int nKill);
char *buildExeLine(int nIndexMutant,int nIndexTest, int nOriginalMode);
char* buildEquivLine(int nIndexMutant, int nOriginalMode);
char* generateCompilationLine(int nIndexMutant, int nOriginalMode);
void showDebugOptions();
int isEnabledAuxLog();
int isEnabledDebugMainCommandLog();
int generateResFolder();
void saveConfigAndEnvironmentFiles();
MutantList* generateCleanMutantList(int nMutants);
void freeMutantList(MutantList* pList);

void createHeuristicStructures();
void createTestRefP(int nIndexTest, char* strResult, double dTime, int nKill, T_stTestInfo** pTest);
char* getMarkerToken();
int existsMarkerToken();
double getOriginalTimeout();
double getMutantsTimeout(double dOriginalTime);

//Common
long int getTick();
int getenv_int(char *strKey);
char *replace(char const *const original,char const *const pattern,char const *const replacement);
char *replace_char(const char *s,char ch,const char *repl);
int file_exist (char *filename);
char* execCommandLine(const char *fmt, ...); 
char* execCommandLine_async(const char *fmt, ...);
char* execCommandLineHPC(const char *fmt, ...);
FILE *fopen_mkdir( char *path, char *mode );
void redirect_stdout(char *strFile);
void recursive_mkdir(char *path);
void copy(char *source, char *dest);
char** str_split(char* a_str, const char a_delim);
FILE * popen2(char* command, char* type, int * pid);
int pclose2(FILE * fp, pid_t pid);
#endif	// AUXILIARS_H


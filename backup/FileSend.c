
#include "Options.h"
#include <sys/stat.h>

  //MPI_File      m_fh;
  FILE* m_fh;  
  void ErrorMessage_(int error, int rank, char* string)
  {
          fprintf(stderr, "Process %d: Error %d in %s\n", rank, error, string);
          //MPI_Finalize();
          //exit(-1);
  }

  int doSendFile(const char* strFileName)
  {
    T_stIniValues* pValues;
    int start, end, error;
    int length;

    char* buffer;

    //Master processing
    if(m_nRank == MALONE_MASTER)
    {
        printf("<%d> Reading path: %s\n", m_nRank, strFileName);
        writeLog(LOG_VERBOSE, "Opening file!! %s\n", strFileName);
       // loadFromIni(strFileName);
        
        int size;
        struct stat st;
        stat(strFileName, &st);
        size = st.st_size;
        char* buffer = malloc(size * sizeof(char));
        
        m_fh = fopen(strFileName, "r");
        //fscanf(m_fh, "%s", buffer);
        fread(buffer, sizeof(char),size, m_fh);
        //Master                   
        MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(buffer,sizeof(char)*size,MPI_CHAR, MALONE_MASTER,MPI_COMM_WORLD);
               
        fclose(m_fh);        
        writeLog(LOG_VERBOSE, "Master end!! %d", m_nRank);
        pValues = loadFromIniMemory(buffer, sizeof(char)*size);
    }
    //g_key_file_load_from_data - Leemos los datos
    //Slaves
    else
    {
       int nData;
       printf("<%d> Receiving buffer\n", m_nRank);
       MPI_Bcast(&nData, 1, MPI_INT, 0, MPI_COMM_WORLD);
       buffer = (char *)malloc(nData);
       MPI_Bcast(buffer,nData,MPI_CHAR, MALONE_MASTER,MPI_COMM_WORLD);
       printf("Received bytes: %d\n data: %s\n", nData, buffer);
       
       pValues = loadFromIniMemory(buffer, nData);
    }
    printf("<%d> End\n", m_nRank);
  }
  

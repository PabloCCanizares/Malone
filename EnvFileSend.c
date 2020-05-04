
#include "Options.h"
#include "Auxiliars.h"
#include <sys/stat.h>

  //MPI_File      m_fh;
  FILE* m_fh;  
  void ErrorMessage_(int error, int rank, char* string)
  {
        fprintf(stderr, "Process %d: Error %d in %s\n", rank, error, string);
        //MPI_Finalize();
        //exit(-1);
  }

  int doSendEnvFile(const char* strFileName)
  {
    T_stIniValues* pValues;
    int start, end, error, length, nData, size, nRet;
    char* buffer;
    struct stat st;
    
    nRet= 0;
    //Master processing
    if(m_nRank == MALONE_MASTER)
    {
        stat(strFileName, &st);
        size = st.st_size;
        char* buffer = malloc(size * sizeof(char));
        
        //Open the file
        m_fh = fopen(strFileName, "r");
        
        //Read the file into the buffer
        length = fread(buffer, sizeof(char),size, m_fh);
           
        //Send the structure size
        MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        //Send the buffer
        MPI_Bcast(buffer,sizeof(char)*size,MPI_CHAR, MALONE_MASTER,MPI_COMM_WORLD);
            
        //Close the file
        fclose(m_fh);  
        
        nRet=1;
    }
    else
    {
       MPI_Bcast(&nData, 1, MPI_INT, 0, MPI_COMM_WORLD);
       buffer = (char *)malloc(nData);
       
       //Receive the ini file
       MPI_Bcast(buffer,nData,MPI_CHAR, MALONE_MASTER,MPI_COMM_WORLD);
       m_stEnvValues = loadEnvironmentFromIniMemory(buffer, nData);  
       
       if(m_stEnvValues != NULL)
        nRet=1;
    }
          
     return nRet;    
  }
  

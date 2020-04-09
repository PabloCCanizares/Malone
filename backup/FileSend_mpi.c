
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "Options.h"       /* Include the MPI definitions */
#include <sys/stat.h>

  MPI_File      m_fh;
    
  void ErrorMessage(int error, int rank, char* string)
  {
          fprintf(stderr, "Process %d: Error %d in %s\n", rank, error, string);
          //MPI_Finalize();
          //exit(-1);
  }

  int doSendFileMPI(const char* strFileName)
  {
    int start, end, error;
    int length;

    char* buffer;
    MPI_Status    status;
    MPI_Offset    filesize;

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
        
        //Master
        if(openFile(strFileName))
        {
            if(getFileSize(&filesize))
            {
                //Read all the 
                length = filesize;
                start = 0;
            }
            length = filesize;
            start = 0;

            /* calculate the range for each process to read */
            //length = filesize / nprocs;
            //start = length * myrank;

            end = filesize;


            fprintf(stdout, "Proc %d: range = [%d, %d)\n", m_nRank, start, end);

            /* Allocate space */
            buffer = (char *)malloc((end - start) * sizeof(char));
            if (buffer == NULL) 
                ErrorMessage(-1, m_nRank, "malloc");

            /* Each process read in data from the file */
            MPI_File_seek(m_fh, start, MPI_SEEK_SET);
            error = MPI_File_read(m_fh, buffer, length, MPI_BYTE, &status);
            if(error != MPI_SUCCESS) 
                ErrorMessage(error, m_nRank, "MPI_File_read");   
            
            MPI_Bcast(buffer,m_nSize,MPI_CHAR, MALONE_MASTER,MPI_COMM_WORLD);
            
            //close the file
            MPI_File_close(&m_fh);        
        }
        writeLog(LOG_VERBOSE, "Master end!! %d", m_nRank);
     }
    //g_key_file_load_from_data - Leemos los datos
    //Slaves
    else
    {
       printf("<%d> Receiving buffer\n", m_nRank);
       buffer = (char *)malloc(1000 * sizeof(char));
       //MPI_Bcast(buffer,m_nSize,MPI_CHAR, MALONE_MASTER,MPI_COMM_WORLD);
       printf("%s", buffer);
    }

  }
  
  int openFile(char* strFileName)
  {
    int error, nOpen;
    
    writeLog(LOG_VERBOSE, "FileSend - Init ");    
    writeLog(LOG_VERBOSE, "FileSend - Opening file: %s", strFileName);
    
    nOpen=1;    
    error = MPI_File_open(MPI_COMM_WORLD, strFileName, MPI_MODE_UNIQUE_OPEN, MPI_INFO_NULL, &m_fh);
    
    if(error != MPI_SUCCESS) 
    {
        nOpen =0;
        ErrorMessage(error, m_nRank, "MPI_File_open");
    }
   
    writeLog(LOG_VERBOSE, "FileSend - End");
    
    return nOpen;    
  }
  int getFileSize(MPI_Offset* filesize)
  {
    int error, nSize;
    
    /* Get the size of file */
    error = MPI_File_get_size(m_fh, filesize);
    if(error != MPI_SUCCESS) 
    {
        ErrorMessage(error, m_nRank, "MPI_File_get_size");      
    }
    else
        nSize = filesize;
    
    return nSize;
  }
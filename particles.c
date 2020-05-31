/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "Malone_options.h"
#include "particles.h"

struct Partstruct 
{ 
   int    type;  /* particle type */ 
   double d[6];   /* particle coordinates */ 
   char   b[7];   /* some additional information */ 
}; 
int severalParticles()
{
    
    struct Partstruct    particle[1000]; 
    MPI_Status status;
    int          i, dest, tag; 
    MPI_Comm     comm; 
    struct Partstruct    singleParticle;

    /* build datatype describing structure */ 

    MPI_Datatype Particletype; 
    MPI_Datatype type[3] = {MPI_INT, MPI_DOUBLE, MPI_CHAR}; 
    int          block[3] = {1, 6, 7}; 
    MPI_Aint     disp[3]; 
    MPI_Aint baseAddr, addr1, addr2;
    
    
    //TODO: Futuro, añadir flag de mostrar prints en tests, de momento nada
    //printf("<%d> Initialized process %d of %d\n", m_nRank, m_nRank, m_nSize);
    
    MPI_Get_address(&singleParticle.type, &baseAddr); 
    MPI_Get_address(&singleParticle.d, &addr1); 
    MPI_Get_address(&singleParticle.b, &addr2); 
    
    disp[0] = 0;
    disp[1] = addr1 - baseAddr;
    disp[2] = addr2 - baseAddr;

    MPI_Type_create_struct(3, block, disp, type, &Particletype); 

    /* Particletype describes first array entry -- using absolute 
       addresses */ 

                      /* 5.1: 
                send the entire array */ 

    MPI_Type_commit(&Particletype); 

    //Initialize particle
    if(m_nRank == 0)
    {
        for(int j=0;j<1000;j++)
        {
            for(int i=0;i<7;i++)
                particle[j].b[i] = 'a'+i;
            for(int i=0;i<6;i++)
                particle[j].d[i] = i;
        }

    }
    
    if(m_nRank == 0)
        //MPI_Send(singleParticle, 1, Particletype, 1, 123, MPI_COMM_WORLD); 
        MPI_Send(&particle, 1000, Particletype, 1, 123, MPI_COMM_WORLD); 
    else
        MPI_Recv(&particle, 1000, Particletype, 0, 123, MPI_COMM_WORLD, &status); 
        //MPI_Recv(singleParticle, 1, Particletype, 0, 123, MPI_COMM_WORLD, &status); 
    
    
    /*if(m_nRank != 0)
    {
        for(int j=0;j<1000;j++)
        {        
            for(int i=0;i<7;i++)
                printf("%c, ", particle[j].b[i]);

            printf("\n");

            for(int i=0;i<6;i++)
                printf("%f, ", particle[j].d[i]);
        }
        
        printf("\n");
    }*/
    //https://www.mpi-forum.org/docs/mpi-3.1/mpi31-report/node91.htm
    
    
    if(m_nRank == 0)
        printf("Test general Datatype_1 PASS\n");
    return 0;
}
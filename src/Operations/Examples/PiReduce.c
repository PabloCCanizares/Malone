/* 
 * File:   Comm_Operations.c
 * Author: pablo
 *
 * Created on October 28, 2015, 12:42 PM
 */

#include  "../../Malone_options.h"
#include <math.h>

double f(double); 

double f(double a)
{
    return (4.0 / (1.0 + a*a));
}
void comm_operations_test1()
{
    int    n,  i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;
    double startwtime = 0.0, endwtime;
    int    namelen;
    char   processor_name[MPI_MAX_PROCESSOR_NAME];

    printf("<%d> Init comm_op\n", m_nRank);
    MPI_Get_processor_name(processor_name,&namelen);

    fprintf(stdout,"Process %d of %d is on %s\n",
	    m_nRank, m_nSize, processor_name);
    fflush(stdout);

    n = 100000;			/* default # of rectangles */
    
    if (m_nRank == 0)
	startwtime = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    
    h   = 1.0 / (double) n;
    sum = 0.0;
    /* A slightly better approach starts from large i and works back */
    for (i = m_nRank + 1; i <= n; i += m_nSize)
    {
	x = h * ((double)i - 0.5);
	sum += f(x);
    }
    mypi = h * sum;

    printf("<%d> pi is partially %.16f\n", m_nRank, mypi );
    MPI_Barrier(MPI_COMM_WORLD);
    
    //MPI_Reduce(&m_nRank, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    MPI_Reduce(&mypi, &pi, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (m_nRank == 0) 
    {
	endwtime = MPI_Wtime();
	printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
	printf("wall clock time = %f\n", endwtime-startwtime);	       
	fflush(stdout);
    }
    printf("<%d> End comm_op\n", m_nRank);
}
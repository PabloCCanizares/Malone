#include <mpi.h>
#include <stdlib.h>
#define RAND_MAX 10
#define MAX_TESTS 100
#define MAX_RESULT_SIZE 10000
typedef struct{
    char res[MAX_RESULT_SIZE]; 
    double dTime;
    int nKill;
    int nTest;
    int nMutant;
       
}T_stTI;  //Reduced version

MPI_Datatype m_TestType;
T_stTI m_oTest[MAX_TESTS];   

void init_Datatype_test()
{
  //Lets initialize all the datatypes neccesary to perform the MALONE process
    
    int base, i;
    int blocklen[5] = {MAX_RESULT_SIZE, 1, 1, 1, 1}; 
     
    MPI_Datatype type[5] = {MPI_CHAR, MPI_DOUBLE, MPI_INT, MPI_INT, MPI_INT};     
    MPI_Aint     disp[5];     
    MPI_Status status;
        
    MPI_Address( m_oTest, disp); 
    MPI_Address( &m_oTest[0].dTime, disp+1); 
    MPI_Address( &m_oTest[0].nKill, disp+2); 
    MPI_Address( &m_oTest[0].nTest, disp+3);
    MPI_Address( &m_oTest[0].nMutant, disp+4);
     
    base = disp[0];
    for (i = 0; i < 5; i++) 
        disp[i] -= base;
       
    MPI_Type_struct( 5, blocklen, disp, type, &m_TestType);    
    MPI_Type_commit(&m_TestType); 
}

int main(int argc, char** argv) {
const int MAX_NUMBERS = 100;
int numbers[MAX_NUMBERS];
int number_amount;
int world_rank, nSize, nRecvCode;

    MPI_Init( 0, 0 );
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nSize);
	init_Datatype_test();
if (world_rank == 0) {
	printf("MASTER!!!\n");
    // Pick a random amont of integers to send to process one
    srand(time(NULL));
    number_amount = (int)(rand() / (float)RAND_MAX) * MAX_NUMBERS;
	number_amount=10;
	printf("Count: %d\n", number_amount);
	//if(number_amount<0)
	  number_amount=10;

    // Send the amount of integers to process one
    MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 sent %d numbers to 1\n", number_amount);

	printf("Sending datatype\n");
        strcpy(m_oTest[0].res, "OH YEEEEEAH\n");
        m_oTest[0].dTime = 1.1;
        m_oTest[0].nKill = 1;
        m_oTest[0].nTest = 666;
        MPI_Send(m_oTest, 1, m_TestType, 1, 0, MPI_COMM_WORLD);
} else if (world_rank == 1) {
    MPI_Status status;
	printf("0\n");
    // Receive at most MAX_NUMBERS from process zero
    MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
             &status);
	printf("1\n");
    // After receiving the message, check the status to determine
    // how many numbers were actually received
    MPI_Get_count(&status, MPI_INT, &number_amount);
	printf("2\n");
    // Print off the amount of numbers, and also print additional
    // information in the status object
    printf("1 received %d numbers from 0. Message source = %d, "
           "tag = %d\n",
           number_amount, status.MPI_SOURCE, status.MPI_TAG);

    nRecvCode = MPI_Recv(m_oTest, 1, m_TestType, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&status); 
    
    printf("<%d>receiveSingleTestAndCheck - Receiving %d test from %d | code: %d\n", world_rank, 1, status.MPI_SOURCE, nRecvCode);
   printf("printTestRed- <%d>Test: %s : %lf <%d>\n",m_oTest[0].nMutant,m_oTest[0].res, m_oTest[0].dTime, m_oTest[0].nKill);

   
}
MPI_Finalize();
}


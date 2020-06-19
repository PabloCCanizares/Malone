/******************************************************************************/
// MALONE: Manager for pAralleL mutatiON tEsting.
/******************************************************************************/
/** @file test_dist_algorithm.c
 *     Source code of the test suite for testing distribution algorithms.
 * @par Purpose:
 *     Testing the distribution algorithms and different combination of 
 * optimisationts.
 * @par Comment:
 *     None.
 * @author   Pablo C. Cañizares
 *  * @date  10 May 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "test_dist_algorithm.h"
#include "test_envfile.h"

void testDistributionAlgorithms()
{
    //for(int i=0;i<20;i++) 
    
    // [101000]
    //test_dist_algorithm_1();       
    test_dist_algorithm_2();       
    test_dist_algorithm_3();  
    test_dist_algorithm_4();      
    test_dist_algorithm_5();
    
    //NONE optimizations [000000]
    //test_dist_algorithm_6();        
    test_dist_algorithm_7();  
    test_dist_algorithm_8();    
    test_dist_algorithm_9();       //KO tests
    test_dist_algorithm_10();     //KO tests
    
    
    //Distributing original TS [100000]
    //test_dist_algorithm_11();        
    test_dist_algorithm_12();  
    test_dist_algorithm_13();  
    test_dist_algorithm_14();  //KO tests
    test_dist_algorithm_15();      
    
    //SCATTER: only affects to the algorithm 4 and 5 [001000]
    test_dist_algorithm_16();  
    test_dist_algorithm_17();
    
    //SCATTER & Distribute [101000]
    test_dist_algorithm_18();  
    test_dist_algorithm_19();
        
    //Detect equivalents    [000100]
    test_dist_algorithm_20();       //ko tests
    test_dist_algorithm_21();       //ko tests  -> ojito que los resultados de los tests estan viniendo vacios!!!
    
    //Detect equivalents and parallel md5 [000101]
    test_dist_algorithm_22();       // ko tests
    test_dist_algorithm_23();       // ko tests
        
    //Distribute + sort  [110000]
    //test_dist_algorithm_24();
    test_dist_algorithm_25();
    test_dist_algorithm_26();
    test_dist_algorithm_27();
    test_dist_algorithm_28();
    
    //Distribute + sort + scatter [111000]
    test_dist_algorithm_29();
    test_dist_algorithm_30();   
    
    //Distribute + sort + equivalents[110100]
    test_dist_algorithm_31();
    test_dist_algorithm_32();  
    
    //Distribute + sort + equivalents + parallel[110101]
    test_dist_algorithm_33();
    test_dist_algorithm_34();  
    
    //Distribute + sort + scatter + equivalents [111100]
    test_dist_algorithm_35();
    test_dist_algorithm_36(); 
    
    //Distribute + sort + scatter + equivalents + parallel [111101]
    test_dist_algorithm_37();
    test_dist_algorithm_38(); 
    
    //Distribute + scatter + equivalents [101100]
    test_dist_algorithm_39();
    test_dist_algorithm_40();
    
    //Distribute + scatter + equivalents + parallel [101101]
    test_dist_algorithm_41();
    test_dist_algorithm_42();
    
    //Distribute + equivalents [100100]
    test_dist_algorithm_43();
    test_dist_algorithm_44();
    
    //Distribute + equivalents + parallel [100101]
    test_dist_algorithm_45();
    test_dist_algorithm_46();
    
    //Sort [010000]
    //test_dist_algorithm_47();
    test_dist_algorithm_48();
    test_dist_algorithm_49();
    test_dist_algorithm_50();
    test_dist_algorithm_51();
    
    //Sort + Scatter [011000]
    test_dist_algorithm_52();
    test_dist_algorithm_53();
    
    //Sort + Equivalent [010100]
    test_dist_algorithm_54();
    test_dist_algorithm_55();
    
    //Sort + Equivalent  + parallel [010101]
    test_dist_algorithm_56();
    test_dist_algorithm_57();
    
    //Scatter + equivalents [001100]
    test_dist_algorithm_58();
    test_dist_algorithm_59();
    
    //Scatter + equivalents + parallel [001101]
    test_dist_algorithm_60();
    test_dist_algorithm_61();    
        
   

    
    
    m_oAutoTests.nCategories++;
}
void test_dist_algorithm_1()
{  
    float fMS;
    printf("Test [test_dist_algorithm_1] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 1;
    fMS = test_dist_base("autotest/test_autotest_add_stand.ini");
      
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.001))
            m_oAutoTests.nPass++;
        else
        {
            printf("%f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;
    printf("Test [test_dist_algorithm_1] - End\n");
}
void test_dist_algorithm_2()
{  
    float fMS;
    printf("Test [test_dist_algorithm_2] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 2;
    fMS = test_dist_base("autotest/test_autotest_add_stand.ini");   
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.9523, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_2] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;
    printf("Test [test_dist_algorithm_2] - End\n");
}
void test_dist_algorithm_3()
{  
    float fMS, nFMS;
    printf("Test [test_dist_algorithm_3] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 3;
    fMS = test_dist_base("autotest/test_autotest_add_stand.ini");
      
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.9523, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_3] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }   
    }    
    
    m_oAutoTests.nTotalTests++;
    printf("Test [test_dist_algorithm_3] - End\n");
}


void test_dist_algorithm_4()
{  
    float fMS;
    printf("Test [test_dist_algorithm_4] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 4;
    fMS = test_dist_base("autotest/test_autotest_add_stand.ini");    
      
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.950, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_4] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }    
        
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_4] - End\n");
}
void test_dist_algorithm_5()
{  
    float fMS;
    printf("Test [test_dist_algorithm_5] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947368, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_5] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }    
    
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_5] - End\n");
}

void test_dist_algorithm_6()
{  
    float fMS;
    printf("Test [test_dist_algorithm_6] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 1;
    fMS = test_dist_base("autotest/test_autotest_add_stand_000000_m20_t999.ini");
      
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.001))
            m_oAutoTests.nPass++;
        else
        {
            printf("%f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }    

    m_oAutoTests.nTotalTests++;
    printf("Test [test_dist_algorithm_6] - End\n");
}
void test_dist_algorithm_7()
{  
    float fMS;
    printf("Test [test_dist_algorithm_7] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 2;
    fMS = test_dist_base("autotest/test_autotest_add_stand_000000_m20_t999.ini");   
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.9523, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_7] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;
    printf("Test [test_dist_algorithm_7] - End\n");
}
void test_dist_algorithm_8()
{  
    float fMS, nFMS;
    printf("Test [test_dist_algorithm_8] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 3;
    fMS = test_dist_base("autotest/test_autotest_add_stand_000000_m20_t999.ini");
      
    if(m_nRank == 0)
    {    
        //Montar Equality
        if(Equality(fMS, 0.9523, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_8] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
    
    m_oAutoTests.nTotalTests++;
    printf("Test [test_dist_algorithm_8] - End\n");
}


void test_dist_algorithm_9()
{  
    float fMS;
    printf("Test [test_dist_algorithm_9] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 4;
    fMS = test_dist_base("autotest/test_autotest_add_stand_000000_m20_t999.ini");    
      
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.950, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_9] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
        
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_9] - End\n");
}
void test_dist_algorithm_10()
{  
    float fMS;
    printf("Test [test_dist_algorithm_10] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_000000_m20_t999.ini");
      
    if(m_nRank == 0)
    {    
        if(Equality(fMS, 0.947368, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_10] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
    
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_10] - End\n");
}

void test_dist_algorithm_11()
{  
    float fMS;
    printf("Test [test_dist_algorithm_6] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 1;
    fMS = test_dist_base("autotest/test_autotest_add_stand_100000_m20_t999.ini");
      
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.001))
            m_oAutoTests.nPass++;
        else
        {
            printf("%f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }    

    m_oAutoTests.nTotalTests++;
    printf("Test [test_dist_algorithm_6] - End\n");
}
void test_dist_algorithm_12()
{  
    float fMS;
    printf("Test [test_dist_algorithm_12] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 2;
    fMS = test_dist_base("autotest/test_autotest_add_stand_100000_m20_t999.ini");   
    
    if(m_nRank == 0)
    {  
        if(Equality(fMS, 0.9523, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_12] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;
    printf("Test [test_dist_algorithm_12] - End\n");
}
void test_dist_algorithm_13()
{  
    float fMS, nFMS;
    printf("Test [test_dist_algorithm_13] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 3;
    fMS = test_dist_base("autotest/test_autotest_add_stand_100000_m20_t999.ini");
      
    if(m_nRank == 0)
    {    
        if(Equality(fMS, 0.9523, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_13] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
    
    m_oAutoTests.nTotalTests++;
    printf("Test [test_dist_algorithm_13] - End\n");
}
void test_dist_algorithm_14()
{  
    float fMS;
    printf("Test [test_dist_algorithm_14] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 4;
    fMS = test_dist_base("autotest/test_autotest_add_stand_100000_m20_t999.ini");    
      
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.950, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_14] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }    
        
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_14] - End\n");
}
void test_dist_algorithm_15()
{  
    float fMS;
    printf("Test [test_dist_algorithm_15] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_100000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947368, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_15] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }   
    
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_15] - End\n");
}
void test_dist_algorithm_16()
{  
    float fMS;
    printf("Test [test_dist_algorithm_16] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 4;
    fMS = test_dist_base("autotest/test_autotest_add_stand_001000_m20_t999.ini");    
      
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.950, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_16] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
       
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_16] - End\n");
}
void test_dist_algorithm_17()
{  
    float fMS;
    printf("Test [test_dist_algorithm_17] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_001000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947368, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_17] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
    
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_17] - End\n");
}
void test_dist_algorithm_18()
{  
    float fMS;
    printf("Test [test_dist_algorithm_18] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 4;
    fMS = test_dist_base("autotest/test_autotest_add_stand_101000_m20_t999.ini");    
     
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.950, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_18] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
            
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_18] - End\n");
}
void test_dist_algorithm_19()
{  
    float fMS;
    printf("Test [test_dist_algorithm_19] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_101000_m20_t999.ini");
       
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947368, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_19] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
    
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_19] - End\n");
}
void test_dist_algorithm_20()
{  
    float fMS;
    printf("Test [test_dist_algorithm_20] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 4;
    fMS = test_dist_base("autotest/test_autotest_add_stand_000100_m20_t999.ini");    
      
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.950, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_20] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
        
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_20] - End\n");
}
void test_dist_algorithm_21()
{  
    float fMS;
    printf("Test [test_dist_algorithm_21] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_000100_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947368, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_21] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
    
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_21] - End\n");
}
void test_dist_algorithm_22()
{  
    float fMS;
    printf("Test [test_dist_algorithm_22] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm = 4;
    fMS = test_dist_base("autotest/test_autotest_add_stand_000101_m20_t999.ini");    
      
    if(m_nRank == 0)
    {    
        if(Equality(fMS, 0.950, 0.005))
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_22] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }
        
    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_22] - End\n");
}
void test_dist_algorithm_23()
{  
    float fMS;
    printf("Test [test_dist_algorithm_23] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_000101_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.909, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_23] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_23] - End\n");
}
void test_dist_algorithm_24()
{  
    float fMS;
    printf("Test [test_dist_algorithm_24] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =1;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_110000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.909, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_24] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_24] - End\n");
}
void test_dist_algorithm_25()
{  
    float fMS;
    printf("Test [test_dist_algorithm_25] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =2;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_110000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_25] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_25] - End\n");
}
void test_dist_algorithm_26()
{  
    float fMS;
    printf("Test [test_dist_algorithm_26] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =3;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_110000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_26] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_26] - End\n");
}
void test_dist_algorithm_27()
{  
    float fMS;
    printf("Test [test_dist_algorithm_27] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_110000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_27] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_27] - End\n");
}
void test_dist_algorithm_28()
{  
    float fMS;
    printf("Test [test_dist_algorithm_28] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_110000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_28] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_28] - End\n");
}
void test_dist_algorithm_29()
{  
    float fMS;
    printf("Test [test_dist_algorithm_29] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_111000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_29] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_29] - End\n");
}
void test_dist_algorithm_30()
{  
    float fMS;
    printf("Test [test_dist_algorithm_30] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_111000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_30] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_30] - End\n");
}

void test_dist_algorithm_31()
{  
    float fMS;
    printf("Test [test_dist_algorithm_31] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_110100_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_31] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_31] - End\n");
}
void test_dist_algorithm_32()
{  
    float fMS;
    printf("Test [test_dist_algorithm_32] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_110100_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_32] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_32] - End\n");
}
//
void test_dist_algorithm_33()
{  
    float fMS;
    printf("Test [test_dist_algorithm_33] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_110101_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_33] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_33] - End\n");
}
void test_dist_algorithm_34()
{  
    float fMS;
    printf("Test [test_dist_algorithm_34] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_110101_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.909, 0.009))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_34] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_34] - End\n");
}
//
void test_dist_algorithm_35()
{  
    float fMS;
    printf("Test [test_dist_algorithm_35] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_111100_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_35] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_35] - End\n");
}
void test_dist_algorithm_36()
{  
    float fMS;
    printf("Test [test_dist_algorithm_36] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_111100_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_36] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_36] - End\n");
}
void test_dist_algorithm_37()
{  
    float fMS;
    printf("Test [test_dist_algorithm_37] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_111101_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_37] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_37] - End\n");
}
void test_dist_algorithm_38()
{  
    float fMS;
    printf("Test [test_dist_algorithm_38] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_111101_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.909, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_38] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_38] - End\n");
}
void test_dist_algorithm_39()
{  
    float fMS;
    printf("Test [test_dist_algorithm_39] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_101100_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_39] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_37] - End\n");
}
void test_dist_algorithm_40()
{  
    float fMS;
    printf("Test [test_dist_algorithm_40] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_101100_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_40] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_40] - End\n");
}
void test_dist_algorithm_41()
{  
    float fMS;
    printf("Test [test_dist_algorithm_41] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_101101_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_41] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_41] - End\n");
}
void test_dist_algorithm_42()
{  
    float fMS;
    printf("Test [test_dist_algorithm_42] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_101101_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.909, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_42] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_42] - End\n");
}
//
void test_dist_algorithm_43()
{  
    float fMS;
    printf("Test [test_dist_algorithm_43] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_100100_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_43] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_43] - End\n");
}
void test_dist_algorithm_44()
{  
    float fMS;
    printf("Test [test_dist_algorithm_44] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_100100_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_44] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_44] - End\n");
}
void test_dist_algorithm_45()
{  
    float fMS;
    printf("Test [test_dist_algorithm_45] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_100101_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_45] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_45] - End\n");
}
void test_dist_algorithm_46()
{  
    float fMS;
    printf("Test [test_dist_algorithm_46] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_100101_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.909, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_46] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_46] - End\n");
}
//
void test_dist_algorithm_47()
{  
    float fMS;
    printf("Test [test_dist_algorithm_47] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =1;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_010000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_47] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_47] - End\n");
}
void test_dist_algorithm_48()
{  
    float fMS;
    printf("Test [test_dist_algorithm_48] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =2;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_010000_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_48] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_48] - End\n");
}
void test_dist_algorithm_49()
{  
    float fMS;
    printf("Test [test_dist_algorithm_49] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =3;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_010000_m20_t999.ini");
        
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.95, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_49] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_49] - End\n");
}
void test_dist_algorithm_50()
{  
    float fMS;
    printf("Test [test_dist_algorithm_50] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_010000_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_50] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_50] - End\n");
}
void test_dist_algorithm_51()
{  
    float fMS;
    printf("Test [test_dist_algorithm_51] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_010000_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_51] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_51] - End\n");
}
void test_dist_algorithm_52()
{  
    float fMS;
    printf("Test [test_dist_algorithm_52] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_011000_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_52] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_52] - End\n");
}
void test_dist_algorithm_53()
{  
    float fMS;
    printf("Test [test_dist_algorithm_53] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_011000_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_53] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_53] - End\n");
}
//
void test_dist_algorithm_54()
{  
    float fMS;
    printf("Test [test_dist_algorithm_54] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_010100_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_54] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_54] - End\n");
}
void test_dist_algorithm_55()
{  
    float fMS;
    printf("Test [test_dist_algorithm_55] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_010100_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_55] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_55] - End\n");
}
void test_dist_algorithm_56()
{  
    float fMS;
    printf("Test [test_dist_algorithm_56] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_010101_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_56] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_56] - End\n");
}
void test_dist_algorithm_57()
{  
    float fMS;
    printf("Test [test_dist_algorithm_57] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_010101_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.909, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_57] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_57] - End\n");
}
void test_dist_algorithm_58()
{  
    float fMS;
    printf("Test [test_dist_algorithm_58] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_001100_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_58] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_58] - End\n");
}
void test_dist_algorithm_59()
{  
    float fMS;
    printf("Test [test_dist_algorithm_59] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_001100_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_59] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_59] - End\n");
}
//
void test_dist_algorithm_60()
{  
    float fMS;
    printf("Test [test_dist_algorithm_60] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =4;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_001101_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.947, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_60] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_60] - End\n");
}
void test_dist_algorithm_61()
{  
    float fMS;
    printf("Test [test_dist_algorithm_61] - Init\n");
    
    MPI_Barrier(MPI_COMM_WORLD);
    m_nSelAlgorithm =5;
    
    fMS = test_dist_base("autotest/test_autotest_add_stand_001101_m20_t999.ini");
    
    if(m_nRank == 0)
    {
        if(Equality(fMS, 0.909, 0.005))  //Equivalent mutants
            m_oAutoTests.nPass++;
        else
        {
            printf("Test [test_dist_algorithm_61] - MS obtained: %f\n", fMS);
            m_oAutoTests.nFail++;
        }
    }

    m_oAutoTests.nTotalTests++;    
    printf("Test [test_dist_algorithm_61] - End\n");
}
float test_dist_base(char* strPathIn)
{
    char* strPathComp;
    int nTCs;
    float fMS;
    
    printf("Test [test_dist_base] - Init\n");
    fMS = 0.0;
    initialize_auxiliars();
    loadConfig();
    
    strPathComp = loadEnvFile_t(strPathIn);
    m_stEnvValues = (T_stIniValues*) loadEnvironmentFromIniFile(strPathComp);
    
    if(m_stConfigValues == NULL)
        printf("Config null!\n");
    
    if(m_stEnvValues != NULL)
    {
        if(malone_isReady())
        {
            if(m_stEnvValues->nStandalone == 1)
                nTCs = readTestSuite();

            launchExecutionMode();
        }
        else
        {
            printf("Test [test_dist_base] - There exist some error in the provided configuration\n");
        }
    }
    else
    {
        printf("Test [test_dist_base] - The environment file has not been loaded!\n");
    }

    //Calculate the MS
    if(m_oMutantList.nElems >0 && m_oMutantList.nDead>0)
        fMS = (float) ((float)m_oMutantList.nDead  / (float)m_oMutantList.nElems);     
    else
    {
        if(m_oMutantList.nDead == 0 && m_oMutantList.nElems>0)
            fMS = 0.0;
        else
            fMS = 0.0;
    }
            
    free_auxiliars();
    
    printf("Test [test_dist_base] - End\n");
    
    return fMS;
}

int Equality(float a, float b, float epsilon)
{
  printf("%02f < %02f", fabs(a - b), epsilon);
  return fabs(a - b) < epsilon;
}
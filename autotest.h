/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   autotest.h
 * Author: Pablo C. Cañizares
 *
 * Created on April 26, 2020, 11:04 PM
 */

#ifndef AUTOTEST_H
#define AUTOTEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "Options.h"
#include "test_command.h"
#include "test_envfile.h"
#include "test_conffile.h"    
#include "test_deploys.h"    
#include "test_testcases.h"    
#include "test_mutants.h"    
#include "test_dist_algorithm.h"    
#define TEST_DEBUG 1

typedef struct
{
    int nTotalTests;
    int nPass;
    int nFail;
    int nCategories;
}T_stAutoTests;

T_stAutoTests m_oAutoTests;

int main(int argc, char** argv);
static void mySignalCatcher(int n);
void autotest();
void autotest_print_results();
void autotest_initialise();
#ifdef __cplusplus
}
#endif

#endif /* AUTOTEST_H */


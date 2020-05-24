/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_envfile.h
 * Author: j0hn
 *
 * Created on April 28, 2020, 9:28 PM
 */

#ifndef TEST_ENVFILE_H
#define TEST_ENVFILE_H

#include "Auxiliars.h"
#include "autotest.h"

#ifdef __cplusplus
extern "C" {
#endif

void testEnvFile();
void test_envfile_1();
void test_envfile_2();        
void test_envfile_3();   
void test_envfile_4();
void test_envfile_5();
char* loadEnvFile_t(char* envValue);


#ifdef __cplusplus
}
#endif

#endif /* TEST_ENVFILE_H */


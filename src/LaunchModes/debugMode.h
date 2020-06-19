/* 
 * File:   debugMode.h
 * Author: Pablo C. Cañizares
 *
 * Created on January 13, 2016, 12:56 PM
 */
#ifndef LAUNCHDEBUGMODE_H
#define LAUNCHDEBUGMODE_H

#include <stdio.h>
#include <stdlib.h>
#include "../Malone.h"
#include "debugMode.h"
#include "../Operations/Aux/Malone_printers.h"
#include "../Operations/Examples/tests.h"
#include "../Operations/Aux/Auxiliars.h"
#include "../Operations/EnvFileSend.h"
#include "../Operations/Examples/MPIDataTypes.h"
#include "../Operations/Examples/MonteCarlo.h"
#include "../Operations/Aux/randomElements.h"
#include "../Operations/Examples/particles.h"

#ifdef __cplusplus
extern "C" {
#endif
    
void launchDebugMode();
void workersDebugOperations();
void masterDebugOperations();
void doStaticMT();
void doPirata();

#ifdef __cplusplus
}
#endif

#endif /* LAUNCHDEBUGMODE_H */
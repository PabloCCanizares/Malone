/* 
 * File:   debugMode.h
 * Author: Pablo C. Cañizares
 *
 * Created on January 13, 2016, 12:56 PM
 */
#ifndef LAUNCHDEBUGMODE_H
#define LAUNCHDEBUGMODE_H

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
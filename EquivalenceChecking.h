#ifndef EQUIVALENCECHECKING_H
#define	EQUIVALENCECHECKING_H

#include "Options.h"
#include "Auxiliars.h"
#include "MPI_Operations.h"
#include "md5Parallel.h"
#include "printers.h"

#define EQUIVALENCE_PRINT 0
#define EQUIVALENCE_PRINT_DEBUG 0

void createClusters();
void checkMutantsEquivalence();
int areEquals(int nMutantSource,int nMutantDest);

#endif	/* EQUIVALENCECHECKING_H */
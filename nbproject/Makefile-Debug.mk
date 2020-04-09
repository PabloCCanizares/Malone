#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=mpicc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Auxiliars.o \
	${OBJECTDIR}/DistributionAlgorithms.o \
	${OBJECTDIR}/EnvFileSend.o \
	${OBJECTDIR}/EquivalenceChecking.o \
	${OBJECTDIR}/ExecutionMapSaver.o \
	${OBJECTDIR}/MPIDataTypes.o \
	${OBJECTDIR}/MPI_Operations.o \
	${OBJECTDIR}/Malone.o \
	${OBJECTDIR}/MonteCarlo.o \
	${OBJECTDIR}/PiReduce.o \
	${OBJECTDIR}/configuration.o \
	${OBJECTDIR}/debugMode.o \
	${OBJECTDIR}/distribution_adaptive_grain.o \
	${OBJECTDIR}/distribution_dynamic_mutants.o \
	${OBJECTDIR}/distribution_full_dynamic.o \
	${OBJECTDIR}/distribution_full_dynamic_scatter.o \
	${OBJECTDIR}/distribution_static_mutants.o \
	${OBJECTDIR}/executionMode.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/md5Parallel.o \
	${OBJECTDIR}/printers.o \
	${OBJECTDIR}/randomElements.o \
	${OBJECTDIR}/testVec.o \
	${OBJECTDIR}/tests.o \
	${OBJECTDIR}/transformations.o


# C Compiler Flags
CFLAGS=-g

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-g -O0

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs glib-2.0` `pkg-config --libs mpich` -lm  -lpthread   

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/malone

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/malone: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	mpicc -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/malone ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Auxiliars.o: Auxiliars.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Auxiliars.o Auxiliars.c

${OBJECTDIR}/DistributionAlgorithms.o: DistributionAlgorithms.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DistributionAlgorithms.o DistributionAlgorithms.c

${OBJECTDIR}/EnvFileSend.o: EnvFileSend.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EnvFileSend.o EnvFileSend.c

${OBJECTDIR}/EquivalenceChecking.o: EquivalenceChecking.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EquivalenceChecking.o EquivalenceChecking.c

${OBJECTDIR}/ExecutionMapSaver.o: ExecutionMapSaver.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ExecutionMapSaver.o ExecutionMapSaver.c

${OBJECTDIR}/MPIDataTypes.o: MPIDataTypes.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MPIDataTypes.o MPIDataTypes.c

${OBJECTDIR}/MPI_Operations.o: MPI_Operations.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MPI_Operations.o MPI_Operations.c

${OBJECTDIR}/Malone.o: Malone.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Malone.o Malone.c

${OBJECTDIR}/MonteCarlo.o: MonteCarlo.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MonteCarlo.o MonteCarlo.c

${OBJECTDIR}/PiReduce.o: PiReduce.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PiReduce.o PiReduce.c

${OBJECTDIR}/configuration.o: configuration.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/configuration.o configuration.c

${OBJECTDIR}/debugMode.o: debugMode.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/debugMode.o debugMode.c

${OBJECTDIR}/distribution_adaptive_grain.o: distribution_adaptive_grain.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/distribution_adaptive_grain.o distribution_adaptive_grain.c

${OBJECTDIR}/distribution_dynamic_mutants.o: distribution_dynamic_mutants.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/distribution_dynamic_mutants.o distribution_dynamic_mutants.c

${OBJECTDIR}/distribution_full_dynamic.o: distribution_full_dynamic.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/distribution_full_dynamic.o distribution_full_dynamic.c

${OBJECTDIR}/distribution_full_dynamic_scatter.o: distribution_full_dynamic_scatter.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/distribution_full_dynamic_scatter.o distribution_full_dynamic_scatter.c

${OBJECTDIR}/distribution_static_mutants.o: distribution_static_mutants.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/distribution_static_mutants.o distribution_static_mutants.c

${OBJECTDIR}/executionMode.o: executionMode.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/executionMode.o executionMode.c

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/md5Parallel.o: md5Parallel.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/md5Parallel.o md5Parallel.c

${OBJECTDIR}/printers.o: printers.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/printers.o printers.c

${OBJECTDIR}/randomElements.o: randomElements.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/randomElements.o randomElements.c

${OBJECTDIR}/testVec.o: testVec.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/testVec.o testVec.c

${OBJECTDIR}/tests.o: tests.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tests.o tests.c

${OBJECTDIR}/transformations.o: transformations.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../libraries/mpich-3.1.4/src `pkg-config --cflags glib-2.0` `pkg-config --cflags mpich`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/transformations.o transformations.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

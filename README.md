# MALONE: Manager for pAralleL mutatiON tEsting

Malone is a framework for automatically parallelising the mutation testing framework. It is important to remark that MALONE is independent from the mutation engine used to generate both mutants and test suites, such as MuJava and Milu, which can be easily adapted to this framework. The unique requirement that the  MuT tool must fulfil is command line support. In this way, MALONE invokes the MuT engine via command line for carrying out different operations, such as creating the mutants set and executing the test suite over the mutants. The execution of the test cases can also be conducted in standalone mode, that is, the test cases -- once created -- are executed directly by MALONE without using an additional MuT tool.

###### Main architecture

###### Parallel distribution algorithms supported
With respect to the distribution algorithms supported, the tool includes both static and dynamic strategies, an adapted version of *DMBO*, *DTC*, *PEDRO*, *EMINENT*, and the *Dynamic adaptive algorithm*.

###### Other supported techniques
Regarding the improvement techniques, MALONE includes *distributing the test suite among the available processes*, *sorting the test suite before executing test cases*, *scatter improvement*, *clustering equivalent mutants*, *parallel compilation* and *parallel checksum*.

###### MALONE in High-Performance Computing environments
MALONE has been designed to be used in large HPC environments. For this purpose, the tool uses MPI to interchange data between processes. The logical process schema is based on a master/worker pattern. In this way, the master process orchestrates the MuT process and distributes the workload among the workers. Finally, the tool also supports sequential MuT execution -- not based on a master/worker pattern, but on a single process -- which can be used to calculate the speed-up obtained in the parallel execution.

set -x 
DIRLZO2=/localSpace/research_edinburgh/Compression_MPI-master/PRACTICAL.5.0/lzo-2.05/bin
DIRPRACTICAL=/localSpace/research_edinburgh/Compression_MPI-master/PRACTICAL.5.0/

mpicc `pkg-config --cflags glib-2.0` -I/usr/include/glib-2.0 -c *.c  -lglib-2.0 -lm -lc -O3
mpicc -g -s  `pkg-config --cflags glib-2.0` -I/usr/include/glib-2.0 *.o -lglib-2.0 -lm -lc -O3 $DIRPRACTICAL/systimer.o $DIRPRACTICAL/snappy.o $DIRPRACTICAL/lz4.o $DIRPRACTICAL/externo.o $DIRPRACTICAL/libbcl.a $DIRLZO2/lib/liblzo2.a -o malone


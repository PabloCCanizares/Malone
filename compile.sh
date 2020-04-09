mpicc `pkg-config --cflags glib-2.0` -I/usr/include/glib-2.0 -o malone *.c -lglib-2.0 -lm -lc -O3

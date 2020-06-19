mpicc `pkg-config --cflags glib-2.0` -I/usr/include/glib-2.0 -o malone *.c src/*.c src/DistributionAlgorithms/*.c src/LaunchModes/*.c src/Operations/*.c src/Operations/Aux/*.c src/Operations/Equivalence/*.c src/Operations/Examples/*.c src/Tests/*.c -lglib-2.0 -lm -lc -g -O3



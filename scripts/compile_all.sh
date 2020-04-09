 #!/bin/bash
echo "Milu home: "$MILU_HOME
echo $MILU_HOME/examples/$2/milu_output/mutants 
cd $MILU_HOME/examples/$2/milu_output/mutants
ls
# $1 - Comando de ejecución
# $2 - Nombre de la aplicación
# $3 - Numero de mutantes
echo "Compiling mutants: "
for i in `seq 0 $3`;
do	echo " ======================> "$i
        echo "gcc -lm -Wall" $i"/src/*.c -o"  $i"/bin/mut.exe -lcrypto -lssl"
	gcc -lm -Wall -O3 $i"/src/bmp_ok.c" "-o" $i"/bin/mut.exe" -lcrypto -lssl
done

echo "Compilation Ends"

#gcc -lm -Wall "0/src/bmp_ok.c" "-o" "0/bin/mut.exe"

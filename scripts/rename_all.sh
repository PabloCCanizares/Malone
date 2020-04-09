 #!/bin/bash
echo "Milu home: "$MILU_HOME
echo $MILU_HOME/examples/$1/milu_output/mutants 
cd $MILU_HOME/examples/$1/milu_output/mutants
ls
# $1 - Nombre de la aplicación
# $2 - Numero de mutantes
echo "Compiling mutants: "
for i in `seq 0 $2`;
do	echo " ======================> "$i
        echo "ex -sc" '%s/localSpace/home\/pablo\/imgDatabase/g|x' "bmp_ok.c"
	cd $i"/src/" 
	ex -sc '%s/alarm(1)/alarm(6)/g|x' bmp_ok.c
	ex -sc '%s/alarm(2)/alarm(12)/g|x' bmp_ok.c
	ex -sc '%s/alarm(3)/alarm(18)/g|x' bmp_ok.c
	ex -sc '%s/alarm(11)/alarm(30)/g|x' bmp_ok.c
	ex -sc '%s/alarm(12)/alarm(30)/g|x' bmp_ok.c
	ex -sc '%s/alarm(15)/alarm(35)/g|x' bmp_ok.c
	cd ../..
done

echo "Renaming complete!!"



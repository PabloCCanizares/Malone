#!/bin/bash
echo "Deploying PBS!"

if [[ $# -eq 0 ]] ; then
    echo 'It is neccesary to set the folder to deploy!'
    exit 1
fi

search_dir=$PWD/$1


if [ ! -d "$search_dir" ]; then
  echo "the directory $search_dir does not exists"
  exit 1
fi

echo "Deploying all the PBS located at directory $search_dir"
sleep 1

echo "removing the ~ elements"
rm $search_dir/*.pbs*~
sleep 1

echo "Getting all the files in array!"

i=0
array={}
for entry in "$search_dir"/*
do
	echo "storing in array: "$entry  
	array[$i]=$entry
	((i++))	
done

echo "Deploying the array files"



pbsLine=0
for pbsLine in ${array[@]}
do
	echo "Start deploying file: "$pbsLine
	qstatStatus=$(qstat -u pcerro | wc -l)
	echo "Number of processes $qstatStatus"
	echo "Number of processes size ${#qstatStatus}"

	while [ $qstatStatus -ge 1 ]
	do		
		sleep 1
		echo "."
		qstatStatus=$(qstat -u pcerro | wc -l)
	done
	
	echo "deploying "$pbsLine
	echo "deleting output"
	rm /lustre/home/dc010/pcerro/repository/bmp/salida__*	
	echo "undoing bzip"
	bunzip2 /lustre/home/dc010/pcerro/repository/bmp/*.bz2
	qsub $pbsLine
	echo "deployed "$pbsLine
done

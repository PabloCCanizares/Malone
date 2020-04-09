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
rm $search_dir/*.pbs~*
sleep 1

echo "sending pbs!"
for entry in "$search_dir"/*
do
	echo "deploying "$entry  
	qsub $entry
  
done


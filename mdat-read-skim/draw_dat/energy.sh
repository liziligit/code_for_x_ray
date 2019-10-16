iStart_num=$1
iAccout=$2

if [ "$1" = "" ]
then
  echo iStart_num number is empty 
  exit	
fi

if [ "$2" = "" ]
then
  echo iAccout number is empty 
  exit	
fi

echo root -L -l -q -b "find_track_Tgraph_energy.c++(${iStart_num},${iAccout})"
root -L -l -q -b "find_track_Tgraph_energy.c++(${iStart_num},${iAccout})"
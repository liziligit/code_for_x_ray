#运行方式 ./energy.sh 0 1


# iStart_num=$1
# iAccout=$2

# if [ "$1" = "" ]
# then
#   echo iStart_num number is empty 
#   exit	
# fi

# if [ "$2" = "" ]
# then
#   echo iAccout number is empty 
#   exit	
# fi

# echo root -L -l -q -b "find_track_Tgraph_energy.c++(${iStart_num},${iAccout})"
# root -L -l -q -b "find_track_Tgraph_energy.c++(${iStart_num},${iAccout})"

# echo root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir.c++(${iStart_num},${iAccout})"
# root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir.c++(${iStart_num},${iAccout})"


# root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir.c++(0,100)"


for i in `seq 1 1`
do
	echo $i
	root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir.c++((${i}-1)*10,10)"
	sleep 1
done
for i in `seq 1 1`
do
	echo $i
	root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir_energy.c++((${i}-1)*100,100)"
	# sleep 1
done

# for i in `seq 1 90`
# do
# 	echo $i
# 	root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir_energy.c++((${i}-1)*100,100)"
# 	# sleep 1
# done

# for i in `seq 1 57`
# do
# 	echo $i
# 	root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir_energy.c++((${i}-1)*100,100)"
# 	# sleep 1
# done
########Ne10DME-80kPa-DV350GV630IV300-55Fe
for i in `seq 1 100`
do
	echo $i
	root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir_energy.c+((${i}-1)*100,100)"
	# sleep 1
done

###########Ne20DME-80kPa-DV350GV760IV300-55Fe
# for i in `seq 1 90`
# do
# 	echo $i
# 	root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir_energy.c+((${i}-1)*100,100)"
# 	# sleep 1
# done

###########Ne20DME-80kPa-DV350GV770IV300-X-ray-generator
# for i in `seq 1 57`
# do
# 	echo $i
# 	root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir_energy.c+((${i}-1)*100,100)"
# 	# sleep 1
# done

###########Ne10DME-80kPa-DV350GV630IV300-X-ray-generator
# for i in `seq 1 4`
# do
# 	echo $i
# 	root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir_energy.c+((${i}-1)*100,100)"
# 	# sleep 1
# done
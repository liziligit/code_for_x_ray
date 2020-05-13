#运行方式 ./mdat_cut_pede_binaryzation_cluster_dir_pixel_num.sh

# for i in `seq 1 1`
# do
# 	echo $i
# 	root -L -l "mdat_cut_pede_binaryzation_cluster_dir_pixel_num.c+((${i}-1)*1,3)"
# 	sleep 1
# done


for i in `seq 1 1`
do
	echo $i
	root -L -l  -q -b "mdat_cut_pede_binaryzation_cluster_dir_pixel_num.c+((${i}-1)*5665,5665)"
	# sleep 1
done

# for i in `seq 2 2`
# do
# 	echo $i
# 	root -L -l -q -b "mdat_cut_pede_binaryzation_cluster_dir.c+((${i}-1)*5600,65)"
# 	sleep 1
# done
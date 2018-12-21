#!/bin/bash


read -p "Item number (4 digits integer): " i_num
read -p "Simple name (String with no white space): " i_sim_name
read -p "Item name (String): " i_name
read -p "Current quantity (Integer): " i_quan
read -p "Maximum quantity (Integer): " i_max
read -p "Description (String) :" i_des

#loop through the data to check if item exist
directory='./data'
data=(./data/*)
exist=$true
files=""
log=./queries.log

if [ ! -d "$directory" ];then
	echo "ERROR: directory doesn not exist"
fi

for file in  "${data[@]}"; do
	files+="${file}"

done

if [[ $files != *$i_num* ]];
then 
	echo "ERROR: Item not found"
	exist=$false
	exit 1
fi

#update the file
if [[ $exist = $true ]];
then

	r_sim_name=none
	r_name=none
	r_quan=none
	r_max=none
	r_des=none
	export r_sim_name r_name r_quan r_max r_des
	i_file_name=""
	i_file_name=$i_num".item"
	log_item=""
	new_item=""
	blank=""
	
	cd ${directory}
	#if item number is blank :exit
	if [ "$i_num" == "$blank" ];
	then
		echo "Cannot update without an item number, Please start again
"
	exit 1
	fi
	
	#read the file into each element 
	source ../subread.bash <$i_file_name
	#comparison to decide what to keep
	if [ "$i_sim_name" == "$blank" ];then
		i_sim_name=$r_sim_name
	fi
	if [ "$i_name" == "$blank" ];then
		i_name=$r_name
	fi
	if [ "$i_quan" == "$blank" ];then
		i_quan=$r_quan
	fi
	if [ "$i_max" == "$blank" ];then
		i_max=$r_max
	fi
	if [ "$i_des" == "$blank" ];then	
		i_des=$r_des
 	fi
	
	new_item="$i_sim_name $i_name
$i_quan $i_max
$i_des" 
	
	log_item="UPDATED: $i_sim_name - $(date +%m/%d/%Y)"
	echo "$new_item" > $i_file_name
	echo "$log_item" >> $log
	cd ..
fi

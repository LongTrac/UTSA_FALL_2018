#!/bin/bash

read -p "Item number (4 digits integer): " i_num

#loop through an array of data string to check for sub string if item exist
directory='./data'
data=(./data/*)
exist=$true
files=""
log=./queries.log

if [ ! -d "$directory" ];then
	echo "ERROR: Directory does not exist"
fi

for file in  "${data[@]}"; do
	files+="${file}"
done
#if item was not found print an error
if [[ $files != *$i_num* ]];
then 
	echo "ERROR: Item not found"
	exist=$false
	exit 1
fi

#read the file
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
	blank=""
	#if item number is blank :exit
	cd ${directory}
	if [ "$i_num" == "$blank" ];
	then
		echo "Cannot read without an item number, Please start again
"
	exit 1
	fi
	#read each each element into an array 
	source ../subread.bash <$i_file_name
	echo "Item name: $r_name"
	echo "Simple name: $r_sim_name"
  	echo "Item number: $i_num"
	echo "Qty: $r_quan / $r_max"
	echo "Description: $r_des"
	cd ..
fi

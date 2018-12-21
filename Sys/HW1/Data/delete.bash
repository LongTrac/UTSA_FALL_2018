#!/bin/bash

read -p "Enter an item number: " i_num

#loop through an array of data string to check for sub string if item exist
directory='./data'
data=(./data/*)
exist=$true
files=""
log=./queries.log

if [ ! -d "$directory" ]; then
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

#delete the file
if [[ $exist = $true ]];
then
	r_sim_name=none
	export r_sim_name
	i_file_name=""
	i_file_name=$i_num".item"
	blank=""
	
	cd ${directory}
	#if item number is blank :exit
	if [ "$i_num" == "$blank" ];
	then
		echo "Cannot delete without an item number, Please start again
"
	exit 1
	fi
	#read each each element into an array 
	source ../subread.bash <$i_file_name
	
	rm $i_file_name
	log_item="DELETED: $r_sim_name - $(date +%m/%d/%Y)"
	echo "$log_item" >> $log
	echo "$r_sim_name was successfully deleted
"
	cd ..
fi

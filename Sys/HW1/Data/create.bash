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
exist=$false
files=""
log=./queries.log

if [ ! -d "$directory" ];then
	mkdir data
fi

for file in  "${data[@]}"; do
	files+="${file}"

done

if [[ $files = *$i_num* ]];
then 
	echo "ERROR: Item already exists"
	exist=$true
	exit 1
fi

#create the file
if [[ $exist = $false ]]
then
	i_file_name=""
	i_file_name=$i_num".item"
	log_item=""
	log_item="CREATED: $i_sim_name - $(date +%m/%d/%Y)"
	new_item=""
	new_item="$i_sim_name $i_name
$i_quan $i_max
$i_des"
	cd ${directory} 
	echo "$new_item" > $i_file_name
	echo "$log_item" >> $log
	cd ..
fi

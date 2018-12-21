#!/bin/bash
# all the args variables
i_dir=$1"/*"
template=$2
date=$3
o_dir=$4

#if output directory did not exist then create one
if [ ! -d "$o_dir" ];then
	mkdir "$o_dir"
fi

#for each file that match a specific pattern,
#the info is read into the corresponding variable
for file in `find $i_dir -name "[0-9][0-9][0-9][0-9].item"`; do
    i=0

    #read each attributes to its corresponding variable
    if [ 0 ]; then
        read s_name i_name 
        read qty max 
        read body 
    fi < "$file"
    
    #substring to extract the 4 number digits
    out_name=${file:7:4}

    #calculate the percent 
    let percent=$qty*100/$max

    #check if it is less then 10% and it only has 4 args
    if [[ "$percent" -lt 10 ]] && [[ $# -eq 4 ]] ; then

        cat $2 > tmp_template.txt

        sed -i  "s:<<simple_name>>:$s_name:g" tmp_template.txt 
        sed -i  "s:<<item_name>>:$i_name:g" tmp_template.txt 
        sed -i  "s:<<current_quantity>>:$qty:g" tmp_template.txt 
        sed -i  "s:<<max_quantity>>:$max:g" tmp_template.txt 
        sed -i  "s:<<body>>:$body:g" tmp_template.txt 
        sed -i  "s:<<date>>:$date:g" tmp_template.txt

        cat tmp_template.txt > "$out_name.out"
        mv "$out_name.out" $o_dir
    fi    
    #fifth and sixth arg
    if [[ "$percent" -lt 10 ]] && [[ $# -eq 6 ]] ; then

        cat $2 > tmp_template.txt

        sed -i -e  "s:$5simple_name$6:$s_name:g" tmp_template.txt 
        sed -i -e  "s:$5item_name$6:$i_name:g" tmp_template.txt 
        sed -i -e  "s:$5current_quantity$6:$qty:g" tmp_template.txt 
        sed -i -e  "s:$5max_quantity$6:$max:g" tmp_template.txt 
        sed -i -e  "s:$5body$6:$body:g" tmp_template.txt 
        sed -i -e  "s:$5date$6:$date:g" tmp_template.txt

        cat tmp_template.txt > "$out_name.out"
        mv "$out_name.out" $o_dir
    fi
done

rm tmp_template.txt
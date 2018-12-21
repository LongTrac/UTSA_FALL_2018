#!/bin/bash
hour=$(date --date='Aug 16 2018 23:05' +%H)
minute=$(date --date='Aug 16 2018 23:05' +%M)
awk -f program.awk -v h="$hour" -v m="$minute" - 

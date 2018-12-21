#!/bin/bash

if [ -z  "$1" ] ; then
	echo "ERROR: No parameter passing"
else
	sed -E -f ./HW2.sed $1
fi

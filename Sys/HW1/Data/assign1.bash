#!/bin/bash
go=0
while [ $go ]; do
	echo Enter one of the following actions or press CTRL-D to exit.
	echo C - create a new item
	echo R - read an existing item
	echo U - update an existing item
	echo D - delete an existing items

	if ! read char ; then
		break
	fi
	
	case "$char" in
		[Cc])	./create.bash ;;
		[Rr])	./read.bash ;;
		[Uu])	./update.bash ;;
		[Dd])	./delete.bash ;;
		*)	echo "ERROR: invalid option";;

	esac
done
		

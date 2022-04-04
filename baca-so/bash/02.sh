#!/bin/bash

read -r _ # reading "total"

while read -r LINE; do
	FILE=$(echo "${LINE}" | awk '{ for (i=9; i<=NF; i++) { printf( "%s", $i ) };}') #Get filename
	PERMS=0
	# Permission modifiers
	R=4
	W=2
	X=1

	# User
	if [[ ${LINE:1:1} != '-' ]]; then
		((PERMS += $((R * 100))))
	fi

	if [[ ${LINE:2:1} != '-' ]]; then
		((PERMS += $((W * 100))))
	fi

	if [[ ${LINE:3:1} != '-' ]]; then
		((PERMS += $((X * 100))))
	fi
	# Group
	if [[ ${LINE:4:1} != '-' ]]; then
		((PERMS += $((R * 10))))
	fi

	if [[ ${LINE:5:1} != '-' ]]; then
		((PERMS += $((W * 10))))
	fi

	if [[ ${LINE:6:1} != '-' ]]; then
		((PERMS += $((X * 10))))
	fi
	# Others
	if [[ ${LINE:7:1} != '-' ]]; then
		((PERMS += $((R * 1))))
	fi

	if [[ ${LINE:8:1} != '-' ]]; then
		((PERMS += $((W * 1))))
	fi

	if [[ ${LINE:9:1} != '-' ]]; then
		((PERMS += $((X * 1))))
	fi
	# Check if directory
	if [[ PERMS -eq 0 ]]; then
		PERMS='000'
	fi
	if [[ ${LINE:0:1} = 'd' ]]; then
		echo "$FILE/ $PERMS"
	else
		if [[ ${LINE:3:1} = 'x' ]]; then
			echo "$FILE* $PERMS"
		else
			echo "$FILE $PERMS"
		fi
	fi
done

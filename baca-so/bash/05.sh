#!/bin/bash

declare -A MAP # Count of each identifier
ORDERED=()     # Order of indentifiers

while read -r LINE; do
	MAP[${LINE}]=$((${MAP[${LINE}]} + 1))
	ORDERED+=("$LINE")
done
SIZE=${#ORDERED[@]}
if [[ $SIZE -ge 1 ]]; then
	SHYEST=100000
	SHYEST_KEY=''
	for key in "${ORDERED[@]}"; do
		if [[ ${MAP[${key}]} -lt $SHYEST ]]; then
			SHYEST=${MAP[${key}]}
			SHYEST_KEY=${key}
		fi
	done
	echo "$SHYEST_KEY $SHYEST"
else
	echo ""
fi

#!/bin/bash

COUNT=0
SUM=0
ARR=()

while read -r LINE; do
	if [[ $LINE == [1-9]*([0-9]) ]]; then # Match only numbers
		((SUM += LINE))
		ARR[$COUNT]=$LINE
		((COUNT++))
	fi
done

[[ COUNT -ne 0 ]] && ((AVE = SUM / COUNT)) || AVE=0

VAR=0
for ((i = 0; i < COUNT; i++)); do
	((VAR += (((ARR[i] - AVE) ** 2))))
done
((VAR = VAR / COUNT))
printf "%s\n%s\n" "$AVE" "$VAR"

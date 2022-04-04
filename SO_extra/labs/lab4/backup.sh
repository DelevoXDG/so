#!/bin/bash
STR=""
for i in *; do
	STR=""$i"_$(date +%d-%m-%Y_%H:%m)"
	# echo "$STR
	cp -v "$i" "$1"/"$STR"
done

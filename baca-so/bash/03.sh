#!/bin/bash

read -r N

VAR1=0
VAR2=1
VARN=0

for ((i = 0; i < N; i++)); do
	echo "$VAR1"
	((VARN = VAR1 + VAR2))
	VAR1=$VAR2
	VAR2=$VARN
done

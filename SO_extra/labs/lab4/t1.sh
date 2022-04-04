#!/bin/bash

# Sort all files passed as parameters and writes them to lsat parameter

# if [ "$#" -ne 1 ]; then
# 	printf "One parameter expected.\n"
# 	exit 1
# fi

echo ${!#} # ostatni wyraz skryptu
eval W='$'{$#}
echo "$W"

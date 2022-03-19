#!/bin/sh

print_line() {
	DASH_STR=$1
	N=$2
	printf '%s' '----'
	printf "%0.s$DASH_STR" $(seq 1 "$N")
	echo
}

if [ "$#" -ne 1 ]; then
	echo "One parameter expected."
	exit 1
else
	if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
		printf "Usage: %s N\n" "$(basename "$0")"
		printf "\tN > 0 && N <= 0\n"
		printf "Exit status: "
		printf "\t 0 if OK,\n\n"
		printf "\t 1 if invalid number of parameters,\n\n"
		printf "\t 2 if invalid argument passed\n\n."
		exit 0
	else
		if [ "$1" -lt 0 ] || [ "$1" -gt 20 ]; then
			echo "Invalid argument passed."
			exit 2
		fi
	fi
fi

N=$1
DASH_STR=""
# MAX_WIDTH=$((${#$((N*N))}+1))\
MAX_WIDTH=$((N * N))
MAX_WIDTH=${#MAX_WIDTH}
MAX_WIDTH=$((MAX_WIDTH + 1))

for _ in $(seq 1 $((MAX_WIDTH * 1))); do
	DASH_STR="$DASH_STR-"
done
#create a multiplication table 5 columns 10 rows

printf "*  |"
printf "%${MAX_WIDTH}s" $(seq 1 "$N")
printf "\n"
print_line "$DASH_STR" "$N"

for y in $(seq 1 "$N"); do
	printf "%-2s |" "$y"
	for x in $(seq 1 "$N"); do
		printf "%${MAX_WIDTH}s" "$((x * y))"
	done
	echo
done

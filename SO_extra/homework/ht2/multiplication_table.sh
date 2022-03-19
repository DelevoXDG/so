#!/bin/sh
# Maksim Zdobnikau

# Napisac skrypt, ktory dla liczby N (nie wiekszej niz 20) podanej jako argument wypisze na standardowe wyjscie "tabliczke mnozenia" liczb od 1 do N.
# * |  1  2  3  4
# ---------------
# 1 |  1  2  3  4
# 2 |  2  4  6  8
# 3 |  3  6  9 12
# 4 |  4  8 12 16
# Uwaga: tabelka powinna miec "ladnie" sformatowane wiersze i kolumny - np. w taki sposob, jak powyzej.

print_line() { # Prints line of right length

	DASH_STR=$1
	N=$2
	printf '%s' '----'
	printf "%0.s$DASH_STR" $(seq 1 "$N")
	printf "\n"
}

if [ "$#" -ne 1 ]; then
	printf "One parameter expected.\n"
	exit 1
else
	if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
		printf "\nUsage: %s N\n\n" "$(basename "$0")"
		printf "\tN > 0 && N <= 0\n\n"
		printf "Exit status: \n\n"
		printf "\t0 if OK,\n\n"
		printf "\t1 if invalid number of parameters,\n\n"
		printf "\t2 if invalid argument passed.\n"
		exit 0
	else
		if [ "$1" -le 0 ] || [ "$1" -gt 20 ]; then
			printf "Invalid argument passed.\n"
			exit 2
		fi
	fi
fi

N=$1
DASH_STR=""

MAX_WIDTH=$((N * N))
MAX_WIDTH=${#MAX_WIDTH}
MAX_WIDTH=$((MAX_WIDTH + 1)) # Calculates width to format output

for _ in $(seq 1 $((MAX_WIDTH * 1))); do
	DASH_STR="$DASH_STR-"
done

printf "*  |"
printf "%${MAX_WIDTH}s" $(seq 1 "$N")
printf "\n"
print_line "$DASH_STR" "$N"

for y in $(seq 1 "$N"); do
	printf "%-2s |" "$y"
	for x in $(seq 1 "$N"); do
		printf "%${MAX_WIDTH}s" "$((x * y))"
	done
	printf "\n"
done

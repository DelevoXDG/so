#!/bin/bash
read -r INPUT

LETTER1=${INPUT:0:1}

if [[ ${INPUT:1:1} = '=' ]]; then
	while read -r LINE; do
		LETTER_COUNT=0
		((LETTER_COUNT += "$(echo "$LINE" | tr -cd "$LETTER1" | wc -c)"))
		printf "%d\n" "$LETTER_COUNT"
	done
else
	LETTER2=$(echo "$LETTER1" | tr '[:lower:][:upper:]' '[:upper:][:lower:]')
	while read -r LINE; do
		LETTER_COUNT=0
		((LETTER_COUNT += "$(echo "$LINE" | tr -cd "$LETTER1$LETTER2" | wc -c)"))
		printf "%d\n" "$LETTER_COUNT"
	done
fi

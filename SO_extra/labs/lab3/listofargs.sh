#!/bin/sh

echo $#
for I in "$@"; do #inaczej interpretowane niz $@, '$@'
	echo $I
done

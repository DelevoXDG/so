#!/bin/sh

echo $#
for I in "$@" #inaczej interpretowane niz $@, '$@'
do
	echo $I
done

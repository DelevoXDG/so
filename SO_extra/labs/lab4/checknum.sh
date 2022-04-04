#!/bin/sh

case "$1" in
*[!0-9]*) echo "To nie jest liczba" ;;
*) echo "To jest liczba" ;;
esac

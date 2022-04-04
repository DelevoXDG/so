#!/bin/bash

cat $1 | tr ' \t' '\n' | grep -v '^$' | sort | uniq -c | sort -n | tail -n 1 | awk '{print $2}' >$2

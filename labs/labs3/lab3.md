# lab3  
Keywords: 
[[SO Practice]]

--- 
`history | awk '{$1=""; print substr($0,2)}'
### [[Command Log L3]]

### 1. Variables in shelll scripts
``` bash
X=1 #no spaces
x='cokolw$iek'
x="cokolw$iek" # ' ' != " "
```

### 1.1 Global Vars Vs. Local Vars
```bash
export PS1='[\u] [\w] $ '
export PS1='[$USER] [$PWD] \$ '
```

### 1.2 Arithmetic
```bash
A=$((X+1))
```
### 1.3 Y/n input
```bash
read Z
```
### 1.4 Bash scripts  
`#!/bin/sh`  - **default**
`#!/bin/sh`**args**` `
### 1.5 Special variables 
![[Scan - 2022-03-16 14_39_16.pdf]]
`"(73)(ala)"`
`$@` - list of all OPerators
polecenie.sh OR1 OR2
```$0 = polecenie.sh
$1 = OR1
$2 = OR2
```

**$#** - number of OPs
**$?** - last command result 

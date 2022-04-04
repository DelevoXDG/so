# Lab 4

### 1. Bash / shell - baca
sh +
bash +

### 2. 
'@$x123' - ok
"@$x123" - $x used as a variable

### 3.
1. $IFS - { space tab newline }
2. Can be modified
3. #IFS="."
4. ala...ma..kota -?	
5. `unset IFS  && echo "$IFS" | cat -et`
or
```bash 
OLD=$IFS
IFS="~smth"
...
IFS=$OLD 
```
6. IFS="$*"
### 4. Tablice mie potrzebne


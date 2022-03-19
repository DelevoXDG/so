### Hello SO
[[command_log]]
1. who
2. man 
3. Tab Tab
4. Catalogues
5. 1. man hier 
6. ls -a
7. makedir [file1] [file2]
8. makedir -p a/b/c
9. touch 
- change "Date modified"
10. chmod (u/g/o/a)(+/-/=)(r/w/x)


### Find 
 ``` shell
 2041  find -name "*.txt"
 2042  find -name "*.txt", -iname "f*"
 2043  man find
 2044  find -name "*.txt", -iname "f*"
 2045  find -iname "f*"
 2046  find -name "*.txt", -iname "f*"
 2047  find -name "*.txt"
 2048  find -name "*.txt",-iname "f*"
 2049  find -name "*.txt" -iname "f*"
 2050  find -type
 2051  find -type d
 2052  find -type f
 2053  find -type f -size 100k
 2054  find -type f -size +1k
 2055  find -type f -size +0k
 2056  find -size +0k
 2057  find -stime +1
 2058  find . -stime +1
 2059  find . -atime +1
 2060  find . -min -10
 2061  find . -mmin -10
 2062  find . -perm **2
 2063  find . -perm -u=x
 2064  find -name "*s*" -type d
 2065  find /lib -name "*s*" -type d
 2066  find /lib -name *s*' -type d
 2067  find /lib -name '*s*' -type d
 2068  find /lib/ -name *s*' -type d
 2069  find /lib/ -name '*s*' -type d
 2070  find /lib/ -name '*s*' -type d -maxdepth 1
find /bin/ -name "*h" -executable -exec cp "{}" ~/SO_extra/lab2/mybin/ \;
ls mybin/
rm -r mybin/
history | awk '{$1=""; print substr($0,2)}'
```
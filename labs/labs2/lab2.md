### LAB 2

### 1. Hard and soft links
- Hard links use the same node
- Soft links link to file, don't work without file linked to
![[Pasted image 20220309135803.png]]
### 2. Processes
1.  `ps / pstree`
![[Pasted image 20220309142100.png]]
2. kill -sygnał `<id procesu>` 
- wysłanie sygnału do procesu, np. TERM (15), KILL (9)
4. `top` - interaktywna lista procesów
5. `<polecenie>` - (ewentualnie naciskając CTRL+Z). - running in processes in the background 
6. `jobs` - managing background processes
lista aktualnie uruchomionych zadań w tle
`bg / fg` numer zadaniaprzywrócenie wstrzymanego zadania w tle/na pierwszym planie

### 3. In/out

1. `>` - changing standart output
2. `<` - changing standart input
3. `<<` - appending from standart output
4. `>>` - reading from standart input until matching text
![[Pasted image 20220309142453.png]]
5. `>` /dev/null - E.g. hiding errors 
6. `Ctrl + D (cat)` - koniec danych wejsciowych

### 4. Stream, filters
![[Pasted image 20220309143806.png]]
1. **Przykład 1:** 
`tail -10 /etc/fstab | wc -c`
`cat /etc/fstab | tail -n 10 | wc -c`
2. `ls / | sort`
3. **Przykład 2:**
`/usr/bin/users | sort | uniq`

### 5. Filters (Replacing characters)
![[Pasted image 20220309144921.png]]
1. `echo 'linuxize' | tr 'lin' 'red'`
2. **Przyklad 1:**
`ls /bin | tr [:lower:] [:upper:]`
3. **Przyklad 2:**
`ls -l | tr -s ' ' | cut -d ' ' -f3,9 | grep ' '`
![[Pasted image 20220309150546.png]]
`ls -l | awk '{print $3,$9}]'`



### Tasks
1. `ls -S /sbin | sort | tail -n 5`
2. `cat test.txt | tr ' ' '\n' | sort -n | t~k '\n' ' ' > done.txt`
3. 
   
   
   
### Questions:
1. k1 zadania
2. ctrl+z
3. testy
4. /usr/bin/users | sort | uniq
5. `grep` **5.3** jak dziala
6. 
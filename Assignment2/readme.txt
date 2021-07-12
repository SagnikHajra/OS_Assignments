First tried in c and then wrote the whole code in python
This is a Python2.4 code.

Uses of file sorting(insertion) and fork() and observation of processes while sorting

1. It reads all_month.csv file line by line and store that into an struct earthquake array
2. Then create 4 processes using fork()
3. 1st 3 children sort the struct earthquake array using function sort(start, end, pid). It uses insertion sort.
4. Parent process merge the partially sorted array by sorting again, using insertion sort.
5. Then parent process saves the sorted records in a file sorted****.csv
6. Using exec, the perent process then gets replaced by another file call(for this we need to compile the exec.c and rename the executable to "exec")

Part2 is for threading instead of fork, rest are the same.

Codes are compiled using default python interpreter at omega.uta.edu


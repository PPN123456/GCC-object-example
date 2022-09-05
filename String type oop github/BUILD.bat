gcc -Wall -Wfatal-errors   -O2 -c m1Types.c
gcc -Wall -Wfatal-errors   -O2 -c Test.c
gcc -Wall -Wfatal-errors   -O2 m1Types.o Test.o -o Test.exe

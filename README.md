# MPI Transputer
 
## Build:
```
mpic++ transputer.cpp -o transputer

```
## Run (always 16 procs, as grid):
```
mpirun -np 16 --oversubscribe transputer 32 // argument - size of random array to send
```

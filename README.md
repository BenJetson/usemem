# usemem

A small C program that fills and holds a specified amount of memory.

## How it Works

The program will allocate blocks of 128 megabytes to reach the amount of memory
the user wants to reserve. An excess block will be allocated if necessary to 
contain the extra megabytes of data that do not fit in the block size.

These blocks of memory will be filled with random data read from 
`/dev/urandom`. The random data should prevent the operating system from
applying a meaningful amount of compression on this space.

To try and prevent the allocated space from being swapped, the program uses
`mlock` to keep the data resident in memory.

## Why would I use this?

The intended purpose for this utility is testing cgroup memory limits. For 
example, to make sure that PBS or SLURM is enforcing your requested memory 
allocation on HPC clusters.

## Compiling

```sh
gcc -Wall -o usemem usemem.c
```

## Usage

Provide a number of megabytes of memory as an argument to the binary.

Press <kbd>Enter</kbd> to release the memory and exit.

## Example

```sh
$ ./usemem 1234
will reserve 1234 megabytes of memory
using block size of 128 megabytes
allocating 9 blocks of 128 megabytes
allocating block 9 of 9
allocating excess of 82 megabytes
holding memory
press the ENTER key to release memory and exit

releasing memory
all memory released
```

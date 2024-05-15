# Patient Priority Queue Implementation

## Overview
This repository contains the implementation of a Patient Priority Queue in C++. The priority queue organizes patients based on their priority, which is determined by a priority function. The priority queue supports various heap types and structures, including leftist heap and skew heap.

## Files
- `pqueue.h` and `pqueue.cpp`: These files contain the implementation of the `PQueue` class, including constructors, destructor, methods for inserting, removing patients, and managing the priority queue structure.
- `mytest.cpp`: This file provides test cases to demonstrate the functionality of the `PQueue` class.
- `driver.cpp`: Another file with test cases.
- `Makefile`: Contains instructions for compiling the project.

## How to Use
1. Include `pqueue.h` in your C++ project.
2. Create an instance of the `PQueue` class, specifying the priority function, heap type, and structure.
3. Insert patients into the priority queue using the `insertPatient` method.
4. Perform various operations such as removing patients, merging queues, getting the next patient, etc.
5. Compile the project using the provided Makefile instructions.

## Compilation
To compile the project, you can use the provided Makefile. Use the following commands:
- `make p`: Compiles `mytest.cpp` with `pqueue.o` to create an executable named `proj3`.
- `make d`: Compiles `driver.cpp` with `pqueue.o` to create an executable named `proj3`.
- `make b`: Runs `gdb` for debugging purposes.
- `make v`: Runs `valgrind` to check for memory leaks.
- `make r`: Runs the executable `proj3`.

## Cleaning Up
To clean up object files and executables, you can use:
- `make clean`: Removes object files and temporary files.

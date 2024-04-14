# Project 5 CMP SCI 4760 by Abigail Sweeney sweeney.5
# Operating System Simulator

## Overview
This project is an operating system simulator designed to demonstrate resource management strategies and deadlock detection/recovery algorithms.

## Project Structure
- `oss.c`: Main program for the operating system simulator.
- `user_proc.c`: User process program that interacts with the operating system.
- `header.h`: Header file containing constants and function prototypes.

## Dependencies
- GNU Compiler Collection (GCC)
- POSIX-compatible operating system

## Compilation Instructions
To compile the project, run the following command:
make

## Usage Instructions
To run the operating system simulator, use the following command:
./oss [-h] [-n proc] [-s simul] [-i intervalInMsToLaunchChildren] [-f logfile]


## Testing
- Sample input files and expected outputs can be found in the `tests/` directory.
- To run the tests, use the following command:
make test



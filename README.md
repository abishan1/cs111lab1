## UID: 123456789

## Pipe Up

This project replicates the terminal's pipe operator without actually using the pipe operator by leveraging file descriptors.

## Building

To build, run "make pipe" in the terminal.

## Running

To run, there are multiple options. The first option is to run "python -m unittest" on the Arch Virtual Machine (provided to us at the beginning of the class). The second option is to run the program with arguments that you specify. For example: "ls | cat | wc" can be run as "./pipe ls cat wc". Verify that both the example commands you specified with the pipe operator matches the output of providing the same commands to "./pipe".

## Cleaning up

To clean up, run "make clean".


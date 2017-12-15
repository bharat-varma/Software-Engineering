System Environment:
Windows 10 OS
Download Cygwin from https://www.cygwin.com/

Refer the following link for installation help:
https://www.youtube.com/watch?v=hh-V6el8Oxk

For execution,
Download the project files Final Project Execution folder. It contains the entire code with the input .pl.nl files 
that are required for execution.
->Copy all these files in cygwin home user directory
For example: C:\cygwin64\home\akshay

->To compile the program use : make main
->To run with an input file : ./main add.pl

How it works
------------------

------>The first part of the program is the parsing module, which is very basic and simply recognizes in the input all numbers,
variables (start with an uppercase letter), constants (start with lowercase letters), and operators. 
Some special keywords are also recognized.

------>Once the program has been parsed it starts analyzing every predicate in the input. 
The goals are stored in a stack, and the variables are unified to assign values to it. 
Once the unification is done, all variables are replaced by the values it was assigned during the unification
and the process keeps running until the stack is empty. 
When the stack is empty it means the predicate is true, and an answer has been found.


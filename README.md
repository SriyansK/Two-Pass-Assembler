## Two Pass Assembler with Compilation Error Detection

The aim of this project is to write a two pass assembler for an extended SIMPLE
instruction set. It creates an object file for assembly code.

## Getting Started

Just download the asm.cpp file and some test cases.
Note : Execution Procedure described below.

## Execution Procedure

g++ asm.cpp -o asm
./asm filename

## Instruction Set
![alt text](https://github.com/SriyansK/Two-Pass-Assembler/blob/master/Images/img1.jpg)

## Result for test cases

.log file : gives the list of compilation errors in code. <br />
.lst file : listing file , it shows the code with its machine code. <br />
.o : object file.<br/>

test1.asm : There are several errors in assembly code so it gives the list of error in .log file.<br />
test2.asm : It asssembles the code and create three files, .log , .o and .lst <br />.

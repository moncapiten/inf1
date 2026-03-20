# IMaC-B

**I**nteractive **M**arginalization **a**nd **C**onditioning - **B**ayesian 

This project is the coronation of the "Informatica con Laboratorio" course of the Laurea Triennale in Fisica in Università di Pisa.

The `make` command can be run in both windows( tested 10 and 11) and linux( tested Ubuntu 20.04 LTS) and it will create two binaries in the bin/ directory.

One of the binaries is conditionalizerTest.[extension], a test for the conditionalizer function using three different version( trivial, slightly optimized and Variable Elimination) and gives conditional probabilities as well as the time each conditionalization took to execute.

The other binary is main.[extension], an interactive program with whom to work with .bif files.

Extensions are set to be .exe on windows systems and .out on else.
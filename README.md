
# Kilo Text Editor

## This code for learn C

## files
main.c			: 		main program

edef.h			:		header global data
globals.c		: 		global data

estruct.h		: 		global define

terminal.h		:		header terminal
terminal.c		:		interactive with terminal

find.h			:		header find
find.c			:		find word

row.h			:		header row
row.c			:		manage row content

operations.h	:		header operations
operations.c	:		manage content

buffer.h		:		header buffer
buffer.c		:		interactive buffer content

file.h			:		header file
file.c			:		manage file i/o

input.h			:		header input
input.c			:		manage input

output.h		:		header output
output.c		:		manage output

syntax.h		:		header syntax
syntax.c		:		syntax highlight


## Library
- unistd.h
Acess to POSIX system
For read() and STDIN_FILENO 

- termios.h
Control terminal I/O

- stdlib.h
Define data/function

- ctype.h
Check, mapping character

- stdio.h
I/O standard

## Mode
+ View 
+ Insert

## Screen
+---------------------------------------------------------------+
|																|
|																|
|																|
|																|
|																|
|						Main Screen								|
|																|
|																|
|																|
|																|
|																|
+---------------------------------------------------------------+
|						Status bar								|
+---------------------------------------------------------------+
|						Message bar								|
+---------------------------------------------------------------+

# Pixel 
A pixel screen is a ASCII
	[ ][ ][ ]
	[ ][ ][ ]
	[ ][ ][ ]


### Data Structure

## Function 

> write()
Ex : write(STDOUT_FILENO, "~", 1);			/* write character ~ */

"\x1b[2J"		:		clear screen 
"\x1b[K"		:		clear from to end line 

"\x1b[H"		:		set cursor to begin 
"\x1b[?25l"		:		hide cursor 
"\x1b[?25h"		:		show cursor 
	
"\r\n"			:		write break line 

"\x1b[7m"		:		inverted colors
"\x1b[m"		:		normal colors

# SOURCES =  main.c file.c row.c globals.c output.c terminal.c 
#				buffer.c find.c input.c operations.c syntax.c

all:
	gcc -o e												\
	main.c file.c row.c globals.c output.c terminal.c		\
	buffer.c find.c input.c operations.c syntax.c command.c	\



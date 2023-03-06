
#include "command.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "edef.h"
#include "estruct.h"
#include "output.h"
#include "file.h"
#include "terminal.h"

void cmInit()
{
	E.command.size = 0;
	E.command.c = malloc(E.command.size);
}

void cmAppend(const char c)
{
	E.command.size++;
	E.command.c = realloc(E.command.c, E.command.size);

	if (E.command.c == NULL)
		return;

	E.command.c[E.command.size -1] = c;
}

void cmDel()
{
	memmove(&E.command.c[E.command.size - 1], &E.command.c[E.command.size], 1);
	E.command.size--;
}

void cmFree()
{
	E.command.c = NULL;
	free(E.command.c);
	E.command.size = 0;
}

void excuteCommand()
{
	if(strcmp(E.command.c,"w")==0){
		editorSave();
		return;
	}
	if(strcmp(E.command.c,"wq")==0){
		editorSave();
		quit();
		return;
	}
	if(strcmp(E.command.c,"q")==0){
		if (E.dirty)
		{
			editorSetStatusMessage("you not save!!!");
			return;
		}
		quit();
		return;
	}
	if(strcmp(E.command.c,"q!")==0){
		quit();
		return;
	}
}



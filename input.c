#include "input.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include "output.h"
#include "row.h"
#include "estruct.h"
#include "edef.h"
#include "terminal.h"
#include "operations.h"
#include "file.h"
#include "find.h"
#include "command.h"

char *editorPrompt(char *prompt, void (*callback)(char *, int))
{
	size_t bufsize = 128;
	char *buf = malloc(bufsize);

	size_t buflen = 0;
	buf[0] = '\0';

	while (1)
	{
		editorSetStatusMessage(prompt, buf);
		editorRefreshScreen();

		int c = editorReadKey();
		if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE)
		{
			if (buflen != 0)
				buf[--buflen] = '\0';
		}
		else if (c == '\x1b')
		{
			editorSetStatusMessage("");
			if (callback)
				callback(buf, c);
			free(buf);
			return NULL;
		}
		else if (c == '\r')
		{
			if (buflen != 0)
			{
				editorSetStatusMessage("");
				if (callback)
					callback(buf, c);
				return buf;
			}
		}
		else
		{
			/*
			 * iscntrl check character is control character
			 */
			if (!iscntrl(c) && c < 128)
			{
				if (buflen == bufsize - 1)
				{
					bufsize *= 2;
					buf = realloc(buf, bufsize);
				}
				buf[buflen++] = c;
				buf[buflen] = '\0';
			}
		}

		if (callback)
			callback(buf, c);
	}
}

void editorMoveCursor(int key)
{
	erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

	switch (key)
	{
	case ARROW_LEFT:
		if (E.cx != 0)
		{
			E.cx--;
		}
		else if (E.cy > 0) /* begin row to before line */
		{
			E.cy--;
			E.cx = E.row[E.cy].size;
		}
		break;
	case ARROW_RIGHT:
		if (row && E.cx < row->size)
		{
			E.cx++;
		}
		else if (row && E.cx == row->size) /* end row to next line */
		{
			E.cy++;
			E.cx = 0;
		}
		break;
	case ARROW_UP:
		/* if cursor index == 0 not move up */
		if (E.cy != 0)
		{
			E.cy--;
		}
		break;
	case ARROW_DOWN:
		/* enable out screen but not enable out file */
		if (E.cy < E.numrows)
		{
			E.cy++;
		}
		break;
	}

	row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
	int rowlen = row ? row->size : 0;
	if (E.cx > rowlen)
	{
		E.cx = rowlen;
	}
}

void processKeyViewMode(c)
{
	switch (c)
	{
	case ARROW_LEFT:
	case ARROW_RIGHT:
	case ARROW_UP:
	case ARROW_DOWN:
		editorMoveCursor(c);
		break;
	case INSERT_KEY:
		E.mode = EDIT_MODE;
		break;
	case COMMAND_KEY:
		editorSetStatusMessage(":");
		E.mode = COMMAND_MODE;
		break;
	}
}

void processKeyEditMode(c)
{
	if (32 <= c && c <= 126)
	{
		editorInsertChar(c);
	}
	else
	{
		switch (c)
		{
		case ENTER_KEY:
			editorInsertNewline();
			break;
		case BACKSPACE:
			editorDelChar();
			break;
		case ESC_KEY:
			E.mode = VIEW_MODE;
			break;
		}
	}
}

void processKeyCommandMode(c)
{
	if (32 <= c && c <= 126)
	{
		cmAppend(c);
		showCommand();
	}
	switch (c)
	{
		case ENTER_KEY:
			excuteCommand();
			cmFree();
			E.mode = VIEW_MODE;
			break;
		case BACKSPACE:
			if(E.command.size > 0){
				cmDel();
				showCommand();
			}
			break;
		case ESC_KEY:
			cmFree();
			E.mode = VIEW_MODE;
			break;
	}
}

/* process key input */
void editorProcessKeypress()
{
	static int quit_times = KILO_QUIT_TIMES;
 
	int c = editorReadKey(); /* get key input */

	switch (E.mode)
	{
	case EDIT_MODE:
		processKeyEditMode(c);
		break;
	case VIEW_MODE:
		processKeyViewMode(c);
		break;
	case COMMAND_MODE:
		processKeyCommandMode(c);
		break;
	}

	/*
	switch (c)
	{
	case '\r':
		editorInsertNewline();
		break;

	case CTRL_KEY('q'):
		if (E.dirty && quit_times > 0)
		{
			editorSetStatusMessage("WARNING!!! File has unsaved changes. "
								   "Press Ctrl-Q %d more times to quit.",
								   quit_times);
			quit_times--;
			return;
		}
		write(STDOUT_FILENO, "\x1b[2J", 4);
		write(STDOUT_FILENO, "\x1b[H", 3);
		exit(0);
		break;

	case CTRL_KEY('s'):
		editorSave();
		break;

	case HOME_KEY:
		E.cx = 0;
		break;

	case END_KEY:
		if (E.cy < E.numrows)
			E.cx = E.row[E.cy].size;
		break;

	case CTRL_KEY('f'):
		editorFind();
		break;

	case BACKSPACE:
	case CTRL_KEY('h'):
	case DEL_KEY:
		if (c == DEL_KEY)
			editorMoveCursor(ARROW_RIGHT);
		editorDelChar();
		break;

	case PAGE_UP:
	case PAGE_DOWN:
	{
		if (c == PAGE_UP)
		{
			E.cy = E.rowoff;
		}
		else if (c == PAGE_DOWN)
		{
			E.cy = E.rowoff + E.screenrows - 1;
			if (E.cy > E.numrows)
				E.cy = E.numrows;
		}

		int times = E.screenrows;
		while (times--)
			editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
	}
	break;

	case ARROW_UP:
	case ARROW_DOWN:
	case ARROW_LEFT:
	case ARROW_RIGHT:
		editorMoveCursor(c);
		break;

	case CTRL_KEY('l'):
	case '\x1b':
		break;

	default:
		editorInsertChar(c);
		break;
	}*/

	quit_times = KILO_QUIT_TIMES;
}


#include "output.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "estruct.h"
#include "edef.h"
#include "file.h"
#include "row.h"
#include "buffer.h"
#include "syntax.h"

/*
 * scroll file when cursor move
 * set data for editorDrawRows draw (move)
 */
void editorScroll()
{
	E.rx = 0;
	if (E.cy < E.numrows)
	{
		E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
	}

	/* if cursor not out screen */
	if (E.cy < E.rowoff)
	{
		/* set row off = index y cursor */
		E.rowoff = E.cy;
	}

	/* if cursor move out screen */
	if (E.cy >= E.rowoff + E.screenrows)
	{
		/* set row off change */
		E.rowoff = E.cy - E.screenrows + 1;
	}

	if (E.rx < E.coloff)
	{
		E.coloff = E.rx;
	}
	if (E.rx >= E.coloff + E.screencols)
	{
		E.coloff = E.rx - E.screencols + 1;
	}
}

/*
 * Draw main screen
 * No need draw all data 
 * Only draw data have display in screen
 */
void editorDrawRows(struct abuf *ab)
{
	int y;
	for (y = 0; y < E.screenrows; y++)
	{
		int filerow = y + E.rowoff;
		if (filerow >= E.numrows)
		{
			if (E.numrows == 0 && y == E.screenrows / 3)
			{
				char welcome[80];
				int welcomelen = snprintf(welcome, sizeof(welcome),
										  "Kilo editor -- version %s", KILO_VERSION);
				if (welcomelen > E.screencols)
					welcomelen = E.screencols;
				int padding = (E.screencols - welcomelen) / 2;
				if (padding)
				{
					abAppend(ab, "~", 1);
					padding--;
				}
				while (padding--)
					abAppend(ab, " ", 1);
				abAppend(ab, welcome, welcomelen);
			}
			else
			{
				abAppend(ab, "~", 1);
			}
		}
		else
		{
			int len = E.row[filerow].rsize - E.coloff;
			if (len < 0)
				len = 0;
			if (len > E.screencols)
				len = E.screencols;
			char *c = &E.row[filerow].render[E.coloff];
			unsigned char *hl = &E.row[filerow].hl[E.coloff];
			int current_color = -1;
			int j;
			for (j = 0; j < len; j++)
			{
				if (iscntrl(c[j]))
				{
					char sym = (c[j] <= 26) ? '@' + c[j] : '?';
					abAppend(ab, "\x1b[7m", 4);
					abAppend(ab, &sym, 1);
					abAppend(ab, "\x1b[m", 3);
					if (current_color != -1)
					{
						char buf[16];
						int clen = snprintf(
								buf, 
								sizeof(buf), 
								"\x1b[%dm", 
								current_color
						);
						abAppend(ab, buf, clen);
					}
				}
				else if (hl[j] == HL_NORMAL)
				{
					if (current_color != -1)
					{
						abAppend(ab, "\x1b[39m", 5);
						current_color = -1;
					}
					abAppend(ab, &c[j], 1);
				}
				else
				{
					int color = editorSyntaxToColor(hl[j]);
					if (color != current_color)
					{
						current_color = color;
						char buf[16];
						int clen = snprintf(buf, sizeof(buf), "\x1b[%dm", color);
						abAppend(ab, buf, clen);
					}
					abAppend(ab, &c[j], 1);
				}
			}
			abAppend(ab, "\x1b[39m", 5);
		}

		abAppend(ab, "\x1b[K", 3);
		abAppend(ab, "\r\n", 2);
	}
}

/*
 * draw status bar
 */
void editorDrawStatusBar(struct abuf *ab)
{
	abAppend(ab, "\x1b[7m", 4);		/* inverted colors */

	char status[80], rstatus[80];
	int len = snprintf(
		status,
		sizeof(status),
		"%s - %.20s - %d lines %s",
		E.mode == VIEW_MODE ? "-- VIEW --" : E.mode == EDIT_MODE ? "-- EDIT --" : "-- COMMAND --",
		E.filename ? E.filename : "[No Name]",
		E.numrows,
		E.dirty ? "(modified)" : ""
	);

	int rlen = snprintf(
		rstatus,
		sizeof(rstatus),
		"%s | %d/%d",
		E.syntax ? E.syntax->filetype : "no ft",
		E.cy + 1, E.numrows);

	if (len > E.screencols)
		len = E.screencols;

	abAppend(ab, status, len);
	while (len < E.screencols)
	{
		if (E.screencols - len == rlen)
		{
			abAppend(ab, rstatus, rlen);
			break;
		}
		else
		{
			abAppend(ab, " ", 1);
			len++;
		}
	}
	
	abAppend(ab, "\x1b[m", 3);	/* return nomal color */
	abAppend(ab, "\r\n", 2);
}

void editorDrawMessageBar(struct abuf *ab)
{
	abAppend(ab, "\x1b[K", 3);
	int msglen = strlen(E.statusmsg);
	if (msglen > E.screencols)
		msglen = E.screencols;
	if (msglen && time(NULL) - E.statusmsg_time < 5)
		abAppend(ab, E.statusmsg, msglen);
}

/*
 * Main Draw Screen
 * Apped Data to buffer
 * write by data buffer
 */
void editorRefreshScreen()
{
	editorScroll();

	struct abuf ab = ABUF_INIT;

	abAppend(&ab, "\x1b[?25l", 6); /* append buffer hide cursor */
	abAppend(&ab, "\x1b[H", 3);	   /* set cursor to begin */

	editorDrawRows(&ab);	   /* append buffer main */
	editorDrawStatusBar(&ab);  /* append buffer status bar */
	editorDrawMessageBar(&ab); /* append buffer message bar */

	/*
	 *	Move cursor to cursor position
	 */
	char buf[32];
	snprintf(
		buf,
		sizeof(buf),
		"\x1b[%d;%dH",
		(E.cy - E.rowoff) + 1,
		(E.rx - E.coloff) + 1);
	abAppend(&ab, buf, strlen(buf));

	abAppend(&ab, "\x1b[?25h", 6); /* append buffer show cursor */

	write(STDOUT_FILENO, ab.b, ab.len); /* write buffer to screen */
	abFree(&ab);
}

void editorSetStatusMessage(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
	va_end(ap);
	E.statusmsg_time = time(NULL);
}

void showCommand()
{
	editorSetStatusMessage("%d:%s",E.command.size, E.command.c);
}
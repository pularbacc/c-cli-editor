/*** interactive with terminal ***/

#include "terminal.h"

#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "estruct.h"
#include "edef.h"
#include "output.h"

/* error handle */
void die(const char *s)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	perror(s); /* global error mess */
	exit(1);   /* exit program */
}

void quit()
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	exit(0);  
}

/* restore terminal config */
void disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
		die("tcsetattr");

	/*
	 * Test display file
	 *
	 * Loop row : i = 0 -> E.numrows
	 * Loop col	: j = 0 -> E.row[i].size
	 * --> Char at : E.row[i].chars[j]
	 */
	int i, j;
	for (i = 0; i < E.numrows; i++)
	{
		printf("[%d] ", i);
		for (j = 0; j < E.row[i].size; j++)
		{
			printf("[%d]:[%d]-[%c] ", j, E.row[i].chars[j], E.row[i].chars[j]);
		}
		printf("\n");
	}
}

/* config terminal mode */
void enableRawMode()
{
	if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
		die("tcgetattr");

	/* auto call disableRawMode when exit */
	atexit(disableRawMode);

	/* get termios struct */
	struct termios raw = E.orig_termios;

	/* config termios */

	/*
	 *	IXON	:	turn off Ctrl-S, Ctrl-Q
	 */
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);

	/*
	 *	ECHO 	: 	turn off echo
	 *	ICANON 	:	read byte by byte (without enter)
	 *	ISIG	:	turn off Ctrl-C, Ctrl-Z
	 */
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

	raw.c_cc[VMIN] = 0;	 /* min byte read return */
	raw.c_cc[VTIME] = 1; /* time await read */

	/* set termios */
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("tcsetattr");
}

/*
 *	get user input from keyboard
 *	it is first input
 */
int editorReadKey()
{
	int nread;

	/* for save input character */
	char c;

	/*
	 *	read 1 byte from input
	 *	stop when timout
	 * 	stop when have input
	 */
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
	{
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}

	return c;
	/*if (c == '\x1b')
	{
		char seq[3];

		if (read(STDIN_FILENO, &seq[0], 1) != 1)
			return '\x1b';
		if (read(STDIN_FILENO, &seq[1], 1) != 1)
			return '\x1b';

		if (seq[0] == '[')
		{
			if (seq[1] >= '0' && seq[1] <= '9')
			{
				if (read(STDIN_FILENO, &seq[2], 1) != 1)
					return '\x1b';
				if (seq[2] == '~')
				{
					switch (seq[1])
					{
					case '1':
						return HOME_KEY;
					case '3':
						return DEL_KEY;
					case '4':
						return END_KEY;
					case '5':
						return PAGE_UP;
					case '6':
						return PAGE_DOWN;
					case '7':
						return HOME_KEY;
					case '8':
						return END_KEY;
					}
				}
			}
			else
			{
				switch (seq[1])
				{
				case 'A':
					return ARROW_UP;
				case 'B':
					return ARROW_DOWN;
				case 'C':
					return ARROW_RIGHT;
				case 'D':
					return ARROW_LEFT;
				case 'H':
					return HOME_KEY;
				case 'F':
					return END_KEY;
				}
			}
		}
		else if (seq[0] == 'O')
		{
			switch (seq[1])
			{
			case 'H':
				return HOME_KEY;
			case 'F':
				return END_KEY;
			}
		}

		return '\x1b';
	}
	else
	{
		return c;
	}*/
}

/* get real cursor position */
int getCursorPosition(int *rows, int *cols)
{
	char buf[32];
	unsigned int i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
		return -1;

	while (i < sizeof(buf) - 1)
	{
		if (read(STDIN_FILENO, &buf[i], 1) != 1)
			break;
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[')
		return -1;
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
		return -1;

	return 0;
}

/* get window size */
int getWindowSize(int *rows, int *cols)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
	{
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
			return -1;
		return getCursorPosition(rows, cols);
	}
	else
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}

/*
 * extern get window size
 * have check error and config size
 */
void updateWindowSize()
{
	if (getWindowSize(&E.screenrows, &E.screencols) == -1)
		die("getWindowSize");

	E.screenrows -= 2; /* make space for status and message bar */
}

/* watch width window */
void handleSigWinCh(int unused __attribute__((unused)))
{
	updateWindowSize();
	if (E.cy > E.screenrows)
		E.cy = E.screenrows - 1;
	if (E.cx > E.screencols)
		E.cx = E.screencols - 1;
	editorRefreshScreen();
}
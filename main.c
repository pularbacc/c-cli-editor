#include <stdio.h>
#include <signal.h>

#include "edef.h"
#include "estruct.h"
#include "terminal.h"
#include "file.h"
#include "output.h"
#include "input.h"

void initEditor()
{
	E.mode = 0;
	E.cx = 0;
	E.cy = 0;
	E.rx = 0;
	E.rowoff = 0;
	E.coloff = 0;
	E.numrows = 0;
	E.row = NULL;
	E.dirty = 0;
	E.filename = NULL;
	E.statusmsg[0] = '\0';
	E.statusmsg_time = 0;
	E.syntax = NULL;

	updateWindowSize();
	signal(SIGWINCH, handleSigWinCh);
}

int main(int argc, char *argv[])
{
	enableRawMode();
	initEditor();

	/* 
	 * open file if have file name 
	 * argv[1] is file name
	 */
	if (argc >= 2)
	{
		editorOpen(argv[1]);
	}

	editorSetStatusMessage(
		"HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");

	while (1)
	{
		editorRefreshScreen();
		editorProcessKeypress();
	}

	return 0;
}


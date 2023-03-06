#include "file.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>  
#include <unistd.h>
#include <errno.h>

#include "edef.h"
#include "estruct.h"
#include "syntax.h"
#include "terminal.h"
#include "row.h"
#include "input.h"
#include "output.h"

char *editorRowsToString(int *buflen)
{
	int totlen = 0;
	int j;
	for (j = 0; j < E.numrows; j++)
		totlen += E.row[j].size + 1;
	*buflen = totlen;

	char *buf = malloc(totlen);
	char *p = buf;
	for (j = 0; j < E.numrows; j++)
	{
		memcpy(p, E.row[j].chars, E.row[j].size);
		p += E.row[j].size;
		*p = '\n';
		p++;
	}

	return buf;
}

/* open file local */
void editorOpen(char *filename)
{
	free(E.filename);
	/* 
	 * get and save storage file name 
	 * strdup clone string
	 */
	E.filename = strdup(filename);	

	editorSelectSyntaxHighlight();	/* turn on syntax */

	FILE *fp = fopen(filename, "r");	/* get file */
	if (!fp)
		die("fopen");

	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;

	/*
	 * getline read row in file
	 * return -1 when end file
	 */
	while ((linelen = getline(&line, &linecap, fp)) != -1)
	{
		while (linelen > 0 && (line[linelen - 1] == '\n' ||
							   line[linelen - 1] == '\r'))
			linelen--;
		editorInsertRow(E.numrows, line, linelen);
	}

	free(line);
	fclose(fp);
	E.dirty = 0;
}

/* save file */
void editorSave()
{
	if (E.filename == NULL)
	{
		E.filename = editorPrompt("Save as: %s (ESC to cancel)", NULL);
		if (E.filename == NULL)
		{
			editorSetStatusMessage("Save aborted");
			return;
		}
		editorSelectSyntaxHighlight();
	}

	int len;
	char *buf = editorRowsToString(&len);

	int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
	if (fd != -1)
	{
		if (ftruncate(fd, len) != -1)
		{
			if (write(fd, buf, len) == len)
			{
				close(fd);
				free(buf);
				E.dirty = 0;
				editorSetStatusMessage("%d bytes written to disk", len);
				return;
			}
		}
		close(fd);
	}

	free(buf);
	editorSetStatusMessage("Can't save! I/O error: %s", strerror(errno));
}


/*** Manage buffer ***/

#include "buffer.h"

#include <stdlib.h>
#include <string.h>

#include "estruct.h"

/*
 *	append s to buffer ab
 */
void abAppend(struct abuf *ab, const char *s, int len)
{
	char *new = realloc(ab->b, ab->len + len);

	if (new == NULL)
		return;
		
	memcpy(&new[ab->len], s, len);
	ab->b = new;
	ab->len += len;
}

void abFree(struct abuf *ab)
{
	free(ab->b);
}

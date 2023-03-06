#ifndef BUFFER_H_
#define BUFFER_H_

#include "estruct.h"

extern void abAppend(struct abuf *, const char *s, int);
extern void abFree(struct abuf *);

#endif  
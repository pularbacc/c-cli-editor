#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "estruct.h"

extern void editorScroll();
extern void editorDrawRows(struct abuf *);
extern void editorDrawStatusBar(struct abuf *);
extern void editorDrawMessageBar(struct abuf *);
extern void editorRefreshScreen();
extern void editorSetStatusMessage(const char *, ...);
extern void showCommand();

#endif
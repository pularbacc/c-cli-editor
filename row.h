#ifndef ROW_H_
#define ROW_H_

#include "estruct.h"

extern int editorRowCxToRx(erow *, int);
extern int editorRowRxToCx(erow *, int);
extern void editorUpdateRow(erow *);
extern void editorInsertRow(int, char *, size_t);
extern void editorFreeRow(erow *);
extern void editorDelRow(int);
extern void editorRowInsertChar(erow *, int, int);
extern void editorRowAppendString(erow *, char *, size_t);
extern void editorRowDelChar(erow *, int);

#endif
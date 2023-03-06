#ifndef SYNTAX_H_
#define SYNTAX_H_

#include "estruct.h"

extern int is_separator(int);
extern void editorUpdateSyntax(erow *row);
extern int editorSyntaxToColor(int);
extern void editorSelectSyntaxHighlight();

#endif
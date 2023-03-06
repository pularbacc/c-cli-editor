#ifndef TERMINAL_H_
#define TERMINAL_H_

extern void die(const char *s);
extern void quit();
extern void disableRawMode();
extern void enableRawMode();
extern int editorReadKey();
extern int getCursorPosition(int *, int *);
extern int getWindowSize(int *, int *);
extern void updateWindowSize();
extern void handleSigWinCh(int unused __attribute__((unused)));

#endif
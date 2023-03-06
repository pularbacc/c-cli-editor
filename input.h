#ifndef INPUT_H_
#define INPUT_H_

/* marco check ctrl key */
#define CTRL_KEY(k) ((k)&0x1f)

extern char *editorPrompt(char *prompt, void (*callback)(char *, int));
extern void editorMoveCursor(int);
extern void editorProcessKeypress();

#endif
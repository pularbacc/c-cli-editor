/*** global defines ***/

#ifndef DEFINES_H_
#define DEFINES_H_

#include <time.h>
#include <termios.h>

/* for compiler test marco */
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#define KILO_VERSION "0.0.1"
#define KILO_TAB_STOP 8
#define KILO_QUIT_TIMES 1

#define HL_HIGHLIGHT_NUMBERS (1 << 0)
#define HL_HIGHLIGHT_STRINGS (1 << 1)

enum mode
{
	VIEW_MODE = 0,
	EDIT_MODE,
	COMMAND_MODE
};

enum editorKey
{
	BACKSPACE = 127,
	ARROW_LEFT = 104,
	ARROW_RIGHT = 108,
	ARROW_UP = 107,
	ARROW_DOWN = 106,
	ENTER_KEY = 13,
	ESC_KEY = 27,
	INSERT_KEY = 105,
	COMMAND_KEY = 58,
	DEL_KEY,
	HOME_KEY,
	END_KEY,
	PAGE_UP,
	PAGE_DOWN
};

enum editorHighlight
{
	HL_NORMAL = 0,
	HL_COMMENT,
	HL_MLCOMMENT,
	HL_KEYWORD1,
	HL_KEYWORD2,
	HL_STRING,
	HL_NUMBER,
	HL_MATCH
};

struct editorSyntax
{
	char *filetype;
	char **filematch;
	char **keywords;
	char *singleline_comment_start;
	char *multiline_comment_start;
	char *multiline_comment_end;
	int flags;
};

/* 
 * Structure for row file 
 * array character one line -> row data
 * multiple row data -> file data  
 */
typedef struct erow
{
	int idx;
	int size;					/* num character in row */
	int rsize;
	char *chars;				/* array character in one line */
	char *render;
	unsigned char *hl;
	int hl_open_comment;
} erow;

typedef struct ecommand
{
	char * c;
	int size;
} ecommand;


/* structure for global data  */
struct editorConfig
{
	int mode;							/* mode edit */
	int cx, cy;							/* cursor position in file */
	int rx;								/* index into render field */
	int rowoff;							/* row offset */
	int coloff;							/* col offset */
	int screenrows, screencols;			/* window size */
	int numrows;						/* num row file */
	erow *row;							/* array row file */
	int dirty;
	char *filename;						/* file name open */
	char statusmsg[80];
	ecommand command;
	time_t statusmsg_time;
	struct editorSyntax *syntax;
	struct termios orig_termios;		/* terminal config */
};

#define ABUF_INIT \
	{             \
		NULL, 0   \
	}

#define E_COMMAND_INIT  \
	{					\
		NULL, 0			\
	}

/* structure data for write screen  */
struct abuf
{
	char *b;		/* buffer */
	int len;		/* len buffer */
};

#endif

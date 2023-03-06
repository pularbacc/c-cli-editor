#ifndef COMMAND_H_
#define COMMAND_H_

#include "estruct.h"

extern void cmInit();
extern void cmAppend(const char);
extern void cmDel();
extern void cmFree();
extern void excuteCommand();

#endif  
#ifndef TOOLS_H
#define TOOLS_H

#include <string.h>
#include "linenoise/linenoise.h"
#include "commands.h"

void completionHook (char const* prefix, linenoiseCompletions* lc);

#endif /* TOOLS_H */
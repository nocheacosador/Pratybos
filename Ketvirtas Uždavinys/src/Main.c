#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "Commander.h"

#include <memory.h>
#include <ctype.h>

#define ARGV_SIZE 64

int main()
{
    char  cmdBuffer[256];
    char* argv[ARGV_SIZE] = { NULL };
    int   argc = 0;

    while (!ShouldQuit())
    {
        fputs("> ", stdout);
        fgets(cmdBuffer, 256, stdin);
        ConvertToArgv(cmdBuffer, &argc, argv, ARGV_SIZE);
        
        if (!argc) continue;
        CallCommand(argc, argv);

        for (int i = 0; i < argc; i++)
            free(argv[i]);
    }

    return 0;
}
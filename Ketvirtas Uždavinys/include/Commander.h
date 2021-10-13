#pragma once

typedef void CommandFunc(int, char*[]);

typedef struct CommandInfo 
{
    CommandFunc* cmd;
    char* cmdName;
    char* helpMsg;
} CommandInfo;

void ConvertToArgv(const char* buf, int* argc, char* argv[], int argvSize);
void CallCommand(int argc, char* argv[]);
int  ShouldQuit();
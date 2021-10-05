/* *****************************************************
 *  Lukas Zajončkovskis
 *  lukas.zajonckovskis@mif.stud.vu.lt
 *  Studento pažymejmo Nr. 2110647
 *  Užduotis Nr. 3, 6 variantas
 *
 *  Refleksija:
 *      Užduotis užrtuko beveik tiek kiek ir planavau, apie 2 val. Bedarant 
 *      užduotį didesnių sunkumų nekilo, vienintelės vietos kur užtrukau 
 *      ilgiau nei tikėjausi, tai ties eilučių nuskaitymu bandžiau rast būdų 
 *      panaudoti gets_s funkciją, bet gcc nepateikia jos implementacijos 
 *      standartinėj bibliotekoj.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(DEBUG)
// malloc ir free wrapperiai, padedantys detectinti memory-leak'us
static size_t g_MallocCallCount = 0;
static size_t g_FreeCallCount = 0;

void* _Malloc(size_t size)
{
    void* ptr = malloc(size);
    if (ptr) g_MallocCallCount++;
    return ptr;
}

void _Free(void* block)
{
    g_FreeCallCount++;
    free(block);
}

void _MemoryReport()
{
    printf("[MemoryReport] malloc calls: %lu, free calls: %lu, diff: %lu\n",
        g_MallocCallCount, g_FreeCallCount, g_MallocCallCount - g_FreeCallCount);
}
#endif

#if defined(DEBUG)
#   define Malloc(size)     _Malloc(size)
#   define Free(block)      _Free(block)
#   define MemoryReport()   _MemoryReport()
#else
#   define Malloc(size)     malloc(size)
#   define Free(block)      free(block)
#   define MemoryReport()
#endif

// return 1 if newline was found
int removeNewline(char* str)
{
    while (*str != '\n')
    {
        str++;
        if (*str == '\0')
            return 0;
    } 

    *str = '\0';
    return 1;
}

void reverseLineWordOrder(char* line)
{
    typedef struct StackNode
    {
        char*             string;
        size_t            stringSize;
        struct StackNode* prev;
    } StackNode;
    
    char* lineReadPtr = line;
    StackNode* topOfStack = NULL;

    do {
        while (*lineReadPtr == ' ' && *lineReadPtr != '\0') lineReadPtr++;

        const char* wordStartPtr = lineReadPtr;
        size_t wordSize = 0;

        while (*lineReadPtr != ' ' && *lineReadPtr != '\0') 
        {
            wordSize++;
            lineReadPtr++;
        }

        if (wordSize)
        {
            StackNode* node = (StackNode*)Malloc(sizeof(StackNode));
            
            if (!node)
            {
                perror("Error while allocating memory");
                break;
            }
            
            node->prev = topOfStack;
            node->string = (char*)Malloc(wordSize + 1);
            node->stringSize = wordSize;

            if (!node->string)
            {
                perror("Error while allocating memory");
                Free(node);
                break;
            }
            
            memcpy(node->string, wordStartPtr, wordSize);
            node->string[wordSize] = '\0';

            topOfStack = node;
        }
    } while (*lineReadPtr != '\0');

    char* lineWritePtr = line;

    while (topOfStack)
    {
        memcpy(lineWritePtr, topOfStack->string, topOfStack->stringSize);
        lineWritePtr += topOfStack->stringSize;
        *lineWritePtr = ' ';
        lineWritePtr++;

        StackNode* nodeToFree = topOfStack;
        topOfStack = topOfStack->prev;
        Free(nodeToFree->string);
        Free(nodeToFree);
    }

    *lineWritePtr = '\0';
}

int main(int argc, char* argv[])
{
    char inputFileName[256];    // file name length limit on linux and windows is 255
    char outFileName[256];      

    printf("Enter input file (max 255 chararcters): \n"
           " > ");
    fgets(inputFileName, 256, stdin);
    removeNewline(inputFileName);

    printf("Enter output file (max 255 chararcters, cannot be the same as input file): \n"
           " > ");
    fgets(outFileName, 256, stdin);
    removeNewline(outFileName);

    if (!strcmp(inputFileName, outFileName))
    {
        puts("You entered same filenames!");
        exit(EXIT_FAILURE);
    }

    FILE* inFile = fopen(inputFileName, "r");
    if (!inFile)
    {
        char buf[280];
        sprintf(buf, "Failed to open file '%s'", inputFileName);
        perror(buf);
        exit(EXIT_FAILURE);
    }

    FILE* outFile = fopen(outFileName, "w");
    if (!outFile)
    {
        char buf[280];
        sprintf(buf, "Failed to open file '%s'", outFileName);
        perror(buf);
        exit(EXIT_FAILURE);
    }

    printf("Reading from '%s'...\n"
           "Writting to '%s'...\n", 
           inputFileName, outFileName);

    while (!feof(inFile))
    {
        char buf[256];
        buf[255] = '\0';
        
        if (fgets(buf, 256, inFile) == NULL)
        {
            perror("Error occured while reading file");
            
            fclose(inFile);
            fclose(outFile);
            
            exit(EXIT_FAILURE);
        }

        if (!removeNewline(buf))
        {
            char c;
            do {
                c = fgetc(inFile);
            } while (c != '\n' && c != '\0' && !feof(inFile));
        }

        reverseLineWordOrder(buf);
        
        if (fputs(buf, outFile) == EOF || fputc('\n', outFile) == EOF)
            perror("Error occured while writing to file");
    }

    fclose(inFile);
    fclose(outFile);

    puts("Done!");

    MemoryReport();
    exit(EXIT_SUCCESS);
}
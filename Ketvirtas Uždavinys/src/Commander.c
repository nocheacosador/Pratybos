#include "Commander.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <signal.h>
#include <ctype.h>

#include "StringList.h"

typedef struct ListInfoNode
{
    char* name;
    SList list;
    struct ListInfoNode* next;
} ListInfoNode;


static volatile int s_ShouldQuit = 0;

static ListInfoNode* s_ListInfoHead = NULL;

static void Help(int argc, char* argv[]);
static void Quit(int argc, char* argv[]);
static void NewList(int argc, char* argv[]);
static void PrintList(int argc, char* argv[]);
static void PushFront(int argc, char* argv[]);
static void PushBack(int argc, char* argv[]);
static void PopFront(int argc, char* argv[]);
static void PopBack(int argc, char* argv[]);
static void DeleteList(int argc, char* argv[]);
static void ListLists(int argc, char* argv[]);
static void SortList(int argc, char* argv[]);
static void IsSorted(int argc, char* argv[]);
static void MergeLists(int argc, char* argv[]);
static void Compress(int argc, char* argv[]);
static void Equal(int argc, char* argv[]);

static const CommandInfo s_CommandInfoTable[] = {
    { Help,       "help",   "\thelp                           Lists available commands\n"
                            "\thelp <command>                 Get usage\n"
                            "\thelp --all                     Prints usage of all commands" },
    { Help,       "menu",   "\tmenu                           Lists available commands (alias for help)" },
    { Quit,       "quit",   "\tquit                           Quits the application" },
    { NewList,    "new",    "\tnew <name>                     Creates empty list named 'name'\n"
                            "\tnew <name> -f <file>           Creates list and initializes it with\n"
                            "\t                               values from the file\n"
                            "\tnew <name> <v1> <v2> ...       Creates list and initializes it with\n"
                            "\t                               values 'd1', 'd2' and so on\n" },
    { DeleteList, "delete", "\tdelete <list> [n]              Deletes list" },
    { PrintList,  "print",  "\tprint <list> [-r]              Prints list, if '-r' flag is set prints\n"
                            "\t                               reverse order" },
    { PushFront,  "pushf",  "\tpushf <list> <v1> <v2> ...     Pushes values 'v1', 'v2' to the front\n"
                            "\t                               of the list" },
    { PushBack,   "pushb",  "\tpushb <list> <v1> <v2> ...     Pushes values 'v1', 'v2' to the back\n"
                            "\t                               of the list" },
    { PopFront,   "popf",   "\tpopf <list> [n]                Pops value from the front of the list,\n"
                            "\t                               if 'n' is specified pops n values" },
    { PopBack,    "popb",   "\tpopb <list> [n]                Pops value from the back of the list,\n"
                            "\t                               if 'n' is specified pops n values" },
    { ListLists,  "ls",     "\tls                             Lists all lists" },
    { SortList,   "sort",   "\tsort <list> [flags]            Sorts list, flags determine how list is\n"
                            "\t                               sorted. 'Sort by' flags:\n"
                            "\t                                 --alpha - sorts alphabetically [default]\n"
                            "\t                                 --size  - sorts by size\n"
                            "\t                               Sorting order flags:\n"
                            "\t                                 --asc   - ascending order [default]\n"
                            "\t                                 --desc  - descending order\n"
                            "\t                               Other flags:\n"
                            "\t                                 -v - verbode" },
    { IsSorted, "issorted", "\tissorted <list>                Checks if and how list is sorted." },
    { MergeLists, "merge",  "\tmerge <list1> <list2> [flags]  Merges two sorted lists.'Sorted by'\n"
                            "\t                               flags:\n"
                            "\t                                 --alpha - sorts alphabetically [default]\n"
                            "\t                                 --size  - sorts by size\n"
                            "\t                               Sorting order flags:\n"
                            "\t                                 --asc   - ascending order [default]\n"
                            "\t                                 --desc  - descending order\n"
                            "\t                               Other flags:\n"
                            "\t                                 -v - verbode" },
    { Compress,   "compr",  "\tcompr <list>                   Compress list." },
    { Equal,     "compare", "\tcompare <list1> <list2>        Compares two lists." }
};

static const int s_CommandInfoTableSize = sizeof(s_CommandInfoTable) / sizeof(CommandInfo);

void ConvertToArgv(const char* buf, int* argc, char* argv[], int argvSize)
{
    char* bufPtr = (char*)buf;
    *argc = 0;
    
    while (*bufPtr != '\n' && *bufPtr != '\r' && *bufPtr != '\0')
    {
        while (*bufPtr == ' ' && *bufPtr != '\n' && *bufPtr != '\r' && *bufPtr != '\0') 
            bufPtr++;
        
        if (*bufPtr == '\'' || *bufPtr == '"')
        {
            char delimitter = *bufPtr;
            char* start = ++bufPtr;

            while (*bufPtr != delimitter && *bufPtr != '\n' && *bufPtr != '\r' && *bufPtr != '\0') 
                bufPtr++;

            size_t size = (size_t)(bufPtr - start);
            argv[*argc] = (char*)malloc(size + 1);
            if (!argv[*argc])
            {
                perror("Error while allocating memory");
                return;
            }

            memcpy(argv[*argc], start, size);
            argv[*argc][size] = '\0';
            (*argc)++;

            while (*bufPtr != ' ' && *bufPtr != '\n' && *bufPtr != '\r' && *bufPtr != '\0') 
                bufPtr++;
        }
        else
        {
            char* start = bufPtr;

            while (*bufPtr != ' ' && *bufPtr != '\n' && *bufPtr != '\r' && *bufPtr != '\0') 
                bufPtr++;

            size_t size = (size_t)(bufPtr - start);
            argv[*argc] = (char*)malloc(size + 1);
            if (!argv[*argc])
            {
                perror("Error while allocating memory");
                return;
            }

            memcpy(argv[*argc], start, size);
            argv[*argc][size] = '\0';
            (*argc)++;
        }

        if (*argc == argvSize)
            break;
    }
}

void CallCommand(int argc, char* argv[])
{
    for (int i = 0; i < s_CommandInfoTableSize; i++)
    {
        if (!strcmp(argv[0], s_CommandInfoTable[i].cmdName))
        {
            s_CommandInfoTable[i].cmd(argc, argv);
            return;
        }
    }

    printf("Command '%s' not found. Type 'help' to see available commands.\n", argv[0]);
}

int ShouldQuit()
{
    return s_ShouldQuit;
}

// commands
static void Help(int argc, char* argv[])
{
    if (argc == 1)
    {
        puts("Hi! I'm here to let you goof around with lists.");
        
        for (int i = 0; i < s_CommandInfoTableSize; i++)
        {
            if (!strcmp(argv[0], s_CommandInfoTable[i].cmdName))
            {
                printf("  Usage:\n%s\n", s_CommandInfoTable[i].helpMsg);
                break;
            }
        }

        puts("  Available commands:");

        int rows = (s_CommandInfoTableSize + 1) / 2;
        for (int i = 0; i < rows; i++)
        {
            printf("\t%s", s_CommandInfoTable[i].cmdName);

            if (rows + i < s_CommandInfoTableSize)
                printf("\t\t%s\n", s_CommandInfoTable[rows + i].cmdName);
            else
                putchar('\n');
        }
    }
    else if (argc == 2)
    {
        if (!strcmp(argv[1], "--all"))
        {
            for (int i = 0; i < s_CommandInfoTableSize; i++)
            {
                printf("%s\n", s_CommandInfoTable[i].helpMsg);
            }
            return;
        }
        for (int i = 0; i < s_CommandInfoTableSize; i++)
        {
            if (!strcmp(argv[1], s_CommandInfoTable[i].cmdName))
            {
                printf("  Usage:\n%s\n", s_CommandInfoTable[i].helpMsg);
                return;
            }
        }
        
        puts("There's no such command.");
    }
    else
        puts("Too many arguments.");
}

static void Quit(int argc, char* argv[])
{
    // delete all lists, free all members
    
    s_ShouldQuit = 1;
}

static void NewList(int argc, char* argv[])
{
    if (argc == 1)
        puts("Need more arguments.");
    else
    {
        ListInfoNode* listInfo = (ListInfoNode*)malloc(sizeof(ListInfoNode));
        listInfo->name = (char*)malloc(strlen(argv[1]) + 1);
        if (!listInfo->name)
        {
            perror("Error while allocating memory");
            return;
        }
        strcpy(listInfo->name, argv[1]);

        listInfo->list = SList_Init();
        listInfo->next = s_ListInfoHead;
        s_ListInfoHead = listInfo;
    
        if (argc > 2)
        {
            if (!strcmp(argv[2], "-f"))
            {
                if (argc > 3)
                {
                    FILE* file = fopen(argv[3], "r");
                    if (!file)
                    {
                        char buf[280];
                        sprintf(buf, "Failed to open file '%s'", argv[3]);
                        perror(buf);
                        return;
                    }

                    size_t size = 256;
                    char* buf = (char*)malloc(size);
                    if (!buf)
                    {
                        char buf[280];
                        sprintf(buf, "Failed to open file '%s'", argv[3]);
                        perror(buf);
                        fclose(file);
                        return;
                    }

                    while(!feof(file))
                    {
                        char*  items[128] = { NULL };
                        int    itemc = 0;
                        getline(&buf, &size, file);
                        ConvertToArgv(buf, &itemc, items, 128);

                        for (int i = 0; i < itemc; i++)
                        {
                            SList_EmplaceBack(&(listInfo->list), items[i]);
                            free(items[i]);
                        }
                    }

                    fclose(file);
                }
                else
                    puts("File not specified. Missing argument.");
            }
            else
            {
                for (int i = 2; i < argc; i++)
                    SList_EmplaceBack(&(listInfo->list), argv[i]);
            }
        }
    }
}

static void PrintList(int argc, char* argv[])
{
    if (argc == 1)
        puts("Need more arguments.");
    else
    {
        ListInfoNode* listInfo = s_ListInfoHead;
        for ( ; listInfo != NULL; listInfo = listInfo->next)
        {
            if (!strcmp(argv[1], listInfo->name)) break;
        }

        if (!listInfo)
        {
            printf("There is no list named '%s'.\n", argv[1]);
            return;
        }

        if (argc > 2 && !strcmp(argv[2], "-r"))
            SList_PrintReverseLn(&(listInfo->list));
        else
            SList_PrintLn(&(listInfo->list));
    }
}

static void PushFront(int argc, char* argv[])
{
    if (argc < 3)
    {
        puts("Too few arguments.");
        return;
    }

    ListInfoNode* listInfo = s_ListInfoHead;
    for ( ; listInfo != NULL; listInfo = listInfo->next)
    {
        if (!strcmp(argv[1], listInfo->name)) break;
    }
 
    if (!listInfo)
    {
        printf("There is no list named '%s'.\n", argv[1]);
        return;
    }

    for (int i = 2; i < argc; i++)
        SList_EmplaceFront(&(listInfo->list), argv[i]);
}

static void PushBack(int argc, char* argv[])
{
    if (argc < 3)
    {
        puts("Too few arguments.");
        return;
    }

    ListInfoNode* listInfo = s_ListInfoHead;
    for ( ; listInfo != NULL; listInfo = listInfo->next)
    {
        if (!strcmp(argv[1], listInfo->name)) break;
    }
 
    if (!listInfo)
    {
        printf("There is no list named '%s'.\n", argv[1]);
        return;
    }

    for (int i = 2; i < argc; i++)
        SList_EmplaceBack(&(listInfo->list), argv[i]);
}

static void PopFront(int argc, char* argv[])
{
    if (argc < 4)
    {
        if (argc == 1)
        {
            puts("Too few arguments.");
            return;
        }

        ListInfoNode* listInfo = s_ListInfoHead;
        for ( ; listInfo != NULL; listInfo = listInfo->next)
        {
            if (!strcmp(argv[1], listInfo->name)) break;
        }
 
        if (!listInfo)
        {
            printf("There is no list named '%s'.\n", argv[1]);
            return;
        }

        if (argc == 3)
        {
            char* s = argv[2];
            while (*s) 
            {
                if (isdigit(*s++) == 0)
                {
                    puts("Cannot convert positional argument to integer."
                         " Value must be positive whole number.");
                    return;
                }
            }

            int n = atoi(argv[2]);

            for (int i = 0; i < n; i++)
            {
                if (listInfo->list.begin == NULL)
                    break;
                SList_PopFront(&(listInfo->list));
            }
        }
        else
            SList_PopFront(&(listInfo->list));
    }
    else
        puts("Too many arguments.");
}

static void PopBack(int argc, char* argv[])
{
    if (argc < 4)
    {
        if (argc == 1)
        {
            puts("Too few arguments.");
            return;
        }

        ListInfoNode* listInfo = s_ListInfoHead;
        for ( ; listInfo != NULL; listInfo = listInfo->next)
        {
            if (!strcmp(argv[1], listInfo->name)) break;
        }
 
        if (!listInfo)
        {
            printf("There is no list named '%s'.\n", argv[1]);
            return;
        }

        if (argc == 3)
        {
            char* s = argv[2];
            while (*s) 
            {
                if (isdigit(*s++) == 0)
                {
                    puts("Cannot convert positional argument to integer."
                         " Value must be positive whole number.");
                    return;
                }
            }

            int n = atoi(argv[2]);

            for (int i = 0; i < n; i++)
            {
                if (listInfo->list.end == NULL)
                    break;
                SList_PopBack(&(listInfo->list));
            }
        }
        else
            SList_PopBack(&(listInfo->list));
    }
    else
        puts("Too many arguments.");
}

static void DeleteList(int argc, char* argv[])
{
    if (argc == 1)
    {
        puts("No arguments provided.");
        return;
    }
    else if (argc > 2)
    {
        puts("Too many arguments provided.");
        return;
    }

    ListInfoNode* listInfo = s_ListInfoHead;
    ListInfoNode* prev;
 
    if (listInfo != NULL && !strcmp(listInfo->name, argv[1])) 
    {
        s_ListInfoHead = listInfo->next;
        
        SList_Delete(&(listInfo->list));
        free(listInfo->name);
        free(listInfo);
        return;
    }
 
    
    while (listInfo != NULL && strcmp(listInfo->name, argv[1])) {
        prev = listInfo;
        listInfo = listInfo->next;
    }
 
    if (!listInfo)
    {
        printf("There is no list named '%s'.\n", argv[1]);
        return;
    }
 
    prev->next = listInfo->next;
 
    SList_Delete(&(listInfo->list));
    free(listInfo->name);
    free(listInfo);
}

static void ListLists(int argc, char* argv[])
{
    ListInfoNode* listInfo = s_ListInfoHead;

    while (listInfo)
    {
        printf("'%s' ", listInfo->name);
        listInfo = listInfo->next;
    }

    putchar('\n');
}

static int FlagIsSet(const char* flag, int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
        if (!strcmp(argv[i], flag))
            return 1;
    return 0;
}

static int AlphaAscendingComp(const char* restrict str1, const char* restrict str2)
{
    return strcmp(str1, str2) > 0;
}

static int AlphaDescendingComp(const char* restrict str1, const char* restrict str2)
{
    return strcmp(str1, str2) < 0;
}

static int SizeAscendingComp(const char* restrict str1, const char* restrict str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 == len2) return strcmp(str1, str2) > 0;
    else return len1 > len2;
}

static int SizeDescendingComp(const char* restrict str1, const char* restrict str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 == len2) return strcmp(str1, str2) < 0;
    else return len1 < len2;
}

static void SortList(int argc, char* argv[])
{
    if (argc == 1)
    {
        puts("No arguments provided.");
        return;
    }
    
    ListInfoNode* listInfo = s_ListInfoHead;
    for ( ; listInfo != NULL; listInfo = listInfo->next)
    {
        if (!strcmp(argv[1], listInfo->name)) break;
    }
    if (!listInfo)
    {
        printf("There is no list named '%s'.\n", argv[1]);
        return;
    }

    int verbose = FlagIsSet("-v", argc, argv);
    int bySize = FlagIsSet("--size", argc, argv);
    int descending = FlagIsSet("--desc", argc, argv);

    SortCompareFunc* comp;

    if (bySize)
    {
        if (descending) comp = SizeDescendingComp;
        else            comp = SizeAscendingComp;
    }
    else
    {
        if (descending) comp = AlphaDescendingComp;
        else            comp = AlphaAscendingComp;
    }

    SList_Sort(&(listInfo->list), comp, verbose);
}

static void IsSorted(int argc, char* argv[])
{
    if (argc == 1)
    {
        puts("No arguments provided.");
        return;
    }
    
    ListInfoNode* listInfo = s_ListInfoHead;
    for ( ; listInfo != NULL; listInfo = listInfo->next)
    {
        if (!strcmp(argv[1], listInfo->name)) break;
    }
    if (!listInfo)
    {
        printf("There is no list named '%s'.\n", argv[1]);
        return;
    }

    int sorted = 0;
    if (sorted |= SList_IsSorted(&(listInfo->list), AlphaAscendingComp))
        puts("Is sorted in ascending alphabetical order.");
    if (sorted |= SList_IsSorted(&(listInfo->list), AlphaDescendingComp))
        puts("Is sorted in descending alphabetical order.");
    if (sorted |= SList_IsSorted(&(listInfo->list), SizeAscendingComp))
        puts("Is sorted in ascending by size order.");
    if (sorted |= SList_IsSorted(&(listInfo->list), SizeDescendingComp))
        puts("Is sorted in descending by size order.");

    if (!sorted)
        puts("Nope.");
}

static void MergeLists(int argc, char* argv[])
{
    if (argc < 3)
    {
        puts("Too few arguments provided.");
        return;
    }

    ListInfoNode* list1Info = s_ListInfoHead;

    for ( ; list1Info != NULL; list1Info = list1Info->next)
    {
        if (!strcmp(argv[1], list1Info->name)) break;
    }
    if (!list1Info)
    {
        printf("There is no list named '%s'.\n", argv[1]);
        return;
    }

    ListInfoNode* list2Info = s_ListInfoHead;
    
    for ( ; list2Info != NULL; list2Info = list2Info->next)
    {
        if (!strcmp(argv[2], list2Info->name)) break;
    }
    if (!list2Info)
    {
        printf("There is no list named '%s'.\n", argv[2]);
        return;
    }

    if (list1Info == list2Info)
    {
        puts("Can't merge list with itself.");
        return;
    }

    int verbose = FlagIsSet("-v", argc, argv);
    int bySize = FlagIsSet("--size", argc, argv);
    int descending = FlagIsSet("--desc", argc, argv);

    SortCompareFunc* comp;

    if (bySize)
    {
        if (descending) comp = SizeDescendingComp;
        else            comp = SizeAscendingComp;
    }
    else
    {
        if (descending) comp = AlphaDescendingComp;
        else            comp = AlphaAscendingComp;
    }

    if (!SList_IsSorted(&(list1Info->list), comp))
    {
        printf("'%s' is not sorted.\n", list1Info->name);
        return;
    }
    
    if (!SList_IsSorted(&(list2Info->list), comp))
    {
        printf("'%s' is not sorted.\n", list2Info->name);
        return;
    }
    
    SList_Merge(&(list1Info->list), &(list2Info->list), comp, verbose);
}

static void Compress(int argc, char* argv[])
{
    if (argc == 1)
    {
        puts("Too few arguments.");
        return;
    }

    if (argc > 2)
    {
        puts("Too many arguments.");
        return;
    }

    ListInfoNode* listInfo = s_ListInfoHead;
    for ( ; listInfo != NULL; listInfo = listInfo->next)
    {
        if (!strcmp(argv[1], listInfo->name)) break;
    }
    if (!listInfo)
    {
        printf("There is no list named '%s'.\n", argv[1]);
        return;
    }

    SList_Compress(&listInfo->list);
}

static void Equal(int argc, char* argv[])
{
    if (argc < 3)
    {
        puts("Too few arguments.");
        return;
    }
    if (argc > 3)
    {
        puts("Too many arguments.");
        return;
    }
    ListInfoNode* list1Info = s_ListInfoHead;

    for ( ; list1Info != NULL; list1Info = list1Info->next)
    {
        if (!strcmp(argv[1], list1Info->name)) break;
    }
    if (!list1Info)
    {
        printf("There is no list named '%s'.\n", argv[1]);
        return;
    }

    ListInfoNode* list2Info = s_ListInfoHead;
    
    for ( ; list2Info != NULL; list2Info = list2Info->next)
    {
        if (!strcmp(argv[2], list2Info->name)) break;
    }
    if (!list2Info)
    {
        printf("There is no list named '%s'.\n", argv[2]);
        return;
    }

    if (SList_Compare(&list1Info->list, &list2Info->list))
    {
        printf("'%s' is equal to '%s'.\n", list1Info->name, list2Info->name);
    }
    else
    {
        printf("'%s' and '%s' are not equal.\n", list1Info->name, list2Info->name);
    }
}
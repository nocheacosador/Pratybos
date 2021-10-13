#pragma once

#include <stddef.h>

typedef struct SListNode 
{
    struct SListNode* prev;
    struct SListNode* next;
    char* str;
} SListNode;

typedef struct SList
{
    SListNode* begin;
    SListNode* end;
} SList;

typedef int SortCompareFunc(const char*, const char*);

SList SList_Init();

void SList_EmplaceFront(SList* list, const char* str);

void SList_EmplaceBack(SList* list, const char* str);

void SList_PopFront(SList* list);

void SList_PopBack(SList* list);

void SList_Print(const SList* list);

void SList_PrintLn(const SList* list);

void SList_PrintReverse(const SList* list);

void SList_PrintReverseLn(const SList* list);

void SList_Sort(SList* list, SortCompareFunc* compFunc, int verbose);

inline int SList_IsEmpty(const SList* list)
{
    return list->begin == NULL;
}

void SList_Delete(SList* list);

void SList_Merge(SList* list, SList* pray, SortCompareFunc* compFunc, int verbose);

int SList_IsSorted(const SList* list, SortCompareFunc* compFunc);

void SListNode_Delete(SListNode* node);

int SList_Compare(const SList* list1, const SList* list2);

void SList_Compress(SList* list);
#include "StringList.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

static inline void SListNode_InsertBefore(SListNode* dest, SListNode* node)
{
    assert(dest != NULL);
    assert(node != NULL);

    node->prev = dest->prev;
    node->next = dest;
    dest->prev = node;
    if (node->prev) node->prev->next = node;
}

static SListNode* SListNode_EmplaceBefore(SListNode* node, const char* str)
{
    assert(str != NULL);

    SListNode* newNode = (SListNode*)malloc(sizeof(SListNode));
    if (!newNode)
    {
        perror("Error occured while allocating new node");
        return node;
    }
    memset(newNode, 0, sizeof(SListNode));

    size_t stringLengh = strlen(str) + 1;
    newNode->str = (char*)malloc(stringLengh);
    
    if (!newNode->str)
    {
        perror("Error occured while allocating memory");
        free(newNode);
        return node;
    }

    memcpy(newNode->str, str, stringLengh);

    if (node) SListNode_InsertBefore(node, newNode);

    return newNode;
}

static inline void SListNode_InsertAfter(SListNode* dest, SListNode* node)
{
    assert(dest != NULL);
    assert(node != NULL);
    
    node->next = dest->next;
    node->prev = dest;
    dest->next = node;
    if (node->next) node->next->prev = node;
}

static SListNode* SListNode_EmplaceAfter(SListNode* node, const char* str)
{
    assert(str != NULL);
    
    SListNode* newNode = (SListNode*)malloc(sizeof(SListNode));
    if (!newNode)
    {
        perror("Error occured while allocating new node");
        return node;
    }

    size_t stringLengh = strlen(str) + 1;
    newNode->str = (char*)malloc(stringLengh);
    
    if (!newNode->str)
    {
        perror("Error occured while allocating memory");
        free(newNode);
        return node;
    }

    memcpy(newNode->str, str, stringLengh);

    if (node) SListNode_InsertAfter(node, newNode);

    return newNode;
}

void SListNode_Delete(SListNode* node)
{
    if (!node) return;

    SListNode* prev = node->prev;
    SListNode* next = node->next;

    if (prev) prev->next = next;
    if (next) next->prev = prev;

    free(node->str);
    free(node);
}

SList SList_Init()
{
    static const SList emptyList = { (SListNode*)NULL, (SListNode*)NULL }; 
    return emptyList;
}

void SList_EmplaceFront(SList* list, const char* str)
{
    assert(list != NULL);
    assert(str != NULL);

    list->begin = SListNode_EmplaceBefore(list->begin, str);
    if (list->end == NULL)
        list->end = list->begin;
}

void SList_EmplaceBack(SList* list, const char* str)
{
    assert(list != NULL);
    assert(str != NULL);

    list->end = SListNode_EmplaceAfter(list->end, str);
    if (list->begin == NULL)
        list->begin = list->end;
}

void SList_PopFront(SList* list)
{
    assert(list != NULL);

    SListNode* newBegin = NULL;
    if (list->begin) newBegin = list->begin->next;
    
    SListNode_Delete(list->begin);
    
    list->begin = newBegin;
    if (list->begin == NULL) list->end = NULL;
}

void SList_PopBack(SList* list)
{
    assert(list != NULL);

    SListNode* newEnd = NULL;
    if (list->end) newEnd = list->end->prev;
    
    SListNode_Delete(list->end);
    
    list->end = newEnd;
    if (list->end == NULL) list->begin = NULL;
}

void SList_Print(const SList* list)
{
    assert(list != NULL);

    putchar('[');
    if (list->begin)
    {
        SListNode* node;
        for (node = list->begin; node != list->end; node = node->next)
        {
            printf("%s <-> ", node->str);
        }

        fputs(node->str, stdout);
    }
    putchar(']');
}

void SList_PrintLn(const SList* list)
{
    assert(list != NULL);

    putchar('[');
    if (list->begin)
    {
        SListNode* node;
        for (node = list->begin; node != list->end; node = node->next)
        {
            printf("%s <-> ", node->str);
        }

        fputs(node->str, stdout);
    }
    putchar(']');
    putchar('\n');
}

void SList_PrintReverse(const SList* list)
{
    assert(list != NULL);

    putchar('[');
    if (list->end)
    {
        SListNode* node;
        for (node = list->end; node != list->begin; node = node->prev)
        {
            printf("%s <-> ", node->str);
        }

        fputs(node->str, stdout);
    }
    
    putchar(']');
}

void SList_PrintReverseLn(const SList* list)
{
    assert(list != NULL);

    putchar('[');
    if (list->end)
    {
        SListNode* node;
        for (node = list->end; node != list->begin; node = node->prev)
        {
            printf("%s <-> ", node->str);
        }

        fputs(node->str, stdout);
    }
    
    putchar(']');
    putchar('\n');
}

static SList Split(SList* list)
{
    assert(list != NULL);

    SList result = { NULL, NULL };
    SListNode* fast = list->begin;
    SListNode* slow = list->begin;

    if (!slow) return result;

    while (fast != list->end)
    {
        fast = fast->next;
        if (fast != list->end) fast = fast->next;
        else break;

        slow = slow->next;
    }

    if (slow->next) 
    {
        result.begin = slow->next;
        result.end = list->end;
        result.begin->prev = NULL;
    }

    list->end = slow;
    list->end->next = NULL;

    return result;
}

void SList_Merge(SList* list1, SList* list2, SortCompareFunc* compFunc, int verbose)
{
    assert(list1 != NULL);
    assert(list2 != NULL);
    assert(compFunc != NULL);

    SListNode* it1 = list1->begin;
    
    if (verbose)
    {
        fputs("Merge: got lists: ", stdout);
        SList_Print(list1);
        fputs(" and ", stdout);
        SList_PrintLn(list2);
    }

    while (list2->begin)
    {
        if (it1)
        {
            if (compFunc(it1->str, list2->begin->str))
            {
                SListNode* node = list2->begin;
                list2->begin = node->next;
                SListNode_InsertBefore(it1, node);

                if (node->next == list1->begin)
                    list1->begin = node;
            }
            else
                it1 = it1->next;
        }
        else
        {
            SListNode* node = list2->begin;
            list2->begin = node->next;
            SListNode_InsertAfter(list1->end, node);

            if (node->prev == list1->end)
                list1->end = node;
        }
    }

    if (verbose)
    {
        fputs("Merge: result lists: ", stdout);
        SList_PrintLn(list1);
    }
}

static size_t s_MergeSortRecursionDepth = 0;

static void MergeSort(SList* list, SortCompareFunc* compFunc, int verbose)
{
    assert(list != NULL);
    assert(compFunc != NULL);

    s_MergeSortRecursionDepth++;
    if (verbose)
    {
        printf("MergeSort(Depth: %lu): got list: ", s_MergeSortRecursionDepth);
        SList_PrintLn(list);
    }

    if (list->begin == list->end)
        return;
    else if (list->begin->next == list->end)
    {
        if (compFunc(list->begin->str, list->end->str))
        {
            list->begin->prev = list->end;
            list->begin->next = NULL;

            list->end->next = list->begin;
            list->end->prev = NULL;
            
            SListNode* temp = list->begin;
            list->begin = list->end;
            list->end = temp;
        }
    }
    else
    {
        SList partialList = Split(list);
        MergeSort(list, compFunc, verbose);
        MergeSort(&partialList, compFunc, verbose);

        SList_Merge(list, &partialList, compFunc, verbose);
    }

    if (verbose)
    {
        printf("MergeSort(Depth: %lu): result list: ", s_MergeSortRecursionDepth);
        SList_PrintLn(list);
    }
    s_MergeSortRecursionDepth--;
}

static int DefaultCompFunc(const char* restrict str1, const char* restrict str2)
{
    return strcmp(str1, str2) > 0;
}

void SList_Sort(SList* list, SortCompareFunc* compFunc, int verbose)
{
    if (!compFunc) compFunc = DefaultCompFunc;
    MergeSort(list, compFunc, verbose);
}

void SList_Delete(SList* list)
{
    while (list->begin != NULL)
        SList_PopFront(list);
}

int SList_IsSorted(const SList* list, SortCompareFunc* compFunc)
{
    if (!compFunc) compFunc = DefaultCompFunc;

    SListNode* node = list->begin;
    while (node && node->next)
    {
        if (compFunc(node->str, node->next->str)) return 0;
        node = node->next;
    }
    return 1;
}

int SList_Compare(const SList* list1, const SList* list2)
{
    SListNode* it1 = list1->begin;
    SListNode* it2 = list2->begin;

    while (it1 && it2)
    {
        if (strcmp(it1->str, it2->str))
            return 0;

        it1 = it1->next;
        it2 = it2->next;
    }

    if ((it1 && !it2) || (!it1 && it2))
        return 0;

    return 1;
}

void SList_Compress(SList* list)
{
    SListNode* node = list->begin;

    while (node && node->next)
    {
        if (!strcmp(node->str, node->next->str))
            SListNode_Delete(node->next);
        else
            node = node->next;
    }
}
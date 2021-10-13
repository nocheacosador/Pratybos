#include <assert.h>
#include <stdio.h>
#include "StringList.h"

void TestCase1()
{
    SList list1 = SList_Init();
    SList_EmplaceBack(&list1, "B");
    SList_EmplaceBack(&list1, "C");
    SList_EmplaceBack(&list1, "E");
    SList_EmplaceBack(&list1, "D");
    SList_EmplaceBack(&list1, "A");
    SList_EmplaceBack(&list1, "F");

    SList list2 = SList_Init();
    SList_EmplaceBack(&list2, "A");
    SList_EmplaceBack(&list2, "B");
    SList_EmplaceBack(&list2, "C");
    SList_EmplaceBack(&list2, "D");
    SList_EmplaceBack(&list2, "E");
    SList_EmplaceBack(&list2, "F");
    
    SList_Sort(&list1, NULL, 0);

    assert(SList_Compare(&list1, &list2));
}


int main()
{
    TestCase1();
}
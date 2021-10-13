#include <assert.h>
#include <stdio.h>
#include "StringList.h"

void TestCase1()
{
    SList list1 = SList_Init();
    SList_EmplaceBack(&list1, "text1");
    SList_EmplaceBack(&list1, "text2");
    SList_EmplaceBack(&list1, "text3");

    SList list2 = SList_Init();
    SList_EmplaceBack(&list2, "text1");
    SList_EmplaceBack(&list2, "text2");
    SList_EmplaceBack(&list2, "text3");

    assert(SList_Compare(&list1, &list2) == 1);
}

void TestCase2()
{
    SList list1 = SList_Init();
    SList_EmplaceBack(&list1, "text1");
    SList_EmplaceBack(&list1, "text2");
    SList_EmplaceBack(&list1, "text3");

    SList list2 = SList_Init();
    SList_EmplaceBack(&list1, "text1");
    SList_EmplaceBack(&list1, "text3");
    SList_EmplaceBack(&list1, "text2");

    assert(SList_Compare(&list1, &list2) == 0);
}

int main()
{
    TestCase1();
    TestCase2();
}
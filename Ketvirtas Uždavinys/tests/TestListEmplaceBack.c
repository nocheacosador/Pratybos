#include <assert.h>
#include <stdio.h>
#include "StringList.h"
#include <string.h>

void TestCase1()
{
    SList list = SList_Init();
    SList_EmplaceBack(&list, "text1");
    
    assert(list.begin);
    assert(list.end);
    assert(list.begin == list.end);
    assert(!strcmp(list.begin->str, "text1"));

    SList_EmplaceBack(&list, "text2");
    
    assert(list.begin->next == list.end);
    assert(!strcmp(list.begin->str, "text1"));
    assert(!strcmp(list.end->str, "text2"));
}

int main()
{
    TestCase1();
}
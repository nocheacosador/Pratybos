# __C Programavimo Stiliaus Gairės__

## __Turinys__
* Vardai
    * Failai
    * Kintamieji
    * Struktūros ir sąjungos
    * Enumeratoriai
    * Metodai
    * #define ir MACROS
    * Rodyklės
* Formatavimas
    * Sklaustai
    * Įtrauka

## __Vardai__
Visi vardai/pavadinimai turi būti informatyvūs, pageidautina sudaryti iš pilnų žodžių.

### __Failai__
Failų vardai turi būti išsamūs ir parašyti naudojant `PascalCase`.

### __Kintamieji__

#### __Globaliniai__
Globaliniai kintamieji turi būti pavadinti naudojant `PascalCase` su prieždėliu 
`g_`, o statiniai globaliniai kintamieji su prieždėliu `s_`, pvz. `g_GlobalVariable` `s_StaticVariable`. 

Globalinių konstantų pavadinimai turi būti sudaryti tik iš didžiųjų raidžių ir 
žodžius skiriant `_` simboliu. Pvz. `GLOBAL_CONSTANT`.

#### __Lokalūs__
Lokalių kintamūjų vardai turi būti sudaryti naudojant `camelCase`. Pvz. `localVariable`.

#### __Struktūros ir sąjungos__
Struktūrų (`struct`) ir sąjungų (`union`) pavadinimai turi būti sudaryti 
naudojant `PascalCase`, o jų narių pavadinimai - `camelCase`. Pvz.:

```C
struct Node 
{
    int data;
    struct Node* next;
};
```

#### __Enumeratoriai__
Enumeratoriaus tipas turi būti pavadintas naudojant `PascalCase`, o jo reikšmės 
naudojant didžiąsias raides ir `_` tarp žodžių.

```C
enum SomeEnumerator
{
    FIRST_VALUE, 
    SECOND_VALUE,
    THIRD_VALUE
};
```

#### __Metodai__
Metodai turi būti pavadinti nauojant `PascalCase`, o jų argumentai - `camelCase`.

```C
void PrintArray(const int* arr, int arrSize);
```

Jeigu metodas yra susietas su tam tikra struktūra ir yra naudojamas tik tos struktūros kontekste,
jo pavadinimas turėtų buti iš dviejų dalių: struktūros pavadinimas, metodo pavadinimas. Tos dalys
turi būti atskirtos `_` simboliu.

```C
struct Node 
{
    int data;
    struct Node* next;
};

void Node_InsertBefore(struct Node** node, int data);
```

#### __#define ir MACROS__
Visi #define ir MACROS turi būti parašyti naudojant vien didžiąsias raides. Jei 
MACRO yra kažkoks reiškinys, jis turi būtų apglaustas sklaustais. Jei reiškinys 
yra ilgesnis nei keleta eilučių - viduj `do { ... } while(0);`, o naujų eilučių 
simboliai turibūti sulygiuoti dešinėje pusėje.

```C
#define MAX(a,b) blah
#define IS_ERR(err) blah
#define	MACRO(v, w, x, y)       do {                \
                                    v = (x) + (y);  \
                                    w = (y) + 2;    \
                                } while (0);
```

#### __Rodyklės__
Rodyklių simbolis (`*`) turi būti prie tipo, ne prie pavadinimo.

```C
int* num;
const char* gendalfSaid = "You shall not pass!";
```
### __Fromatavimas__

#### __Skliaustai__
Rekomenduojama skliaustus dėti tokia tvarka:

```C
void PrintArray(const int* arr, int arrSize);
{
    int first = 1;
    for (int i = 0; i < arrSize; i++)
    {
        if (first)
        {
            printf("[%d", arr[i]);
            first = 0;
        }
        else
            printf(", %d", arr[i]);
    }

    puts("]\n");
}
```

> Skliaustai nėra būtini kai reiškinys užima tik vieną eilutę.

#### __Sklaiustai po raktinių žožių__
Tarp raktinių žožių ir skliaustų turi būti tarpas.

```C
while (exp) ...
if (exp) ...
switch (val) ...
```
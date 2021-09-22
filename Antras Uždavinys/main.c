/* *****************************************************
 *  Lukas Zajončkovskis
 *  lukas.zajonckovskis@mif.stud.vu.lt
 *  Studento pažymejmo Nr. 2110647
 *  Užduotis Nr. 2, 13 variantas
 *
 * 
 *  Užduotis:
 *    Įvesti sveiką skaičių N. Įvesti N sveikų skaičių a1, a2, ... aN. 
 *    Jeigu tarp jų visi skaičiai yra neigiami arba visi neneigiami - 
 *    išvesti pranešimą. Priešingu atveju apskaičiuoti reiškinį:
 *      S = x1 * y1 + x2 * y2 + ... + xp * yp
 *    kur xi - neigiami sekos nariai, einantys iš eilės,
 *    yi - neneigiami sekos nariai, einantys iš eilės,
 *    p = min(n, t), kur n - neigiamų sekos narių skaičius, 
 *    o t - neneigiamų sekos narių skaičius.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//#define MIN(a, b)	(((a) < (b)) ? (a) : (b))						// universal, works for double, float, int, char, etc.
																	// tho allows to compare different types with each other
																	// i.e. not typesafe

static inline int Min(const int a, const int b)						// this is typesafe, tho not universal
{
	return a < b ? a : b;
}

static inline int GetNumber(const char* prompt)
{
	int num = 0;
	char buf[32];

	while (true)
	{
#if defined(__GNUC__)
	#pragma GCC diagnostic push										// save diagnostic state
	#pragma GCC diagnostic ignored "-Wformat-security"				// disable -Wformat-security warning
#endif
		printf(prompt);												// causes -Wformat-security warning
#if defined(__GNUC__)
	#pragma GCC diagnostic pop										// restore diagnostic state
#endif

		if (!fgets(buf, 32, stdin))
			continue;

		num = atoi(buf);

		if (num == 0 & strcmp(buf, "0") != 0)
		{
			puts("It is not a whole number!");
		}
		else
			return num;
	}
}

static inline int GetNaturalNumber(const char* prompt)
{
	int  num = 0;
	char buf[32] = { '\0' };

	while (true)
	{
#if defined(__GNUC__)
	#pragma GCC diagnostic push										// save diagnostic state
	#pragma GCC diagnostic ignored "-Wformat-security"				// disable -Wformat-security warning
#endif
		printf(prompt);												// causes -Wformat-security warning
#if defined(__GNUC__)
	#pragma GCC diagnostic pop										// restore diagnostic state
#endif

		if (!fgets(buf, 32, stdin))
			continue;

		num = atoi(buf);

		if (num == 0 & strcmp(buf, "0") != 0)
		{
			puts("It is not a whole number!");
		}
		else if (num > 0)
			return num;
		else
			puts("It is not a natural number. Enter any number bigger that 0.");
	}
}

int main(int argc, char* argv[])
{
    int NumberCount = GetNaturalNumber("Enter number of digits you desire: ");
	int Arr[NumberCount];
	int NegativeNumberCount = 0;

	for (int n = 0; n < NumberCount; n++)
	{
		Arr[n] = GetNumber("Enter number: ");
		if (Arr[n] < 0)
			NegativeNumberCount++;
	}
	
	if (NegativeNumberCount == NumberCount)
	{
		puts("All numbers are negative!");
		exit(EXIT_FAILURE);
	}
	else if (NegativeNumberCount == 0)
	{
		puts("All numbers are positive!");
		exit(EXIT_FAILURE);
	}

	int p = Min(NumberCount - NegativeNumberCount, NegativeNumberCount);

	int PositiveNumberIndex = 0;
	int NegativeNumberIndex = 0;
	int Sum = 0;

	printf("Answer: ");

	for (int i = 0; i < p; i++)
	{
		while (Arr[PositiveNumberIndex] <  0) PositiveNumberIndex++;
		while (Arr[NegativeNumberIndex] >= 0) NegativeNumberIndex++;

		if (i == 0)	printf("%d * %d",    Arr[PositiveNumberIndex], Arr[NegativeNumberIndex]);
		else 		printf(" + %d * %d", Arr[PositiveNumberIndex], Arr[NegativeNumberIndex]);
		
		Sum += Arr[PositiveNumberIndex] * Arr[NegativeNumberIndex];

		PositiveNumberIndex++;
		NegativeNumberIndex++;
	}

	printf(" = %d\n", Sum);
}
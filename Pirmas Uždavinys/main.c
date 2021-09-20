/* *****************************************************
 *  Lukas Zajončkovskis
 *  lukas.zajonckovskis@mif.stud.vu.lt
 *  Studento pažymejmo Nr. 2110647
 *  Užduotis Nr. 1, 12 variantas
 *
 * 
 *  Užduotis:
 *    Įvedinėti sveikų skaičių seką iki pirmo skaičiaus, 
 *    mažesnio už -1000 arba didesnio už 1000. Nustatyti, 
 *    ar yra įvestoje sekoje trys iš eilės einantys 
 *    nuliniai nariai ir išvesti atitinkamą pranešimą.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static inline int getNumber(const char* prompt)
{
	int num = 0;

ask:
#if defined(__GNUC__)
	#pragma GCC diagnostic push									// save diagnostic state
	#pragma GCC diagnostic ignored "-Wformat-security"			// disable -Wformat-security warning
#endif
	printf(prompt);											// causes -Wformat-security warning
#if defined(__GNUC__)
	#pragma GCC diagnostic pop									// restore diagnostic state
#endif

	if (!scanf("%d", &num))
	{
		char ch;
		while ((ch = getchar()) != '\n' && ch != EOF) { ; }		// empty stdin buffer
		puts("It is not a whole number!");

		goto ask;
	}

	return num;
}

static inline bool shouldExit(int number)
{
	return (number < -1000 || number > 1000);
}

static inline bool isNullMember(int number)
{
	return (number == 0);
}

int main(int argc, char* argv[])
{
	unsigned char nullMemberInARowCount = 0;
	
	puts("Enter any whole numbers in range from -1000 to 1000.\n"
		 "Enter any number outside of this range to exit.\n");
	
	while (1)
	{
		int number = getNumber("Enter number: ");
		
		if (shouldExit(number))
		{
			puts("Bye bye!");
			exit(EXIT_SUCCESS);
			break;
		}

		if (isNullMember(number))
		{
			nullMemberInARowCount++;
			
			if (nullMemberInARowCount > 2)
				printf("%d null numbers in a row!\n", nullMemberInARowCount);
		}
		else
			nullMemberInARowCount = 0;
	}
}
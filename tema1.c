#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}
void reverse(char *first)
{
	char *last = first;

	if (!first) //daca sirul este null ies din functie 
		return;
	while(*last) 
		++last;    //parcurg sirul pana la caracterul NULL
	last--;        //scad 1 ca sa ajung la ultimul element diferit de NULL
	while(first < last) {
		char h = *first, t = *last;//imi salvez primul si ultimul caracter

		*first = t;   //le inlocuiesc in timp ce parcurg sirul
		*last = h;
		first++;		
		last--;			
	}
}
static char* unsignedInt_to_char(unsigned int n, char *str)
{
	int current = 0;	//variabila pentru a parcurge sirul

	if (n == 0) {
        str[0] = '0';	//daca numarul este 0 ii dau primului caracter valoare 0
        return str;		//intorc sirul
    }
	while (n != 0) {	//construiesc un sir cu numerele de la coada la cap
		str[current] = n % 10 + '0';//adaug '0' ca sa corespunda caracterul din ASCII
		current++;		//adaug 1 pentru a-i creste dimensiunea sirului
		n/=10;			//impart la 10 sa scap de ultima cifra
	}
	str[current] = '\0';//adaug terminatorul de sir
	reverse(str);		//inversez sirul
	return str;
}
static char* int_to_hexadec(unsigned int n, char *str)
{
	char* character = "0123456789abcdef";//sir in care stochez caracterele hexa
	int i = 0;							 

	if (n == 0) 
		return "0";//intorc sirul "0" daca numarul este 0
	if (n > 0) {
		while(n) {
			str[i] = character[n % 16];//convertesc in baza 16 luand
			n /= 16;				   //restul impartirii numarului la 16
			i++;					   //apoi impart la 16
		}
	} else {
		unsigned int num = n;//daca nr este mai mic decat 0 il fac de tip
							 //unsigned pentru a nu mai avea semn
		while(num) {
			str[i] = character[num%16];//procedez la fel ca la un nr pozitiv
			num /= 16;
			i++;
		}
	}
	str[i++] = '\0';//adaug terminatorul de sir
	reverse(str);	//inversez sirul
	return str;
}
static char* int_to_char(int n,char *str)
{
	int check = 0, current = 0, isNegative = 0;

	 if (n == 0) {
        str[current++] = 0;//intorc 0 daca numarul este 0
        return str;
    }
	if (n < 0) {
		n =  n * (-1);
		isNegative = 1;//verific daca numarul este negativ si il fac pozitiv
	}
	if(n == INT_MIN) {//daca este INT_MIN ii dau valoarea INT_MAX
		n = INT_MAX;  //pentru a putea parcurge numarul si a-l transforma in string
		check = 1;	  //variabila pt a sti ca este INT_MIN
		isNegative = 1;
	}
	while(n != 0) {
		str[current++] = n % 10 + '0';//adaug '0' pentru  
		n = n/10;					 //a corespunde cu caracterul ASCII
	}
	if(isNegative == 1) {
		str[current] = '-';//daca este negativ ii pun semnul -
		current++;
	}
	str[current] = '\0';//adaug terminatorul de sir
	reverse(str);		//inversez sirul
	if(check == 1) {
		str[strlen(str) - 1] += 1;//daca este INT_MIN adaug 1 la ultima cifra
	}							  //pentru a corespunde cu INT_MIN
	return str;
}
int iocla_printf(const char *format, ...)
{
	va_list args;			//initializarea listei de argumente
	va_start(args, format);	//incep parcurgerea argumentelor
	int number_of_chars = 0;//variabila pentru stocarea numarului de caractere afisate

	while(*format != 0) {   //parcurg sirul pana se ajunge
		if(*format == '%') {//verific daca am caracterul '%'
			format++;
			const char* save = format;//salvez pentru a escapa un caracter
			save++;
			if(*format == '%') {//escapez daca am doua '%' consecutive
				write_stdout(format, 1);//afisez '%'
				format++;				//continui parcurgerea sirului
				number_of_chars++;		//adaug 1 la nr de caractere afisat
				if(*save != '%') {		//verific sa am numai doua '%' consecutive
					write_stdout(save, 1);//printez caracterul escapat
					format++;			  
					number_of_chars++;
				}
			}
			if(*format == 'd' ) {
				int x = va_arg(args, int);//salvez variabila de tip int
				char buff[256];//buffer pentru int_to_char
				char *aux = int_to_char(x, buff);//salvez sirul transformat
				number_of_chars+=strlen(aux);//adaug lungimea stringului
				write_stdout(aux, strlen(aux));//afisez stringul
				format++;
			}
			if(*format == 'u') {//procedez la fel ca la tipul "int"
				unsigned int x = va_arg(args, unsigned int);
				char buff[256];
				char *new= unsignedInt_to_char(x, buff);
				write_stdout(new, strlen(new));
			 	number_of_chars+=strlen(new);
				format++;
			}
			if(*format == 'x') {//procedez la fel ca la tipul "int"
				int x = va_arg(args, int);
				char buff[256];
				char *new= int_to_hexadec(x, buff);
				write_stdout(new, strlen(new));
				number_of_chars+=strlen(new);
				format++;
			}
			if(*format == 'c') {
				int y = va_arg(args, int);//iau variabila de tip int
				char buff[1];			  
				buff[0] = (char) y;//castez int-ul la char ca sa-l afisez
				write_stdout(buff, 1);
				number_of_chars++;
				format++;
			}
			if(*format == 's') {
				char *x = va_arg(args, char*);//salvez stringul
				write_stdout(x, strlen(x));//afisez stringul
				number_of_chars += strlen(x);
				format++;
			}
		} else {//daca nu gaseste caracterul '%' il afiseaza
			write_stdout(format, 1);
			number_of_chars++;
			format++;	
		}
	}
	va_end(args);//termin de parcurs lista de parametrii
	return number_of_chars;//intorc numarul de caractere afisat
}

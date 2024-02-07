#include <stdio.h>
#include <math.h>
#include <float.h>


	//MINIMUM
int min (int a, int b) {
if (a<b && a > 0) return a;
else if (b > 0) return b;
else return a;
}
	//POROVNÁVÁNÍ DBL
int dbl (double a, double b) {
//printf ("A=%g, B=%g, A-B = %g\n", a, b, fabs(a-b));
if (fabs (a-b) <= DBL_EPSILON * (fabs (a) + fabs (b) ) ) return 1;
else return 0;
}
	//VÝPOČET OBECNÝ

int strana (double l,double p,double s) {
double strana = (p-l)/(l-s);
int down = floor (strana);
if (dbl (down, strana) ) return down;
else return ceil (strana);
}

int vypocet (double l1, double l2, double p1, double p2, double s) {
double strana1, strana2;

if (l1 > p1 || dbl(l1, p1) ) {
strana1 = 1;
strana2 = strana (l2, p2, s) + 1;
}
else if (l2 > p2 || dbl(l1, p1) ) {
strana2 = 1;
strana1 = strana (l1, p1, s) + 1;
}
else {
strana1 = strana (l1, p1, s) + 1;
strana2 = strana (l2, p2, s) + 1;
};

return strana1 * strana2;
}
	//HLAVNÍ PROGRAM
int main(void) {

double l1,l2,p1,p2,s;
int vysledek = 1;

	// NAČTENÍ VSTUPU PRO LÁTKU
printf("Velikost latky:\n");

if (scanf (" %lf %lf", &l1, &l2) != 2 || l1 <= 0 || l2 <= 0) printf("Nespravny vstup.\n");
else { // 1.ELSE

	//NAČTENÍ VSTUPU PRO PLACHTU
printf("Velikost plachty:\n");

if (scanf (" %lf %lf", &p1, &p2) != 2 || p1 <= 0 || p2 <= 0) printf("Nespravny vstup.\n");
else { // 2.ELSE

	//PODMÍNKA PŘEKRYVU
if (  ( (l1 > p1 || dbl(l1,p1)) && (l2 > p2 || dbl(l2,p2)) ) || ( (l2 > p1 || dbl(l2,p1)) && (l1 > p2 || dbl(l1,p2)) ) ) printf("Pocet kusu latky: %d\n", vysledek);
else { //3.ELSE
	//NAČTENÍ PŘEKRYVU
printf("Prekryv:\n");

if (scanf ("%lf", &s) != 1 || s < 0 ) printf("Nespravny vstup.\n");
else if (s >= l1 && s>= l2) printf("Nelze vyrobit.\n");

	// VÝPOČET
else {
vysledek = min ( vypocet (l1, l2, p1, p2, s), vypocet (l1, l2, p2, p1, s) );
printf("Pocet kusu latky: %d\n", vysledek);
//printf("PRVNÍ VÝPOČET: %d a DRUHÝ VÝPOČET: %d \n",vypocet (l1, l2, p1, p2, s), vypocet (l2,l1, p1, p2, s));
};
}; // 3.ELSE
}; // 2.ELSE
}; // 1.ELSE
return 0;
} // MAIN

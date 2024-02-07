#include <stdio.h>
#include <math.h>

int main (void) {
double x,y;
char a,b;

printf ("Zadejte vzorec:\n");
if (scanf ( "%lf %c %lf %c", &x, &a, &y, &b ) !=4 ||
(a != 42 &&
a != 43 &&
a != 45 &&
a != 47) ||
(a == 47 && y==0) ||
b != 61
) {
printf ("Nespravny vstup.\n");
//printf ("Hodnota scanf:%d",t );
}
else
{
if (a == 42) { /* Krát */
printf ("%g\n", x*y);

}
if (a == 43) {/* Plus */
printf ("%g\n", x+y);

}
if (a == 45) {/* Minus  */
printf ("%g\n", x-y);

}
if (a == 47) {/* Děleno */
double vysl = floor(x/y);
if (vysl < 0 && x/y !=vysl) {
vysl +=1;
printf ("%g",vysl);
}
else {
printf ("%g\n", floor(x/y));
}
}
};
return 0;
}

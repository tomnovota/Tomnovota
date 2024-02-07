#include <stdio.h>

int main(void) {
int x, y, z;
char zavorka;
printf("Zadejte barvu v RGB formatu:\n");

if (scanf (" rgb ( %d , %d , %d %c[] ", &x, &y, &z, &zavorka) != 4 ||
x < 0 || x > 255 ||
y < 0 || y > 255 ||
z < 0 || z > 255 ||
zavorka != 41

) {
printf("Nespravny vstup.\n");
}

else {

printf("#%.2X%.2X%.2X\n", x, y, z);
};
return 0;
}

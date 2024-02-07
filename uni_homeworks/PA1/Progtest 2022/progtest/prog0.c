#include <stdio.h>

int error ()
{
	printf("luj\n");
	return 0;
}

int res ( int a )
{
	switch (a)
	{
		case 1: 
			printf( "noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n" );
			break;
		case 2:  
			printf( "bortaS bIr jablu'DI' reH QaQqu' nay'.\n" );
			break;
		case 3:  
			printf( "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n" );
			break;
		case 4:  
			printf( "bISeH'eghlaH'be'chugh latlh Dara'laH'be'.\n" );
			break;
		case 5:  
			printf( "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n" );
			break;
		case 6:  
			printf( "Suvlu'taHvIS yapbe' HoS neH.\n" );
			break;
		case 7:  
			printf( "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n" );
			break;
		case 8:  
			printf( "Heghlu'meH QaQ jajvam.\n" );
			break;
		case 9:  
			printf( "leghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n" );
			break;
	}
	return 0;
}

int main (void)
{
	int a;
	char c;
	printf( "ml' nob:\n" );
	if (scanf( "%d %c", &a, &c) != 1
		|| a > 9 || a < 1)
		return error();
	printf( "Qapla'\n" );
	return res(a);
}

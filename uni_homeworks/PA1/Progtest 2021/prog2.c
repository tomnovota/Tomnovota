#include <stdio.h>

void rad (int pos, int sous, int * maxPozicePredchozichRadu, int * maxCisloPredchozihoRadu, int * pocetCislic) {
    int cisloRadu = 1;
    int obsPozice = sous -1;
    int obsPozicePred = 0;
    int maxCisloTed = sous -1;
    int maxCisloPred = 0;
    int tmp;
    while (pos > obsPozice) {
        cisloRadu += 1;
        maxCisloPred = maxCisloTed;
        maxCisloTed = (maxCisloPred + 1) * sous - 1;
        tmp = obsPozice; // max pozice predchoziho radu
        obsPozicePred = obsPozice;
        obsPozice = ((maxCisloTed - maxCisloPred) * cisloRadu ) + tmp;
    };
    * maxPozicePredchozichRadu = obsPozicePred;
    * maxCisloPredchozihoRadu = maxCisloPred;
    * pocetCislic = cisloRadu;
}

void hledat (int pos, int maxPozicePredchozichRadu, int pocetCislic, int maxCisloPredchozihoRadu) {
    int poziceHledane = pos - maxPozicePredchozichRadu;
    int kolikate = 1;
    while ( (kolikate*pocetCislic) < poziceHledane)
        kolikate +=1;
    if (pos > 9) {
        printf ("%d\n", maxCisloPredchozihoRadu + kolikate);
        int pocetMezer = pocetCislic - 1 - (kolikate*pocetCislic - poziceHledane);
        for (int i=0; i < pocetMezer; i++)
            printf(" ");
        printf("^\n");
    }
    else
        printf("%d\n^\n", pos);
}

int main (void) {
    printf("Pozice a soustava:\n");

    while ( ! feof ( stdin ) ) {
        int pos,sous;
        if (scanf ("%d %d", &pos, &sous) != 2
            || pos < 0
                || sous < 2
                    || sous > 36 ) {
            printf ("Nespravny vstup.\n");
            return 1;
        };
    int maxPozicePredchozichRadu, pocetCislic, maxCisloPredchozihoRadu;
    rad (pos, sous, &maxPozicePredchozichRadu, &maxCisloPredchozihoRadu, &pocetCislic);
    hledat (pos, maxPozicePredchozichRadu, pocetCislic, maxCisloPredchozihoRadu);
    };
    return 0;
}


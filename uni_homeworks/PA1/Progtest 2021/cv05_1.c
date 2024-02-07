#include <stdio.h>

int main (void) {
    int a,b;
    printf("Zadejte pocet poli:\n");
    if (scanf("%d",&a) != 1 || a < 1) {
        printf("Nespravny vstup.\n");
        return 1;
    };
    printf("Zadejte velikost pole:\n");
    if (scanf("%d",&b) != 1 || b < 1) {
        printf("Nespravny vstup.\n");
        return 1;
    };
    for (int i=0;i<(a+2);i++) {
        if (i==0 || i==(a+1)) {
            printf("+");
            for (int j=0;j<a*b;j++)
                printf("-");
            printf("+");
        }
        else {
            if ((i%2)!=0) {
                for (int k=0;k<b;k++) {
                    printf("|");
                    for (int j=0;j<a;j++) {
                        if ((j%2)==0) {
                            for (int k=0;k<b;k++)
                                printf(" ");
                        }
                        else {
                            for (int k=0;k<b;k++)
                                printf("X");
                        }
                    }
                    printf("|");
                    if (k<b-1)
                        printf("\n");
                }
            }
            else {
                for (int k=0;k<b;k++) {
                    printf("|");
                    for (int j=0;j<a;j++) {
                        if ((j%2)==0) {
                            for (int k=0;k<b;k++)
                                printf("X");
                        }
                        else {
                            for (int k=0;k<b;k++)
                                printf(" ");
                        }
                    }
                    printf("|");
                    if (k<b-1)
                        printf("\n");
                }
            }
        }
        printf("\n");
    };
    return 0;
}


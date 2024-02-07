#include <cstdio>
#include <iostream>
#include <fstream>
#include "struct.h"


void scan_data ( data & src )
{
    std::cout << "Zadej " << n << " čísel:" << std::endl;
    for ( int i = 0; i < n; i ++ )
    {
        int tmp;
        std::cin >> tmp;
        src . arr [i] = tmp;
    }
}

void print_data ( const data & src )
{
    std::cout << "Data: ";
    for ( int i = 0; i < n; i ++ )
        std::cout << src . arr [i];
    std::cout << std::endl;
}

bool save_data ( const char * fileName, const data & src )
{
    std::ofstream file;
    file . open ( fileName, std::ios::binary );
    if ( ! file . is_open () )
    {
        std::cout << "Chyba při otevření souboru" << std::endl;
        file . close ();
        return false;
    }

    file . write ( (char *) src . arr, sizeof (src . arr) );

    file << std::endl;
    file . close ();

    return true;
}

bool read_data ( const char * srcName, data & dest )
{
    std::ifstream file;
    file . open ( srcName, std::ios::binary );
    if ( ! file . is_open () )
    {
        std::cout << "Chyba při otevření souboru" << std::endl;
        file . close ();
        return false;
    }

    file . read ( (char *) dest . arr, sizeof (dest . arr) );

    file . close ();
    return true;
}

bool is_equal ( const data & l, const data & r )
{
    for ( int i = 0; i < n; i ++ )
        if ( l . arr [i] != r . arr [i] )
            return false;
    return true;
}

int cmpInt ( const void * a, const void * b )
{
    const int * x = (int *) a;
    const int * y = (int *) b;
    return ( *y < *x ) - ( *x < * y );
}

void sort_data ( data & src )
{
    std::qsort ( src . arr, n, sizeof (int), cmpInt );
}
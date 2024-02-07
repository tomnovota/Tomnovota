#ifndef STRUCT_H
#define STRUCT_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>

const int n = 5;

struct data
{
    int arr [n];
};

void scan_data( data & src );
void print_data( const data & src );
bool save_data( const char * fileName, const data & src );
bool read_data( const char * srcName, data & dest );
bool is_equal( const data & l, const data & r );
void sort_data( data & src );

#endif
#ifndef CART_H
#define CART_H

#include "tnode.h"

using namespace std;

extern vector<string> features;
extern vector<vector<string>> values;
extern vector<vector<string>> rows;
extern bool verbose;

TNode *cart();

#endif // CART_H

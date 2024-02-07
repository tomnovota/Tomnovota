#ifndef EVAL_H
#define EVAL_H

#include "tfact.h"
#include "trule.h"
#include <algorithm>
#include <deque>

using namespace std;

void loadRule( vector<TFact*> &facts, vector<TRule*> &rules, const char *rule);
void solve( vector<TFact*> &facts, vector<TRule*> &rules, vector<string> atoms);

#endif // EVAL_H

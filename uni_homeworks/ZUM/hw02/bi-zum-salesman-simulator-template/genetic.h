//-------------------------------------------------------------------
//
//  Template for solving the Travelling Salesman Problem
//  (c) 2021 Ladislava Smítková Janků <ladislava.janku@fit.cvut.cz>
//
//-------------------------------------------------------------------

#ifndef GENETIC_H
#define GENETIC_H

#include "tmatrix.h"
#include <vector>

typedef enum { CROSSOVER_METHOD_OX, CROSSOVER_METHOD_PMX } TCrossoverMethod;

typedef struct {
    std::vector<int> path;
    long long fitness;
    double Pr;
    double q;
} TIndividual;

extern uint POPULATION;
extern double PROBABILITY_CROSSOVER;
extern double PROBABILITY_MUT_STEP;
extern std::vector<TIndividual> individuals;

void salesmanProblemGeneticInit( TMatrix *matrix);
void salesmanProblemGeneticStep( TMatrix *matrix, TCrossoverMethod crossoverMethod);

#endif // GENETIC_H

//---------------------------------------------------------------------------
//
//  Template for solving the Travelling Salesman Problem
//  (c) 2021 Ladislava Smítková Janků <ladislava.smitkova@fit.cvut.cz>
//
//  genetic.cpp: Implementation of genetic algorithm with these parameters:
//
//  Population:        500 individuals (can be modified by POPULATION constant)
//  Generations:       30 (can be modified by GENERATIONS constant)
//  Crossover method:  OX or PMX
//  Mutation method:   reversion of the path
//
//  Crossover probability:    95%  (PROBABILITY_CROSSOVER)
//  Mutation probability:     stepped by 5%  (PROBABILITY_MUT_STEP)
//
//  If the fitness value of the actual generation is better than last one,
//  mutation probability will be set to zero. In other case, mutation
//  probability will be increased by PROBABILITY_MUT_STEP.
//
//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <algorithm>

#include "genetic.h"
#include "path.h"

#define POPULATION              100
#define PROBABILITY_CROSSOVER   0.95
#define PROBABILITY_MUT_STEP    0.05
#define GENERATIONS             200

typedef struct {
    std::vector<int> path;
    double fitness;
    double Pr;
    double q;
} TIndividual;

std::vector<TIndividual> individuals;

bool compareByFitness( const TIndividual &a, const TIndividual &b)
{
    return a.fitness > b.fitness;
}

double computeFitness( const std::vector<int> &path, TMatrix *matrix)
{
    // Fitness Function (also known as the Evaluation Function) evaluates how close
    // a given solution is to the optimum solution of the desired problem.
    // It determines how fit a solution is.
    
    //****************************************************************
    // ZDE IMPLEMENTUJTE FITNESS
    return 0;
    //****************************************************************
}

void recalculate( TMatrix *matrix)
{
    // calculate fitness and Pr
    double sum_fitness = 0;
    for (auto ind = individuals.begin(); ind != individuals.end(); ++ind) {
        ind->fitness = computeFitness( ind->path, matrix);
        sum_fitness += ind->fitness;
    }

    // sort population by fitness
    std::sort( individuals.begin(), individuals.end(), compareByFitness);

    // compute Pr_i and q_i
    double q = 0;
    for (auto ind = individuals.begin(); ind != individuals.end(); ++ind) {
        ind->Pr = ind->fitness / sum_fitness;
        q += ind->Pr;
        ind->q = q;
    }
}

void selection()
{
    std::vector<TIndividual> newGeneration;

    for (auto i=0; i<POPULATION; i++)
    {
		//****************************************************************
		// ZDE IMPLEMENTUJTE SELEKCI (tvorba nove generace)
		//
        // newGeneration.push_back( ... );
		//****************************************************************
    }

    // new generation was borned
    individuals = newGeneration;
}

bool pathContainsCity( const std::vector<int> &path, int city)
{
    for (auto x = path.begin(); x != path.end(); ++x) {
        if (city == *x) return true;
    }
    return false;
}

void doCrossoverOX( std::vector<TIndividual> &result, TMatrix *matrix, TIndividual &a, TIndividual &b)
{
    TIndividual aa, bb;

    aa = a;
    bb = b;

	//****************************************************************
	// ZDE IMPLEMENTUJTE OPERATOR KRIZENI OX
	//****************************************************************
	
    // propagate only childs
    result.push_back( aa);
    result.push_back( bb);
}

void doCrossoverPMX( std::vector<TIndividual> &result, TMatrix *matrix, TIndividual &a, TIndividual &b)
{
    TIndividual aa, bb;

    aa = a;
    bb = b;

	//****************************************************************
	// ZDE IMPLEMENTUJTE OPERATOR KRIZENI PMX
	//****************************************************************

    // propagate only childs
    result.push_back( aa);
    result.push_back( bb);
}

void crossover( TMatrix *matrix, TCrossoverMethod crossoverMethod)
{
    std::vector<TIndividual> crossovered;
    std::vector<TIndividual>::iterator candidate = individuals.end();

    for (auto ind = individuals.begin(); ind != individuals.end(); ++ind) {
        // select candidates to the crossover process
        if (drand48() <= PROBABILITY_CROSSOVER) {
            if (candidate == individuals.end()) {
                // this is the first parent
                candidate = ind;
            }
            else {
                // now we have both parents, we can do crossover
                if (crossoverMethod == CROSSOVER_METHOD_PMX)
                    doCrossoverPMX( crossovered, matrix, *ind, *candidate);
                else
                    doCrossoverOX( crossovered, matrix, *ind, *candidate);

                candidate = individuals.end();
            }
        }
        else
            crossovered.push_back( *ind);
    }

    // If we got odd number of parents, do nothing with this candidate and push it directly
    // into the new generation.
    if (candidate != individuals.end())
        crossovered.push_back( *candidate);

    // crossover is done
    individuals = crossovered;
}

void mutation( double probability)
{
    for (auto ind = individuals.begin(); ind != individuals.end(); ++ind) {
        if (drand48() <= probability) {
            
			//****************************************************************
			// ZDE IMPLEMENTUJTE OPERATOR MUTACE
			//****************************************************************
            
        }
    }
}

void printState( int generation)
{
    printf("[%d]  %lld\n", generation, -individuals.at(0).fitness);
}

std::vector<int> salesmanProblemGenetic(TMatrix *matrix, TCrossoverMethod crossoverMethod)
{
    unsigned i, j;
    int x;
    std::vector<int>::iterator p;
    double mutation_probability = 0;
    double lastFitness = -std::numeric_limits<double>::max();
    std::vector<int> best;
    double bestFitness = -std::numeric_limits<double>::max();

    // initialization of random number generator
    srand( getpid());

    // born first population
    for (i=0; i<POPULATION; i++) {
        TIndividual ind;

        // Generate some random path: Place city indexes to the vector in some random order.
        // At index 0 will be city we start from.
        ind.path.clear();
        ind.path.push_back(0);
        j=1;
        while (j < matrix->getNumberOfTargets()) {
            x = random() % matrix->getNumberOfTargets();
            p = find( ind.path.begin(), ind.path.end(), x);
            if (p == ind.path.end()) {
                ind.path.push_back(x);
                j++;
            }
        }

        // Store this path into table of individuals.
        // Fitness and other parameters will be computaed later.
        individuals.push_back( ind);
    }

    // compute fitnesses and sort individuals
    recalculate( matrix);
    printState(0);

    // remember the best solution
    best = individuals.at(0).path;
    bestFitness = individuals.at(0).fitness;

    // run simulation
    for (i=1; i<GENERATIONS; i++) {
        // selection: select individuals for a new generation
        selection();

        // crossover
        crossover( matrix, crossoverMethod);

        // mutation
        if (mutation_probability > 0) mutation( mutation_probability);

        // print the best result
        recalculate( matrix);
        printState(i);

        // if fitness < lastFitness, increase mutation probability by one step
        if (individuals.at(0).fitness < lastFitness)
            mutation_probability += PROBABILITY_MUT_STEP;
        else
            mutation_probability = 0;

        lastFitness = individuals.at(0).fitness;

        if (lastFitness > bestFitness) {
            best = individuals.at(0).path;
            bestFitness = individuals.at(0).fitness;
        }
    }

    return best;
}

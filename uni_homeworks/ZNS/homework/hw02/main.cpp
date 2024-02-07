//-------------------------------------------------------------------------
//
//  BI-ZNS: Šablona pro úlohu č.1 - Inferenční systém s dopředným řetězením
//  (c) 2021 Ladislava Smítková Janků <ladislava.smitkova@fit.cvut.cz>
//
//  Verze šablony 20211008
//
//-------------------------------------------------------------------------

#include "texception.h"
#include "tfact.h"
#include "eval.h"
#include <iostream>
#include <fstream>
#include <regex>

vector<TFact*> facts;
vector<TRule*> rules;
vector<string> atoms;

void printFacts()
{
//    printf("[facts]\n");
    for (auto f = facts.begin(); f != facts.end(); ++f) {
        (*f)->print();
        printf("\n");
    }
}

void printRules()
{
    printf("[rules]\n");
    for (auto r = rules.begin(); r != rules.end(); ++r) {
        (*r)->print();
        printf("\n");
    }
    printf("\n");
}

void printAtoms()
{
    printf("[atoms]\n");
    for (auto a = atoms.begin(); a != atoms.end(); ++a) {
        printf("%s\n", (*a).c_str());
    }
    printf("\n");
}

string ltrim( const string &s)
{
    return regex_replace( s, regex("^\\s+"), string(""));
}

string rtrim( const string &s)
{
    return regex_replace( s, regex("\\s+$"), string(""));
}

string trim( const string &s)
{
    return ltrim( rtrim(s));
}

void clean()
{
    for (auto f = facts.begin(); f != facts.end(); f++) delete (*f);
    for (auto r = rules.begin(); r != rules.end(); r++) delete (*r);
    facts.clear();
    rules.clear();
    atoms.clear();
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: ./predikaty <rules.txt>\n");
        return 1;
    }
    try {
        string line;
        string filename = string( argv[1]);
        ifstream input_file( filename);
        if (!input_file.is_open())
            throw TException("Could not open the file '" + filename + "'");

        while (getline( input_file, line)) {
            line = trim( line);
            if (line != "") loadRule( facts, rules, line.c_str());
        }

        input_file.close();
        atoms = createListOfAtoms( facts);
/*
        printFacts();
        printRules();
        printAtoms();
*/
        solve( facts, rules, atoms);

        printFacts();

    }  catch (TException e) {
        fprintf( stderr, "Exception: %s\n", e.what());
    }

    clean();
}

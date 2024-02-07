//-------------------------------------------------------------------------
//
//  BI-ZNS: Šablona pro úlohu č.4 - CART
//  (c) 2021 Ladislava Smítková Janků <ladislava.smitkova@fit.cvut.cz>
//
//-------------------------------------------------------------------------

#include <stdio.h>
#include <fstream>
#include <cmath>
#include <regex>
#include "cart.h"

using namespace std;

vector<string> split(string str, string token)
{
    vector<string> result;
    while (str.size()) {
        int index = str.find(token);
        if (index != (int) string::npos){
            result.push_back( str.substr( 0, index));
            str = str.substr( index + token.size());
            if (str.size() == 0) result.push_back(str);
        }
        else {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

char asciitolower( char in)
{
    if (in <= 'Z' && in >= 'A') return in - ('Z' - 'z');
    return in;
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
    return ltrim(rtrim(s));
}

void loadData( const char *fileName)
{
    if (verbose) printf("Nahrávám data...\n");

    ifstream inputFile( fileName);
    if(!inputFile.is_open()) {
        throw (string) "Vstupní soubor nelze přečíst.";
    }
    string line;

    if (!getline( inputFile, line))
        throw string("Vstupní soubor nic neobsahuje.");

    features = split( line, ",");
    if (features.size() <= 1)
        throw string("Ve vstupním CSV souboru je pouze jeden sloupec.");

    for (unsigned long i=0; i<features.size()-1; i++)
        values.push_back( vector<string>());

    rows.clear();

    while (std::getline( inputFile, line))
    {
        vector<string> row = split( line, ",");

        string decision = trim( row[ row.size() - 1]);
        transform( decision.begin(), decision.end(), decision.begin(), asciitolower);

        if ((decision == "1") || (decision == "true") || (decision == "ano"))
            row[ row.size() - 1] = "1";
        else
            if ((decision == "0") || (decision == "false") || (decision == "ne"))
                row[ row.size() - 1] = "0";
            else
                throw string("Rozhodnutí nelze dekódovat, musí být 'ano' nebo 'ne'.");

        if (row.size() != features.size())
            throw string("Počet sloupců v hlavičce není stejný jako počet sloupců na řádku: " + line);

        for (unsigned long c=0; c<row.size()-1; c++) {
            if (find( values[c].begin(), values[c].end(), row[c]) == values[c].end()) {
                values[c].push_back( row[c]);
            }
        }
        rows.push_back( row);
    }
    inputFile.close();
}

void printTrainingData()
{
    for (auto i : features) {
        printf("%-15s ", i.c_str());
    }
    string hline( features.size() * 16, '-');
    printf("\n%s\n", hline.c_str());
    for (auto r : rows) {
        for (auto c : r) {
            printf("%-15s ", c.c_str());
        }
        printf("\n");
    }
    printf("\n");

    for (unsigned long c=0; c<values.size(); c++) {
        printf("%-15s ", string( features[c] + ":").c_str());
        for (auto v : values[c]) {
            printf("%-15s ", v.c_str());
        }
        printf("\n");
    }
    printf("\n");
}

void printDecisionTree( TNode *node)
{
    if (verbose) printf("\nDecision tree:\n\n");
    printf("%s", node->toString().c_str());
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Použití: ./CART [-q] <data.csv>\n\n"
               "  -q              tichý režim\n"
               "  <data.csv>      CSV soubor s trénovacími daty.\n\n");
        exit(1);
    }

    const char *fileName;
    if (argc < 3) fileName = argv[1];
    else {
        if (argc != 3) {
            printf("Chybné parametry.\n");
            exit(1);
        }
        if (strcmp( argv[1], "-q") == 0) {
            verbose = false;
        }
        else {
            printf("Neznámý přepínač: %s\n", argv[1]);
            exit(1);
        }
        fileName = argv[2];
    }

    try {
        loadData( fileName);
        if (verbose) printTrainingData();
        TNode *rootNode = cart();
        printDecisionTree( rootNode);
    }
    catch (std::string msg) {
        fprintf( stderr, "Exception: %s\n", msg.c_str());
        exit(1);
    }
    return 0;
}

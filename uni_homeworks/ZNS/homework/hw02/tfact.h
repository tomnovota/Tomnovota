#ifndef TFACT_H
#define TFACT_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class TFact
{
public:
    TFact( const char *fact);
    TFact( const char **fact);
    TFact( string id_, const char **factArguments);
    TFact( string id_, vector<string> &arguments_);
    ~TFact();
    string getName();
    int getNumberOfArguments();
    vector<string> & getArguments();
    TFact *translate( map<string, string> &dictionary);
    bool compare( TFact *fact);
    void print();

private:
    string id;
    vector<string> arguments;
    void load( const char **fact);
};

#endif // TFACT_H

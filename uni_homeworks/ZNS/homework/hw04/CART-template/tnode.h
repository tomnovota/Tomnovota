#ifndef TNODE_H
#define TNODE_H

#include <string>
#include <vector>
using namespace std;

class TNode
{
public:
    TNode();
    virtual ~TNode();
    virtual string toString( string prefix = "") = 0;
};

class TParent : public TNode
{
public:
    TParent( int featureIndex_, TNode *left_, TNode *right_);
    virtual ~TParent();
    virtual string toString( string prefix = "") override;
private:
    int featureIndex;
    TNode* left;
    TNode* right;
};

class TLeaf : public TNode
{
public:
    TLeaf( bool decision_);
    virtual string toString( string prefix = "") override;
private:
    bool decision;
};

#endif // TNODE_H

//-------------------------------------------------------------------------
//
//  BI-ZNS: Šablona pro úlohu č.4 - CART
//  (c) 2021 Ladislava Smítková Janků <ladislava.smitkova@fit.cvut.cz>
//
//-------------------------------------------------------------------------

#include "tnode.h"

extern vector<string> features;
extern vector<vector<string>> values;

TNode::TNode()
{
}

TNode::~TNode()
{
}

TParent::TParent( int featureIndex_, TNode *left_, TNode *right_)
{
    featureIndex = featureIndex_;
    left = left_;
    right = right_;
}

TParent::~TParent()
{
    delete left;
    delete right;
}

string TParent::toString( string prefix)
{
    string result;

    result += prefix;
    result += "if ";
    result += features[featureIndex];
    result += "=";
    result += values[featureIndex][0];
    result += " then\n";
    result += left->toString( prefix + "    ");
    result += prefix;
    result += "else\n";
    result += right->toString( prefix + "    ");
    return result;
}

TLeaf::TLeaf( bool decision_)
{
    decision = decision_;
}

string TLeaf::toString( string prefix)
{
    return prefix + (decision ? "yes\n" : "no\n");
}

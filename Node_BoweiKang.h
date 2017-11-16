
//  Name: Bowei Kang
//  Course: 16:332:503 , PM for Financial Applications and Numerical Computing
//  Final Project: Account Management System
//  Header file of class Node
//  2015/11/15

#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

class Node {
    friend class StockAccount;

public:
    Node(string & name, int share)  // constructor
    :stockSymbol (name), stockShare (share)
    {
        this->next = NULL;
        this->prev = NULL;
    }

private:
    string stockSymbol; // stock symbol
    int stockShare; // number of shares
    Node * next; // next pointer
    Node * prev; // previous pointer
};

#endif





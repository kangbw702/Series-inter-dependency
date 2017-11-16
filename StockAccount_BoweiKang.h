
//  Name: Bowei Kang
//  Course: 16:332:503 , PM for Financial Applications and Numerical Computing
//  Final Project: Account Management System
//  Header file of class StockAccount
//  2015/11/15

#ifndef STOCKACCOUNT_H
#define STOCKACCOUNT_H

#include <string>
#include <map>
#include "Node_BoweiKang.h"
#include "Account_BoweiKang.h"
using namespace std;

class StockAccount : public Account {

public:
    StockAccount();   // constructor
    ~StockAccount();  // destructor
    map <string, double> readStockPrice(string); // read stock price from file
    void setStockPrice (); // set price1 and price2
    map <string, double> randomSelectPrice(); // randomly select stock price
    void readPortfolio(); // read portfolio infomation from file
    void addToEnd(Node *); // insert node at end
    double calculateTotalValue (); // calculate total portfolio value (stock and cash)
    void printPrice(string &); // print current price of specific stock
    virtual void printPortfolio(); // print current portfolio
    virtual void printTransHistory(); // print transaction history
    void printTotalValue(); // print total value (should be replaced by Matlab plot)
    void buyStock(Node *, double); // buy stock
    void sellStock(Node *, double); // sell stock
    void appTransStock(bool, Node *); // append transaction to file (0-buy, 1-sell)
    void appTransCash(int, double); // append transaction history to file (0-buy, 1-sell)
    void updatePortfolio(); // update portfolio after transaction
    void updateTotalValue(); // update total portfolio value after transaction
    void swap(Node *, Node *); // utility function for sorting
    Node * partition(Node *, Node *); // partition for quick sorting
    void quickSort(Node *, Node *); // quick sort

private:
    Node *myHead;  // first pointer
    Node *myTail;  // last pointer
    map <string, double> currentPrice; // stock symbol and corresponding current price
    map <string, double> price1; // stock price 1
    map <string, double> price2; // stock price 2

};

#endif

//  Name: Bowei Kang
//  Course: 16:332:503 , PM for Financial Applications and Numerical Computing
//  Final Project: Account Management System
//  Header file of class Account
//  2015/11/15

#ifndef ACCOUNT_H
#define ACCOUNT_H

class Account {
    
public:
    Account(); // constructor
    void readBalance(); // read current cash balance from file
    double getBalance(); // get balance
    void setBalance(double); // set balance
    void updateBalance(); // update balance after transaction
    virtual void printTransHistory() = 0; // print transaction history
    virtual void printPortfolio() = 0; // print current portfolio and balance
    
private:
    double balance; // current cash balance
    
};

#endif

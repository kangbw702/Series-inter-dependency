
//  Name: Bowei Kang
//  Course: 16:332:503 , PM for Financial Applications and Numerical Computing
//  Final Project: Account Management System
//  Header file of class BankAccount
//  2015/11/15

#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "Account_BoweiKang.h"


class BankAccount : public Account {
    
public:
    BankAccount();
    virtual void printPortfolio(); // print out current cash balance
    void deposit(int); // deposit to bank account
    void withdraw(int); // withdraw from bank account
    virtual void printTransHistory(); // print out deposit/withdraw history
    void appTransCash(int, double); // append transaction history to file (2-deposit, 3-withdraw)
	void updateTotalValue(bool, double); // update total portfolio value to file

private:
    //double balance; // cash balance
    
};

#endif
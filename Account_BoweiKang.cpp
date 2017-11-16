
//  Name: Bowei Kang
//  Course: 16:332:503 , PM for Financial Applications and Numerical Computing
//  Final Project: Account Management System
//  Source code for class Account
//  2015/11/15

#include <iostream>
#include <fstream>
#include "Account_BoweiKang.h"  
using namespace std;


// constructor
Account::Account() {
    readBalance();
} // end constructor


// set balance
void Account::setBalance(double valuein) {
    balance = (valuein >=0) ? valuein : 0;
} // end setBalance


// get balance
double Account::getBalance() {
    return balance;
} // end getBalance


// read balance from file
void Account::readBalance() {
    
    double balancein;
    ifstream myfilein("balance.txt");
    
    // newly build balance.txt and set balance to be initial value
    if(! myfilein) {
        ofstream myfileout("balance.txt");
        double initial_balance = 10000.00;
        setBalance(initial_balance); // initial cash balance is 10 thousands
        myfileout << getBalance(); // write intial balance into file
        myfileout.close();
    } // end if
    
    // read balance from txt
    else while (myfilein >> balancein)
        setBalance(balancein);
    
    myfilein.close();
    
} // end readBalance


// update balance file after transaction
void Account::updateBalance() {
    
    ofstream myfileout("balance.txt");
    
    if(! myfileout) {
        cerr << "File could not be opened 4" << endl;
        exit( 1 );
    } // end if
    
    myfileout << getBalance();
    myfileout.close();
    
} // end updatePortfolio








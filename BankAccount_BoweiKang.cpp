
//  Name: Bowei Kang
//  Course: 16:332:503 , PM for Financial Applications and Numerical Computing
//  Final Project: Account Management System
//  Source code of class BankAccount
//  2015/11/15

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string>
#include "BankAccount_BoweiKang.h"  
using namespace std;

// constructor
BankAccount::BankAccount()
: Account()
{ } // end constructor


// print out current cash balance
void BankAccount::printPortfolio() {
    
    readBalance();
    cout << "You have $" << getBalance() << " in your bank account." << endl;
    
} // end printPortfolio

// deposit
void BankAccount::deposit(int valuein) {
    
    readBalance();
    setBalance(getBalance() + valuein);
    updateBalance(); // update cash balance
    appTransCash(2, valuein); // update transaction file
	updateTotalValue(0, valuein);
    cout << "You have deposited " << valuein << " into your cash account." << endl;
    
} // end deposit


// withdraw
void BankAccount::withdraw(int valuein) {
    
    readBalance();
    if (valuein > getBalance())
        cout << "You donot have enough moneyto withdraw." << endl;
    else {
        setBalance(getBalance() - valuein);
        updateBalance(); // update cash balance
        appTransCash(3, valuein); // update transaction file
		updateTotalValue(1, valuein);
        cout << "You have withdrew " << valuein << " from your cash account." << endl;
    } // end else
    
} // end deposit

 
// appendding transaction information to file (2-deposit, 3-withdraw)
void BankAccount::appTransCash(int event, double money) {
    
    ofstream transcFile("bank_transaction_history.txt", ios::app);
    
    if(! transcFile) {
        cerr << "File could not be opened 7" << endl;
        exit( 1 );
    } // end if
    
    time_t now = time(0);
    tm newtime;
    localtime_r(&now, &newtime);
    //localtime_s(&newtime, &now);
        
    transcFile << event << '\t'
    << money << '\t'
    << getBalance() << '\t'
    << newtime.tm_mon + 1 << "/" << newtime.tm_mday << "/" << newtime.tm_year + 1900 << " "
    << newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec << '\n';
    // Mmm-dd-yyyy hh:mm:ss
    
    transcFile.close();
    
} // end appTransCash

// print transaction history
void BankAccount::printTransHistory() {
    
    int event_code;
    string event;
    string date1;
    double money;
    double value;
    
    ifstream myfilein("bank_transaction_history.txt");
    
    if(! myfilein) {
        ofstream myfileout("bank_transaction_history.txt");
        cout << "newly build a txt 7." << endl;
        myfileout.close();
        cout << "You have no transactions up to now." << endl;
    } // end if
    
    else {
        // print header
        cout << setw(10) << left << "Event" << setw(8) << right << "Amount"
        << setw(10) << right << "Balance" << setw(8) << right << "Time" << endl;
        
        while (myfilein.peek() != EOF) {
            myfilein >> event_code >> money >> value;
            getline(myfilein, date1);
            
            if (event_code == 0) event = "Buy";
            if (event_code == 1) event = "Sell";
            if (event_code == 2) event = "Deposit";
            if (event_code == 3) event = "Withdraw";

            cout << setw(10) << left << event
            << fixed << setprecision(2) << setw(8) << right << money << setw(10) << right << value
            << setw(22) << left << date1 << endl;
        } // end while
        
    } // end else
    
    myfilein.close();
    
} // end printHistory

  // update Ttotal portfolio value to file
void BankAccount::updateTotalValue(bool event, double money) {

	string date1;
    double value1 = 0.0;
    double value2 = 0.0;

	time_t now = time(0);
	tm newtime;
    localtime_r(&now, &newtime);
    //localtime_s(&newtime, &now);
	
	ifstream myfilein("total_value.txt");

	if (!myfilein) {
		ofstream myfileout1("total_value.txt"); 
		double  initial_value = 10000.00;
		myfileout1 << initial_value << '\t'
		<< newtime.tm_mon + 1 << "/" << newtime.tm_mday << "/" << newtime.tm_year + 1900 << " "
		<< newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec << '\n';
		// Mmm-dd-yyyy hh:mm:ss
		myfileout1.close();
	} // end if

	else {
		while (myfilein.peek() != EOF) {
			myfilein >> value1;
			getline(myfilein, date1);
		} // end while
	} // end else

	ofstream myfileout2("total_value.txt", ios::app);
	if (event == 0)
		value2 = value1 + money;
	else
		value2 = value1 - money;

	myfileout2 << value2 << '\t'
		<< newtime.tm_mon + 1 << "/" << newtime.tm_mday << "/" << newtime.tm_year + 1900 << " "
		<< newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec << '\n';
	// Mmm-dd-yyyy hh:mm:ss

	myfileout2.close();

} // end updateTotalValue






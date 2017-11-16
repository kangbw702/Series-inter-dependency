
//  Name: Bowei Kang
//  Course: 16:332:503 , PM for Financial Applications and Numerical Computing
//  Final Project: Account Management System
//  Main implementation
//  2015/11/15

//#pragma comment( lib, "libmx.lib" )
//#pragma comment( lib, "libmex.lib" )
//#pragma comment( lib, "libeng.lib" )

//#include <engine.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include "StockAccount_BoweiKang.h"
#include "BankAccount_BoweiKang.h"
using namespace std;

void menuMain();
void menuStock();
void menuBank();
void operationMain(StockAccount &, BankAccount &);
void operationStock(StockAccount &, BankAccount &);
void operationBank(StockAccount &, BankAccount &);

int main() {
    
    cout << "Welcome to the Account Management System." << endl;
    StockAccount stockObject;
    BankAccount bankObject;
    operationMain(stockObject, bankObject);
    
    
    return 0;
}

// main instruction menu
void menuMain() {
    cout << "\nPlease select an account to access:\n";
    cout << "1. Stock Portfolio Account\n";
    cout << "2. Bank Account\n";
    cout << "3. Exit\n";
} // end menuMain

// instruction menu of stock account
void menuStock() {
    cout << "\nPlease select an option:\n";
    cout << "1. Display the price for a stock symbol\n";
    cout << "2. Display the current portfolio\n";
    cout << "3. Buy shares\n";
    cout << "4. Sell shares\n";
    cout << "5. View a graph for portfolio value\n";
    cout << "6. View transaction history\n";
    cout << "7. Return to previous menu\n";
} // end menuStock

// instruction menu of bank account
void menuBank() {
    cout << "\nPlease select an option:\n";
    cout << "1. View account balance\n";
    cout << "2. Deposit money\n";
    cout << "3. Withdraw money\n";
    cout << "4. Print out history\n";
    cout << "5. Return to previous menu\n";
} // end menuBank

// deposit/withdraw amount selecting
void amountSelect() {
    cout << "1. 100\n";
    cout << "2. 200\n";
    cout << "3. 500\n";
    cout << "4. 1000\n";
    cout << "5. Return to previous menu\n";
} // end amountSelect


// operation of selecting account
void operationMain(StockAccount & mystock, BankAccount & mycash) {
    
    int choice; // store user choice
    //string symbolin; // store inputed stock symbol
    //int sharesin; // store inputed stock shares
    //double tol; // store inputed tolerance limit for price
    bool flag = 1; // if exit while or not
    
    while (flag) // display selection table
    {
        menuMain(); // show selection list
        flag = 0;
        cin >> choice;
        switch (choice) {
            case 1:
            {
                //StockAccount stockObject;
                operationStock(mystock, mycash);
                break;
            }
            case 2:
            {
                //BankAccount bankObject;
                operationBank(mystock, mycash);
                break;
            }
            case 3:
            {
                cout << "Exit successfully, Thanks for using!" << endl;
                break;
            }
            default:
            {
                cout << "\nInvalid input. Please select again." << endl;
                flag = 1;
                break;
            }
        } // end switch
    } // end while
} // end operationMain

// operation of stock account
void operationStock(StockAccount & mystock, BankAccount & mycash) {
    
    int choice; // store user choice
    string symbolin; // store inputed stock symbol
    int sharesin; // store inputed stock shares
    double tol; // store inputed tolerance limit for price
    bool flag = 1; // if exit while or not
    
    while (flag) // display selection table
    {
        menuStock(); // show selection list
        
        flag = 0;
        cin >> choice;
        switch (choice) {
            case 1: // Display the price for a stock symbol
            {
                cout << "Please enter the stock symbol: ";
                cin >> symbolin;
                mystock.printPrice(symbolin);
                flag = 1;
                break;
            }
                
            case 2:
            {
                mystock.printPortfolio();
                flag = 1;
                break;
            }
                
            case 3: // buy
            {
                cout << "Please enter the stock symbol you wish to purchase: ";
                cin >> symbolin;
                cout << "Please enter the number of shares: ";
                cin >> sharesin;
                cout << "Please enter the maximum amount you are willing to pay per share: ";
                cin >> tol;
                Node buy(symbolin, sharesin);
                mystock.buyStock(&buy, tol);
                flag = 1;
                break;
            }
                
            case 4: // sell
            {
                cout << "Please enter the stock symbol you wish to sell: ";
                cin >> symbolin;
                cout << "Please enter the number of shares: ";
                cin >> sharesin;
                cout << "Please enter the minimum amount you are willing to sell per share: ";
                cin >> tol;
                Node sell(symbolin, sharesin);
                mystock.sellStock(&sell, tol);
                flag = 1;
                break;
            }
            
            case 5:
            {
                mystock.printTotalValue();
                flag = 1;
                break;
            }
            case 6: // display transaction history
            {
                mystock.printTransHistory();
                flag = 1;
                break;
            }
            
            case 7: // return to main menu
            {
                operationMain(mystock, mycash);
                break;
            }
            
            default:
            {
                cout << "\nInvalid input. Please select again." << endl;
                flag = 1;
                break;
            }
        } // end switch
    } // end while
} // end operationStock

// operation of bank account
void operationBank(StockAccount & mystock, BankAccount & mycash) {
    
    int choice, choiced, choicew; // store user choice
    //int amount;
    //string symbolin; // store inputed stock symbol
    //int sharesin; // store inputed stock shares
    //double tol; // store inputed tolerance limit for price
    bool flag = 1; // if exit while or not
    bool flagd = 1; // if exit while or not for deposit
    bool flagw = 1; // if exit while or not for withdraw
    
    while (flag) // display selection table
    {
        menuBank(); // show selection list
        
        flag = 0;
        cin >> choice;
        switch (choice) {
            case 1: // Display the price for a stock symbol
            {
                mycash.printPortfolio();
                flag = 1;
                break;
            }
            
            case 2:
            {
                while (flagd) {
                    cout << "Please select an amount you are willing to deposit: " << endl;
                    amountSelect();
                    flagd = 0;
                    cin >> choiced;
                
                    if (choiced == 1) mycash.deposit(100);
                    else if (choiced == 2) mycash.deposit(200);
                    else if (choiced == 3) mycash.deposit(500);
                    else if (choiced == 4) mycash.deposit(1000);
                    else if (choiced == 5) flagd = 0;
                    else { cout << "Invalid input. please select again."; flagd = 1; } // end else
                } // end while
                
                flag = 1;
                break;
            }
            case 3:
            {
                while (flagw) {
                    cout << "Please select an amount you are willing to withdraw: " << endl;
                    amountSelect();
                    flagw = 0;
                    cin >> choicew;
                    
                    if (choicew == 1) mycash.withdraw(100);
                    else if (choicew == 2) mycash.withdraw(200);
                    else if (choicew == 3) mycash.withdraw(500);
                    else if (choicew == 4) mycash.withdraw(1000);
                    else if (choicew == 5) flagw = 0;
                    else { cout << "Invalid input. please select again."; flagw = 1; } // end else
                } // end while
                
                flag = 1;
                break;
            }
                
            case 4:
            {
                mycash.printTransHistory();
                flag = 1;
                break;
            }
            case 5: // return to main menu
            {
                operationMain(mystock, mycash);
                break;
            }
                
            default:
            {
                cout << "\nInvalid input. Please select again." << endl;
                flag = 1;
                break;
            }

        } // end switch
    } // end while
} // end operationBank

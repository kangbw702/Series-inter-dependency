
//  Name: Bowei Kang
//  Course: 16:332:503 , PM for Financial Applications and Numerical Computing
//  Final Project: Account Management System
//  Source code for class StockAccount
//  2015/11/15

/*
#pragma comment(lib, "libmx.lib")
#pragma comment(lib, "libmex.lib")
#pragma comment(lib, "libeng.lib")


#include <yvals.h>
#if (_MSC_VER >= 1600)
#define __STDC_UTF_16__
#endif
#include "mex.h"

#include<engine.h>
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "StockAccount_BoweiKang.h"
using namespace std;

// constructor
StockAccount::StockAccount()
:myHead(0), myTail(0), Account()
{
    setStockPrice();
    currentPrice = randomSelectPrice();
    readPortfolio(); // read current portfolio into list
    
    
} // end constructor


// destructor
StockAccount::~StockAccount() {
    
    // List is not empty
    if (myHead != 0)
    {
        // let current pointer = first pointer
        Node * cptr = myHead;
        Node * tptr;    // temp pointer
        
        // delete remaining nodes
        while (cptr != 0) {
            tptr = cptr;
            cptr = cptr->next;
            delete tptr;
        } // end while
    } // end if
} // end destructor
 

// read stock price from file
map<string, double> StockAccount::readStockPrice(string filename) {
    
    string a;
    string token, date1;
    double price;
    
    map<string, double> data;
    
    ifstream myfilein(filename);
    
    if(! myfilein) {
        cerr << "Price files could not be opened." << endl;
        exit( 1 );
    } // end if
    
    while (! myfilein.eof()) {
        myfilein >> token >> price >> date1;
        data[token] = price;
    }
    
    myfilein.close();
    
    return data;
    
} // end readStockPrice


// set stock price candidates
void StockAccount::setStockPrice() {
    
    price1 = readStockPrice("Results_1.txt");
    price2 = readStockPrice("Results_2.txt");
    
} // end setStockPrice


// randomly select stock price
map <string, double> StockAccount::randomSelectPrice() {
    
    srand((unsigned)time(0));
    int r = rand() % 2;
    map <string, double> stockPriceRand;
    if (r == 0) return price1;
    else return price2;
    
} // end randomSelectPrice


// read portfolio infomation from file 
void StockAccount::readPortfolio() {

    string token;
    int share;
    
    ifstream myfilein("portfolio.txt");
    
    if(! myfilein) {
        ofstream myfileout("portfolio.txt");
        myfileout.close();
    } // end if
    
    else {
        while (myfilein >> token >> share) {
            Node initial(token, share);
            addToEnd(&initial);
        } // end while
    } // end else
    
    myfilein.close();
    
} // end readPortfolio


// add at end of list
void StockAccount::addToEnd(Node *itemPtr) {
    
    Node *newPtr = new Node(*itemPtr); // new Node
    
    if (myHead == 0) // List is empty
        myHead = myTail = newPtr;
    
    else // List is not empty
    {
        myTail->next = newPtr; // link original last node to new node
        newPtr->prev = myTail; // aim previousPtr to original last node
        myTail = newPtr; // aim lastPtr at new node
    } // end else
    
} // end addToEnd


// calculate total portfolio value (stock and cash)
double StockAccount::calculateTotalValue() {
    
    readBalance();
    double summ = 0.0;
    
    Node * cptr = myHead;
    while (cptr != 0) {
        summ = summ + (currentPrice[cptr->stockSymbol] * (cptr->stockShare));
        cptr = cptr->next;
    } // end while
    
    return (summ + getBalance());
    
} // end calculateTotalValue


// print current price by user defined stock symbol
void StockAccount::printPrice(string & symbolin) {
    
    currentPrice = randomSelectPrice(); // get current price
    
    map<string,double>::const_iterator got = currentPrice.find (symbolin);
    if (got == currentPrice.end())
        cout << symbolin << " not found" << endl;
    
    else {
        cout << setw(15) << left << "Stock Symbol" << setw(15) << left << "Price per Share" << endl;
        cout << setw(15) << left << got->first << setw(15) << left << got->second << endl;
    } // end else
} // end printPrice


// print current portfolio
void StockAccount::printPortfolio() {
   
    // get current price
    currentPrice = randomSelectPrice();
    
    // sort from largest value to smallest using quickSort
    quickSort(myHead, myTail);
    
    // print balance
    readBalance();
    cout << "Cash Balance = $" << getBalance() << endl;
    
    // print stock portfolio details
    if (myHead == 0)
        cout << "Stock portfolio is empty." << endl;
    else {
        // print header
        cout << setw(15) << left << "Stock_Symbol"
        << setw(10) << left << "Number"
        << setw(20) << left << "Price_per_Share"
        << setw(14) << left << "Total_Vlaue" << endl;
        
        Node *cptr = myHead;
        while (cptr != 0) {
            cout << setw(15) << left << cptr->stockSymbol << setw(10) << left << cptr->stockShare
            << setw(1) << left << "$" << setw(19) << left << currentPrice[cptr->stockSymbol]
            << setw(1) << left << "$" << setw(13) << left <<
            currentPrice[cptr->stockSymbol] * (cptr->stockShare) << endl;
            cptr = cptr->next;
        }
    } // end else
    
    // print total value
    cout << "Total portfolio value: $" << fixed << setprecision(2) << calculateTotalValue() << endl;
    
} // end printlist


// print transaction history
void StockAccount::printTransHistory() {
    
    bool buyorsell;
    string token, date1;
    int share;
    double price, value;
    
    ifstream myfilein("stock_transaction_history.txt");
    
    if(! myfilein) {
        ofstream myfileout("stock_transaction_history.txt");
        cout << "newly build a txt 5." << endl;
        myfileout.close();
        cout << "You have no transactions up to now." << endl;
    } // end if
    
    else {
        // print header
        cout << setw(8) << left << "Event" << setw(14) << left << "Stock_Symbol"
        << setw(9) << left << "Number" << setw(18) << left << "Price_per_Share"
        << setw(15) << left << "Total_Vlaue" << setw(5) << left << "Time" << endl;
        
        while (myfilein.peek() != EOF) {
            myfilein >> buyorsell >> token >> share >> price >> value;
            getline(myfilein, date1);
            
            cout << setw(8) << left << (buyorsell ? "sell" : "buy")
            << setw(14) << left << token << setw(9) << left << share << setw(18) << left << price
            << setw(14) << left << value << setw(30) << left << date1 << endl;
        } // end while
        
    } // end else
    
    myfilein.close();
    
} // end printTransaction


// print total portfolio value
void StockAccount::printTotalValue() {
    
    string date1;
    double value;
    //vector<double> value_series;
    
    time_t now = time(0);
    tm newtime;
    localtime_r(&now, &newtime);
    //localtime_s(&newtime, &now);
    
    ifstream myfilein("total_value.txt");
    
    if (!myfilein) {
        ofstream myfileout("total_value.txt");
        double initial_value = 10000.00;
        myfileout << initial_value << '\t'
        << newtime.tm_mon + 1 << "/" << newtime.tm_mday << "/" << newtime.tm_year + 1900 << " "
        << newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec << '\n';
        // Mmm-dd-yyyy hh:mm:ss
        
        // print out
        cout << setw(13) << left << "Total_Vlaue" << setw(5) << left << " Time" << endl;
        cout << setw(14) << left << initial_value << left <<  newtime.tm_mon + 1
        << "/" << newtime.tm_mday << "/" << newtime.tm_year + 1900 << " "
        << newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec << endl;
        
        myfileout.close();
    } // end if
    
    else {
        // print header
        cout << setw(15) << left << "Total_Vlaue" << setw(5) << left << " Time" << endl;
        
        while (myfilein.peek() != EOF) {
            myfilein >> value;
            getline(myfilein, date1);
            cout << setw(13) << left << value << setw(30) << left << date1 << endl;
            //value_series.push_back(value);
        } // end while
    } // end else
    
    /*
	int const n = 100;
	int iter;
    double plot_array[n];
	if ((value_series.size()) <= n)
		iter = value_series.size();
	else
		iter = n;

    for (int i = 0; i < iter; i++) 
		plot_array[i] = value_series[i]; 
    
    // creating a pointer to the engine
    Engine *ep;
    ep = engOpen(NULL);
    if (ep == NULL)
    {
        cout << "Error: Not Found." << endl;
        exit(1);
    }
    
    mxArray *A;
    A = mxCreateDoubleMatrix(1, iter, mxREAL);
    memcpy((void *)mxGetPr(A), (void *)plot_array, value_series.size()*sizeof(double));
    engPutVariable(ep, "xx", A);
    
    engEvalString(ep, "X=xx;");
    engEvalString(ep, "plot(X);");	//call plot function
    engEvalString(ep, "title('Total Portfolio Value');");
    mxDestroyArray(A);
    */

    myfilein.close();
    
} // end printTotalValue


// buy transaction
void StockAccount::buyStock(Node *itemPtr, double limit) {
    
    readBalance(); // get current cash balance
    currentPrice = randomSelectPrice(); // get current price
    
    map<string,double>::const_iterator got = currentPrice.find (itemPtr->stockSymbol);
    if (got == currentPrice.end()) // cannot find corresponding stock price
        cout << itemPtr->stockSymbol << " is not in our transaction system." << endl;
    else {
        if (limit < currentPrice[itemPtr->stockSymbol]) // current price exeed cliens tolerance
            cout << "Current price of " << itemPtr->stockSymbol << " is too high for you." << endl;
        else {
            if ((itemPtr->stockShare)*(currentPrice[itemPtr->stockSymbol]) > getBalance()) // not enough fund
                cout << "Your current cash balance is not enough for this purchasing transaction." << endl;
            else {
                if (myHead == 0)
                    addToEnd(itemPtr);
                else {
                    Node *currentPtr = myHead; // current pointer
                    bool flag = false; // if find stock name in list
        
                    while (currentPtr != 0 && flag == false) {
                        if (currentPtr->stockSymbol == itemPtr->stockSymbol) {
                            currentPtr->stockShare = currentPtr->stockShare + itemPtr->stockShare;
                            flag = true;
                        } // end if
                        else
                            currentPtr = currentPtr->next; // move current pointer to next node
                    } // end while
        
                    if (flag == false) // not found same stock in list
                        addToEnd(itemPtr);
                } // end else
                
                double diffBalance = currentPrice[itemPtr->stockSymbol] * (itemPtr->stockShare);
                setBalance(getBalance() - diffBalance); // set balance
                updateBalance(); // update cash balance in file
                appTransStock(0, itemPtr); // update transaction history file
                appTransCash(0, diffBalance); // update bank transaction history file
                updatePortfolio(); // update portfolio file
                updateTotalValue(); // update total value in file

                cout << "\nYou have purchased " << itemPtr->stockShare << " share"
                << (itemPtr->stockShare == 1 ? "" : "s") << " of "
                << itemPtr->stockSymbol << " at $" << currentPrice[itemPtr->stockSymbol]
                << " each for a total of $" << (currentPrice[itemPtr->stockSymbol] * (itemPtr->stockShare))
                << endl;
            } // end else
        } // end else
    } // end else
} // end buyStock


// sell transaction
void StockAccount::sellStock(Node *itemPtr, double limit) {
    
    readBalance(); // get current cash balance
    currentPrice = randomSelectPrice(); // get current price
    
    int cshare = 0; // current share of this stock if in list
    
    if (myHead == 0)
        cout << "You have no stocks in account now." << endl;
    else {
        Node *tPtr = myHead; // temp pointer
        bool flag1 = false; // indicator find or not
        while (tPtr != 0 && flag1 == false) {
            if (tPtr->stockSymbol == itemPtr->stockSymbol) {
                flag1 = true;
                cshare = tPtr->stockShare;
            }
            else
                tPtr = tPtr->next; // move to next node
        } // end while
        if (flag1 == false)  cout << "The stock you choose is not in your portfolio." << endl;
        else {
            if (limit > currentPrice[itemPtr->stockSymbol]) // current price exeed cliens tolerance
                cout << "Current price of " << itemPtr->stockSymbol << " is too low for you." << endl;
            else {
                if (itemPtr->stockShare > cshare) // not enough share of stocks
                    cout << "You have not enough shares for this selling transaction." << endl;
                else { // stock can be found in list and has valid selling shares <=
                    Node *cPtr = myHead; // current pointer
                    bool flag2 = false;
                    
                    while (cPtr != 0 && flag2 == false) {
                        if (cPtr->stockSymbol == itemPtr->stockSymbol) {
                            if (itemPtr->stockShare < cPtr->stockShare)
                                cPtr->stockShare = cPtr->stockShare - itemPtr->stockShare;
                            else { // sell out this stock, need remove node
                                Node *dPtr = cPtr; // temp ptr used to delete node
                                if (dPtr == myHead && dPtr == myTail) { // have only one node
                                    myHead = myTail = 0;
                                    delete dPtr;
                                } // end if
                                else {
                                    if (dPtr == myHead) { // delete at beginning
                                        myHead = dPtr->next;
                                        (dPtr->next->prev) = 0;
                                        delete dPtr;
                                    } // end if
                                    else {
                                        if (dPtr == myTail) { // delete at end
                                            myTail = dPtr->prev;
                                            (dPtr->prev->next) = 0;
                                            delete dPtr;
                                        } // end if
                                        else { // delete at middle
                                            (dPtr->prev->next) = dPtr->next;
                                            (dPtr->next->prev) = dPtr->prev;
                                            delete dPtr;
                                        } // end else
                                    } // end else
                                } // end else
                            } // end else
                            flag2 = true;
                        } // end if
                        else  cPtr = cPtr->next; // move to next node
                    } // end while
                    
                    double diffBalance = currentPrice[itemPtr->stockSymbol] * (itemPtr->stockShare);
                    setBalance(getBalance() + diffBalance);
                    updateBalance(); // update cash balance in file
                    appTransStock(1, itemPtr); // update stock transaction history file
                    appTransCash(1, diffBalance); // update bank transaction history file
                    updatePortfolio(); // update portfolio file
                    updateTotalValue(); // update total value in file
                
                    cout << "\nYou have sold " << itemPtr->stockShare << " share"
                    << (itemPtr->stockShare == 1 ? "" : "s") << " of "
                    << itemPtr->stockSymbol << " at $" << currentPrice[itemPtr->stockSymbol]
                    << " each for a total of $" << (currentPrice[itemPtr->stockSymbol] * (itemPtr->stockShare))
                    << endl;
                } // end else
            } // end else
        } // end else
    } // end else
} // end sellStock


// appendding transaction information to file (stock history) (0-buy, 1-sell)
void StockAccount::appTransStock(bool buyorsell, Node *Ptr) {
    
    ofstream transcFile("stock_transaction_history.txt", ios::app);
    
    if(! transcFile) {
        cerr << "File could not be opened 3" << endl;
        exit( 1 );
    } // end if
    
    time_t now = time(0);
    tm newtime;
	localtime_r(&now, &newtime);
    //localtime_s(&newtime, &now);
    
    transcFile << buyorsell << '\t'
    << Ptr->stockSymbol << '\t'
    << Ptr->stockShare << '\t'
    << currentPrice[Ptr->stockSymbol] << '\t'
    << (currentPrice[Ptr->stockSymbol] * Ptr->stockShare) << '\t'
    << newtime.tm_mon + 1 << "/" << newtime.tm_mday << "/" << newtime.tm_year + 1900 << " "
    << newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec << '\n';
    // Mmm-dd-yyyy hh:mm:ss
    
    transcFile.close();
    
} // end appTransStock


// appendding transaction information to file (bank history) (0-buy, 1-sell)
void StockAccount::appTransCash(int event, double money) {
    
    ofstream transcFile("bank_transaction_history.txt", ios::app);
    
    if(! transcFile) {
        cerr << "File could not be opened 7" << endl;
        exit( 1 );
    } // end if
    
    time_t now = time(0);
    tm newtime;
    localtime_r(&now, &newtime);
    //localtime_s(&newtime, &now);
    
    // Convert now to tm struct for local timezone
    //tm* localtm = localtime(&now);
    //tm timeinfo;
    //localtime_s(&timeinfo, &now);
    
    transcFile << event << '\t'
    << money << '\t'
    << getBalance() << '\t'
    << newtime.tm_mon + 1 << "/" << newtime.tm_mday << "/" << newtime.tm_year + 1900 << " "
    << newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec << '\n';
    // Mmm-dd-yyyy hh:mm:ss
    
    transcFile.close();
    
} // end appTransCash


// update portfolio file after transaction
void StockAccount::updatePortfolio() {
    
    ofstream myfileout("portfolio.txt");
    
    if(! myfileout) {
        cerr << "File could not be opened 4" << endl;
        exit( 1 );
    } // end if
    
    if (myHead == 0)
        myfileout << '\n';
    
    else {
        Node *currentPtr = myHead;
        while (currentPtr != 0) {
            myfileout << currentPtr->stockSymbol << '\t' << currentPtr->stockShare << '\n';
            currentPtr = currentPtr->next;
        } // end while
    } // end else
    
    myfileout.close();
    
} // end updatePortfolio


// update Ttotal portfolio value to file
void StockAccount::updateTotalValue() {
    
    ofstream myfileout("total_value.txt", ios::app);
    
    if(! myfileout) {
        cerr << "File could not be opened 4" << endl;
        exit( 1 );
    } // end if
    
    double value = calculateTotalValue();
    
    time_t now = time(0);
    tm newtime;
    localtime_r(&now, &newtime);
    //localtime_s(&newtime, &now);
    
    myfileout << value << '\t'
    << newtime.tm_mon + 1 << "/" << newtime.tm_mday << "/" << newtime.tm_year + 1900 << " "
    << newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec << '\n';
    // Mmm-dd-yyyy hh:mm:ss
    
    myfileout.close();
    
} // end updateTotalValue


// utility function to swap two elements for sorting
void StockAccount::swap(Node * a, Node * b) {
    string s;
    int t;
    s = a->stockSymbol;
    t = a->stockShare;
    a->stockSymbol = b->stockSymbol;
    a->stockShare = b->stockShare;
    b->stockSymbol = s;
    b->stockShare = t;
} // end swap

// partition for quick sorting
Node * StockAccount::partition(Node * left, Node * right) {
    
    double pivot_val;
    
    // capture pivot value at rightmost
    pivot_val = right->stockShare * currentPrice[right->stockSymbol];
    Node * i = left->prev;
    
    for (Node * j = left; j != right; j = j->next) {
        if (j->stockShare * currentPrice[j->stockSymbol] >= pivot_val) {
            i = (i == 0) ? left : i->next;
            swap(i, j);
        } // end if
    } // end for
    
    i = (i == 0) ? left : i->next;
    swap(i, right);
    return i;

} // end partition


// quick sort
void StockAccount::quickSort(Node * low, Node * high) {
    
    if ((high != 0) && (low != high) && (low != high->next)) {
        Node * p = partition(low, high);
        quickSort(low, p->prev);
        quickSort(p->next, high);
    } // end if
    
} // end quickSort






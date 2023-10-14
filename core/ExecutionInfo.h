#include <iostream>
#include <ctime>
#ifndef EXECUTIONREPORT_INCL_H
#define EXECUTIONREPORT_INCL_H
#include "ExecutionReport.h"
#endif

using namespace std;

class ExecutionInfo {
private:
    // equivalent to fix tag 17 
    string _execid;
    // equivalent to fix tag 48
    string _symbol;
    // equivalent to fix tag 44
    double _price;
    // equivalent to fix tag 32
    double _qty;
    // active buy - 'A'; active sell - 'B'
    char _activeside;
    ExecutionReport _buyexec;
    ExecutionReport _sellexec;
    // equivalent to fix tag 60
    time_t _exectime;

public:
    ExecutionInfo(string,string,double,double,char,ExecutionReport,ExecutionReport,time_t);
    string execid() { return _execid; }
    string symbol() { return _symbol; }
    double price() { return _price; }
    double qty() { return _qty; }
    char activeside() { return _activeside; }
    time_t exectime() { return _exectime; }
    ExecutionReport buyexec() { return _buyexec; }
    ExecutionReport sellexec() { return _sellexec; }
    ~ExecutionInfo() { cout << "ExecutionInfo Destructor: " << this << endl; }
    void setbuyexec(ExecutionReport*); 
    void setsellexec(ExecutionReport*); 
   
};

ExecutionInfo::ExecutionInfo(string execid,string symbol,double price,double qty,char activeside,ExecutionReport buyexec, ExecutionReport sellexec,time_t exectime=time(0)) {
    _execid=execid;
    _symbol=symbol;
    _price=price;
    _qty=qty;
    _activeside=activeside;
    _buyexec = ExecutionReport(buyexec);
    _sellexec = ExecutionReport(sellexec);
    _exectime=exectime;
};

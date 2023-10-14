#ifndef EXECUTIONREPORT_H
#define EXECUTIONREPORT_H

#include <iostream>
#include <ctime>
using namespace std;

class ExecutionReport {
private:
    // equivalent to fix tag 17 
    string _execid;
    // equivalent to fix tag 48
    string _symbol;
    // equivalent to fix tag 37
    string _orderid;
    // equivalent to fix tag 39
    char _orderstatus;
    // equivalent to fix tag 44
    double _price;
    // equivalent to fix tag 32
    double _lastqty;
    // equivalent to fix tag 14
    double _cumqty;
    // equivalent to fix tag 60
    time_t _exectime;

public:
    ExecutionReport() {}
    ExecutionReport(string,string,string,char,double,double,double,time_t);
    ExecutionReport(ExecutionReport&);
    string execid() { return _execid; }
    string symbol() { return _symbol; }
    string orderid() { return _orderid; }
    char orderstatus() { return _orderstatus; }
    double price() { return _price; }
    double lastqty() { return _lastqty; }
    double cumqty() { return _cumqty; }
    time_t exectime() { return _exectime; }
    ~ExecutionReport() { cout << "\nExecutionReport Destructor: " << this << endl; }
};

ExecutionReport::ExecutionReport(ExecutionReport& e) {
    _execid=e.execid();
    _symbol=e.symbol();
    _orderid=e.orderid();
    _orderstatus=e.orderstatus();
    _price=e.price();
    _lastqty=e.lastqty();
    _cumqty=e.cumqty();
    _exectime=e.exectime();
}


ExecutionReport::ExecutionReport(string execid,string symbol,string orderid,char orderstatus,double price,double lastqty,double cumqty,time_t exectime=time(0)) {
    _execid=execid;
    _symbol=symbol;
    _orderid=orderid;
    _orderstatus=orderstatus;
    _price=price;
    _lastqty=lastqty;
    _cumqty=cumqty;
    _exectime=exectime;
}


#endif //EXECUTIONREPORT_H

#include <iostream>
#include <ctime>
using namespace std;

class OrderInfo {
  private:
    bool _isdummy;
    // equivalent to fix tag 49
    string _senderid;
    // equivalent to fix tag 37
    string _orderid;
    // equivalent to fix tag 48
    string _symbol;
    // equivalent to fix tag 35 (D for new order, F for cancel order)
    char _msgtype;
    // equivalent to fix tag 38
    double _orderqty;
    // equivalent to fix tag 40 (1 for market order, 2 for limit order)
    char _ordertype;
    // equivalent to fix tag 44	
    double _price;
    // equivalent to fix tag 54
    char _side;
    // equivalent to fix tag 60
    time_t _transacttime;
    // equivalent to fix tag 14
    double _cumqty;
    // equivalent to fix tag 32
    double _lastqty;

  public:
    OrderInfo() { _isdummy = 1; }
    OrderInfo(string,string,string,char,double,char,time_t);//market order constructor
    OrderInfo(string,string,string,char,double,double,char,time_t);//limit order constructor
    OrderInfo(string,string,time_t);//cancel order constructor
    string senderid() { return _senderid; } 
    string orderid() { return _orderid; }
    string symbol() { return _symbol; }
    char msgtype() { return _msgtype; }
    double orderqty() { return _orderqty; }
    char ordertype() { return _ordertype; }
    double price() { return _price; }
    char side() { return _side; }
    time_t transacttime() { return _transacttime; }
    bool  isdummy() { return _isdummy; }
    double cumqty() { return _cumqty; }
    double lastqty() { return _lastqty; }
    void lastqty(double lastqty) { _lastqty = lastqty; _cumqty += _lastqty; } 
    double outstandingqty() { return _orderqty - _cumqty; }
    ~OrderInfo() { cerr << "\nOrderInfo Destructor: " << this << endl; }

};

OrderInfo::OrderInfo(string senderid,string orderid,string symbol,char msgtype,double orderqty,char side,time_t transacttime=time(0)) {
    _isdummy = 0;
    _senderid=senderid; 
    _orderid=orderid;
    _symbol=symbol;
    _msgtype=msgtype;
    _orderqty=orderqty;
    _ordertype='1';
    _side=side;
    _transacttime=transacttime;
    _cumqty = 0;
}

OrderInfo::OrderInfo(string senderid,string orderid,string symbol,char msgtype,double orderqty,double price,char side,time_t transacttime=time(0)) {
    _isdummy = 0; 
    _senderid=senderid;
    _orderid=orderid;
    _symbol=symbol;
    _msgtype=msgtype;
    _orderqty=orderqty;
    _ordertype='2';
    _price=price;
    _side=side;
    _transacttime=transacttime;
    _cumqty = 0;
}

OrderInfo::OrderInfo(string senderid,string orderid,time_t transacttime=time(0)) {
    _senderid=senderid;
    _orderid=orderid;
    _msgtype='G';
    _transacttime=transacttime;
}



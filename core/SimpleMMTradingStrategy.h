#include <iostream>
using namespace std;

#ifndef TRADINGSTRATEGY_INCL_H
#define TRADESTRATEGY_INCL_H
#include "TradingStrategy.h"
#endif

#ifndef POSITIONKEEPER_INCL_H
#define POSITIONKEEPER_INCL_H
#include "PositionKeeper.h"
#endif

class SimpleMMTradingStrategy: public TradingStrategy {
private:
     string _symbol;
     string _ccy;
     double _initcashbalance;
     double _initinstrumentbalance;
     double _initinstrumentsecmarketprice;
     double _securitysecmarketbestbid;
     double _securitysecmarketbestask;
     double _securitysecmarketbestbidqty;
     double _securitysecmarketbestaskqty;
     double _orderqty;
     double _pricemargin;
     double _orderqtymargin;

     PositionKeeper* _pk;

     /*
      * These two functions are for initial checking for 
      * further market making checking
      */
     bool isBuySecondaySellPrimaryPotential(OrderBook& orderBook) {
        return orderBook.bestbid() > _securitysecmarketbestask;
     }
     bool isSellSecondaryBuyPrimaryPotential(OrderBook& orderBook) {
        return _securitysecmarketbestbid > orderBook.bestask();
     }  
	     

public:
     SimpleMMTradingStrategy(string ccy, double initcashbalance, string symbol, double initinstrumentbalance, double securitysecmarketbestbid, double securitysecmarketbestask, double securitysecmarketbestbidqty, double securitysecmarketbestaskqty,double orderqty,double pricemargin,double orderqtymargin) {
        _ccy=ccy;
	_symbol=symbol;
	_initcashbalance=initcashbalance;
	_initinstrumentbalance=initinstrumentbalance;
        _securitysecmarketbestbid=securitysecmarketbestbid;
        _securitysecmarketbestask=securitysecmarketbestask;
        _securitysecmarketbestbidqty=securitysecmarketbestbidqty;
        _securitysecmarketbestaskqty=securitysecmarketbestaskqty;
	_initinstrumentsecmarketprice=(securitysecmarketbestbid+securitysecmarketbestask)/2;
	_orderqty=orderqty;
	_pricemargin=pricemargin;
	_orderqtymargin=orderqtymargin;
	_pk = new PositionKeeper(_ccy,_initcashbalance,_symbol,_initinstrumentbalance);
     }
     ~SimpleMMTradingStrategy() {
        delete _pk;
     }
     void setsecmarket(double securitysecmarketbestbid, double securitysecmarketbestask, double securitysecmarketbestbidqty, double securitysecmarketbestaskqty) {
        _securitysecmarketbestbid=securitysecmarketbestbid;
        _securitysecmarketbestask=securitysecmarketbestask;
        _securitysecmarketbestbidqty=securitysecmarketbestbidqty;
        _securitysecmarketbestaskqty=securitysecmarketbestaskqty;
     }
     string ccy() { return _ccy; }
     string symbol() { return _symbol; }
     double initcashbalance() { return _initcashbalance; }
     double initinstrumentbalance() { return _initinstrumentbalance; }
     double initinstrumentsecmarketprice() { return _initinstrumentsecmarketprice; }
     double securitysecmarketbestbid() { return _securitysecmarketbestbid; }
     double securitysecmarketbestask() { return _securitysecmarketbestask; }
     double securitysecmarketbestbidqty() { return _securitysecmarketbestbidqty; }
     double securitysecmarketbestaskqty() { return _securitysecmarketbestaskqty; }
     double cashbalance() { return _pk->cashbalance(); }
     double instrumentbalance() { return _pk->instrumentbalance(); }
     double orderqty() { return _orderqty; }
     double pricemargin() { return _pricemargin; }
     double orderqtymargin() { return _orderqtymargin; }
     void onOrderAdd(OrderBook& orderBook, const OrderInfo& orderInfo) {
        //TODO

     }
     void onOrderCancel(OrderBook& orderBook, const OrderInfo& orderInfo) {
        //TODO

     }
     void onOrderExecution(OrderBook& orderBook, const OrderInfo& orderInfo, const ExecutionInfo& executionInfo) {
        //TODO

     }
     void onSendOrder(OrderBook& orderBook, const OrderInfo& orderInfo) {
        //TODO

     }

};

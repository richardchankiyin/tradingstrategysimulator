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
     string _securityid;
     string _ccy;
     double _initcashbalance;
     double _initinstrumentbalance;
     PositionKeeper* _pk;  

public:
     SimpleMMTradingStrategy(string ccy, double initcashbalance, string securityid, double initinstrumentbalance) {
        _ccy=ccy;
	_securityid=securityid;
	_initcashbalance=initcashbalance;
	_initinstrumentbalance=initinstrumentbalance;
	_pk = new PositionKeeper(_ccy,_initcashbalance,_securityid,_initinstrumentbalance);
     }
     ~SimpleMMTradingStrategy() {
        delete _pk;
     }
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

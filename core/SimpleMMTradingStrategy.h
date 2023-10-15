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

#ifndef UTILS_INCL_H
#define UTILS_INCL_H
#include "Utils.h"
#endif

/**
 *
 *
 * Market Making algo on order handling
 - send order
  - ismminprogress -> true
  - store orderinfo internally
  - update positionkeeper balances
  - pending execution/add
  - if execution fully
      - trade secondary market
      - capture secondary market execution
      - update positionkeeper balances
      - send primary leg and secondary leg to post trade

  - else if execution rejected
      - send primary leg to post trade
      - update positionkeeper

  - else (assuming partially filled/new)
      - capture order add
      - capture outstanding qty
      - send cancel order on this order add
      - on order cancel 
      - update positionkeeper
      - send primary leg to post trade

      
  - remove orderinfo from internal storage
  - ismminprogress -> false*
 *
 */

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
     bool _ismminprogress;
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


     /**
      * detail checking of possible MM through liquidity checking
      * using bufferredorderqty and least favour price in secondary
      * market
      */     
     bool isBuySecondarySellPrimaryFeasible(OrderBook& orderBook) {
        if (isBuySecondaySellPrimaryPotential(orderBook)) {
	    double boqty = bufferredorderqty();

	    if (_securitysecmarketbestaskqty > boqty) {
                 std::vector<std::pair<double,vector<OrderInfo>>> bidqueue=orderBook.bidqueue();
                 double primaryavgprice = Utils::avgprice(bidqueue, boqty);
		 if (primaryavgprice == -1) {
                     //avg price cannot be determined
		     return 0;
		 }
		 double secondarylfprice = Utils::leastfavourprice(_securitysecmarketbestask, 1, _pricemargin);
		 return secondarylfprice < primaryavgprice;
	    } else {
                 return 0;
	    } 
	    return 0;
	}
	else {
            return 0;
	}	
     }

     bool isSellSecondaryBuyPrimaryFeasible(OrderBook& orderBook) {
        if (isSellSecondaryBuyPrimaryPotential(orderBook)) {
            double boqty = bufferredorderqty();

            if (_securitysecmarketbestbidqty > boqty) {
               std::vector<std::pair<double,vector<OrderInfo>>> askqueue=orderBook.askqueue();
               double primaryavgprice = Utils::avgprice(askqueue, boqty);
	       if (primaryavgprice == -1) {
                    //avg price cannot be determined
	            return 0;
	       }
	       double secondarylfprice = Utils::leastfavourprice(_securitysecmarketbestbid, 0, _pricemargin);
	       return primaryavgprice < secondarylfprice;
	    } else {
               return 0;
	    }

	} else {
            return 0;
	}
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
	_ismminprogress=0;
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
     bool ismminprogress() { return _ismminprogress; }
     double bufferredorderqty() {
         return _orderqty * (1+_orderqtymargin); 
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

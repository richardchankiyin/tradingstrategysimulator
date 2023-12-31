#include <iostream>
#include <tuple>
#include <map>
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

#ifndef CLOCK_INCL_H
#define CLOCK_INCL_H
#include "Clock.h"
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
     string _id;
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
     unsigned int _ordercreated;
     map<unsigned int,pair<OrderInfo,OrderInfo>> _orderstored;
     Clock* _clock;
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
     tuple<bool,double,double> isBuySecondarySellPrimaryFeasible(OrderBook& orderBook) {
        if (isBuySecondaySellPrimaryPotential(orderBook)) {
	    double boqty = bufferredorderqty();

	    if (_securitysecmarketbestaskqty > boqty) {
                 std::vector<std::pair<double,vector<OrderInfo>>> bidqueue=orderBook.bidqueue();
                 double primaryavgprice = Utils::avgprice(bidqueue, boqty);
		 if (primaryavgprice == -1) {
                     //avg price cannot be determined
		     return {0,-1,-1};
		 }
		 double secondarylfprice = Utils::leastfavourprice(_securitysecmarketbestask, 1, _pricemargin);
		 return {secondarylfprice < primaryavgprice,secondarylfprice,primaryavgprice};
	    } else {
                 return {0,-1,-1};
	    } 
	    return {0,-1,-1};
	}
	else {
            return {0,-1,-1};
	}	
     }

     tuple<bool,double,double> isSellSecondaryBuyPrimaryFeasible(OrderBook& orderBook) {
        if (isSellSecondaryBuyPrimaryPotential(orderBook)) {
            double boqty = bufferredorderqty();

            if (_securitysecmarketbestbidqty > boqty) {
               std::vector<std::pair<double,vector<OrderInfo>>> askqueue=orderBook.askqueue();
               double primaryavgprice = Utils::avgprice(askqueue, boqty);
	       if (primaryavgprice == -1) {
                    //avg price cannot be determined
	            return {0,-1,-1};
	       }
	       double secondarylfprice = Utils::leastfavourprice(_securitysecmarketbestbid, 0, _pricemargin);
	       return {primaryavgprice < secondarylfprice,secondarylfprice,primaryavgprice};
	    } else {
               return {0,-1,-1};
	    }

	} else {
            return {0,-1,-1};
	}
     }

     void onStrategyCheck(OrderBook& orderBook) {
        if (_ismminprogress) {
                cerr << "ismminprogress...skip" << endl;
	} else {
            tuple<bool,double,double> buysecsellprimaryassess = isBuySecondarySellPrimaryFeasible(orderBook);
            if (get<0>(buysecsellprimaryassess)) {
                cout << "preparing to buy secondary and sell primary..." << endl; 
		double instbalance = _pk->instrumentbalance();
		bool isshortsell = _orderqty > instbalance;
		double buysecondaryprice = get<1>(buysecsellprimaryassess);
		double sellprimaryprice = get<2>(buysecsellprimaryassess);
		string sellprimaryorderid = nextorderid();
		string buysecondaryorderid = sellprimaryorderid; buysecondaryorderid.append("_SEC");
		OrderInfo sellprimaryorder = OrderInfo(_id,sellprimaryorderid,_symbol,'D',_orderqty,sellprimaryprice,isshortsell ? '5' : '2', currenttime());
		OrderInfo buysecondaryorder = OrderInfo(_id,buysecondaryorderid,_symbol,'D',_orderqty,buysecondaryprice,'1',currenttime());
		pair<OrderInfo,OrderInfo> orderpair = {buysecondaryorder,sellprimaryorder};
		_orderstored.insert({++_ordercreated,orderpair});
		// for sell order first deduct instrument balance and wait for execution to add cash balance back
		_pk->addinstrument(-1 * _orderqty);
		// set is_mminprogress as truea
		_ismminprogress = 1;
		// send sellprimaryorder out
		onSendOrder(orderBook,sellprimaryorder);
	    } else {
                tuple<bool,double,double> sellsecbuyprimaryassess = isSellSecondaryBuyPrimaryFeasible(orderBook);
		if (get<0>(sellsecbuyprimaryassess)) {
                   cout << "preparing to sell secondary and buy primary..." << endl; 
		   double cbalance = _pk->cashbalance();
		   double sellsecondaryprice = get<1>(sellsecbuyprimaryassess);
		   double buyprimaryprice = get<2>(sellsecbuyprimaryassess);
		   double buyprimarynominal = buyprimaryprice * _orderqty;
		   if (buyprimarynominal > cbalance) {
                       // not enough buying power not proceed
                       cout << "warning!!!buyprimarynominal:" << buyprimarynominal << "> cashbalance:" << cbalance << endl;
		   } else {
		       string buyprimaryorderid = nextorderid();
		       string sellsecondaryorderid = buyprimaryorderid + "_SEC";
		       OrderInfo buyprimaryorder = OrderInfo(_id,buyprimaryorderid,_symbol,'D',_orderqty,buyprimaryprice,'1', currenttime());
		       OrderInfo sellsecondaryorder = OrderInfo(_id,sellsecondaryorderid,_symbol,'D',_orderqty,sellsecondaryprice,'2',currenttime());
		       pair<OrderInfo,OrderInfo> orderpair = {sellsecondaryorder,buyprimaryorder};
		       _orderstored.insert({++_ordercreated,orderpair});

		       // for buy order first deduct cash balance and wait for execution to add instrument balance back
		       _pk->addcash(-1 * buyprimarynominal);
		       // set is_mminprogress as truea
		       _ismminprogress = 1;
		       // send sellprimaryorder out
		       onSendOrder(orderBook,buyprimaryorder);
		   }
                } else {
                   //nothing happen 
		}
	    } 

	    
	}
     }


     void settleSellPrimaryBuySecondary(ExecutionReport buysecondaryer, ExecutionReport sellprimaryer) {
         // sell primary buy secondary -> deduct stock, then add stock, add cash (primary nominal - secondary nominal)	
         double filledqty = sellprimaryer.cumqty();
	 _pk->addinstrument(filledqty);
	 double primarynominal = sellprimaryer.price() * sellprimaryer.cumqty();
	 double secondarynominal = buysecondaryer.price() * buysecondaryer.cumqty();
	 _pk->addcash(primarynominal-secondarynominal);
     }

     void settleBuyPrimarySellSecondary(ExecutionReport sellsecondaryer, ExecutionReport buyprimaryer) {
	 // buy primary sell secondary -> deduct cash, then add cash with secondary nominal 
	 double secondarynominal = sellsecondaryer.price() * sellsecondaryer.cumqty();
	 _pk->addcash(secondarynominal);
     }

     void posttrade(OrderInfo secondaryorder, ExecutionReport secondaryer, OrderInfo primaryorder, ExecutionReport primaryer) {
         //TODO a function to send order to post trade analysis/middle office
	 cout << "Post Trading processing...." << endl;
     }
     /**
      * This is a function to mimic secondary market trading
      * always fully filled
      */
     ExecutionReport tradeSecondaryMarket(OrderInfo oi) {
          bool isbuy = oi.side() == '1';
	  double price = isbuy ? _securitysecmarketbestask : _securitysecmarketbestbid;
          ExecutionReport er = ExecutionReport(oi.orderid().append("_EXEC"),oi.symbol(),oi.orderid(),'8',price,oi.orderqty(),oi.orderqty(),oi.orderqty(),oi.side(),currenttime());
	  return er;   
     }

public:
     SimpleMMTradingStrategy(string id,string ccy, double initcashbalance, string symbol, double initinstrumentbalance, double securitysecmarketbestbid, double securitysecmarketbestask, double securitysecmarketbestbidqty, double securitysecmarketbestaskqty,double orderqty,double pricemargin,double orderqtymargin,Clock* clock) {
        _id=id;
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
	_ordercreated=0;
	_clock=clock;
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
     string id() { return _id; }
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
     unsigned int ordercreated() { return _ordercreated; }
     bool isorderbookrelevant(OrderBook& orderBook) { return _symbol == orderBook.symbol(); }
     string nextorderid() { 
	     string result = _id;
	     result.append("_ORD_").append(to_string(_ordercreated + 1));
	     return result; }
     time_t currenttime() { return _clock == nullptr ? time(0) : _clock->currenttime(); }
     OrderInfo outstandingorder() { return _ismminprogress ? _orderstored.find(_ordercreated)->second.second : OrderInfo(); }
     OrderInfo outstandingsecorder() { return _ismminprogress ? _orderstored.find(_ordercreated)->second.first : OrderInfo(); }
     double bufferredorderqty() {
         return _orderqty * (1+_orderqtymargin); 
     }  
     void onOrderAdd(OrderBook& orderBook, const OrderInfo& orderInfo) {
        if (!isorderbookrelevant(orderBook)) {
             return ;
	}
	if (_ismminprogress) {
	   // no special handling for market execption handling, just need to trigger strategy check
	   // TODO
	} else {
	    onStrategyCheck(orderBook);
	}
     }
     void onOrderCancel(OrderBook& orderBook, const OrderInfo& orderInfo) {
        if (!isorderbookrelevant(orderBook)) {
             return ;
	}
        if (_ismminprogress) {
            // pending order cancel to come back for trading strategy market exception handling
            //TODO to be implemented
	    
	} else {
            // perform strategy checking	
            onStrategyCheck(orderBook);
	}

     }
     void onOrderExecution(OrderBook& orderBook, const OrderInfo& orderInfo, const ExecutionInfo& executionInfo) {
        if (!isorderbookrelevant(orderBook)) {
             return ;
	}
        cerr << "onOrderExecution triggered......" << endl;
        if (_ismminprogress) {
            // pending execution report to coming for second market leg handling, PK balance update and other post trade processes
	    OrderInfo outstandingprimaryorder = outstandingorder();
	    OrderInfo incomingorder = orderInfo;
	    if (incomingorder.orderid() == outstandingprimaryorder.orderid()) {
                // match outstanding, proceed to check status
	        bool isoutstandingprimaryorderbuy=outstandingprimaryorder.side()=='1';
		ExecutionReport outstandingprimaryorderexecutionreport = isoutstandingprimaryorderbuy ? executionInfo._buyexec : executionInfo._sellexec;
		// check from execution report status is fully filled ('8')
		if (outstandingprimaryorderexecutionreport.orderstatus() == '8') {
			OrderInfo outstandingsecondaryorder = outstandingsecorder();
			ExecutionReport outstandingsecondaryorderexecutionreport = tradeSecondaryMarket(outstandingsecondaryorder);
			if (isoutstandingprimaryorderbuy) {
                            settleBuyPrimarySellSecondary(outstandingsecondaryorderexecutionreport,outstandingprimaryorderexecutionreport);
			} else {
                            settleSellPrimaryBuySecondary(outstandingsecondaryorderexecutionreport,outstandingprimaryorderexecutionreport);
			}
			posttrade(outstandingsecondaryorder,outstandingsecondaryorderexecutionreport,outstandingprimaryorder,outstandingprimaryorderexecutionreport);
			_ismminprogress=0;
		} else {
                    //TODO for partial fill or reject case they are execption and to be handled here
		    //to be implemented
		}
	    }
	    
	} else {

            // perform strategy checking	
            onStrategyCheck(orderBook);
	}

	
     }
     void onSendOrder(OrderBook& orderBook, const OrderInfo& orderInfo) {
        if (!isorderbookrelevant(orderBook)) {
             return ;
	}
        orderBook.receiveorder(orderInfo);
     }
     void onMarketDataChange(OrderBook& orderBook, double secmarketbestbid, double secmarketbestbidqty, double secmarketbestask, double secmarketbestaskqty) {
	setsecmarket(secmarketbestbid,secmarketbestask,secmarketbestbidqty,secmarketbestaskqty);
        onStrategyCheck(orderBook);
     }

};

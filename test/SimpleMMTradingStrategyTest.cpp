#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../simpletest/simpletest.cpp"

#ifndef ORDERBOOK_INCL_H
#define ORDERBOOK_INCL_H 
#include "../core/OrderBook.h"
#endif

#include "../core/SimpleMMTradingStrategy.h"
#ifndef CLOCK_INCL_H
#define CLOCK_INCL_H
#include "../core/Clock.h"
#endif

using namespace std;

class OrderBookSimpleMMTSLocal:public OrderBook {
private:
      string _symbol;
public:
      string symbol() { return _symbol; }
      double bestbid() { return 0.0; }
      double bestask() { return 0.0; }
      std::vector<std::pair<double,vector<OrderInfo>>> bidqueue() { 
          OrderInfo oi;
          std::vector<OrderInfo> v = {oi};
          std::pair<double,std::vector<OrderInfo>> p=pair(10.0,v);
          vector<pair<double,vector<OrderInfo>>> r={p};
          return r;
      }
      std::vector<std::pair<double,vector<OrderInfo>>> askqueue() { 
          OrderInfo oi;
          std::vector<OrderInfo> v = {oi};
          std::pair<double,std::vector<OrderInfo>> p=pair(10.0,v);
          vector<pair<double,vector<OrderInfo>>> r={p};
          return r;
      }
      OrderBookSimpleMMTSLocal(string symbol) {_symbol=symbol;}
      ~OrderBookSimpleMMTSLocal() { cerr << "OrderBookSimpleMMTSLocal Destructor:" << this << endl; }
};


class OrderBookSimpleMMTSLocalGeneric:public OrderBook {
private:
      string _symbol;
      double _bestbid = 0.0;
      double _bestask = 0.0;
      std::vector<std::pair<double,vector<OrderInfo>>> _bidqueue = initbidqueue();
      std::vector<std::pair<double,vector<OrderInfo>>> _askqueue = initaskqueue();

public:
      string symbol() { return _symbol; }
      double bestbid() { return _bestbid; }
      double bestask() { return _bestask; }
      void bestbid(double bid) { _bestbid = bid; }
      void bestask(double ask) { _bestask = ask; } 
      std::vector<std::pair<double,vector<OrderInfo>>> bidqueue() { return _bidqueue; }
      std::vector<std::pair<double,vector<OrderInfo>>> askqueue() { return _askqueue; }
      void bidqueue(std::vector<std::pair<double,vector<OrderInfo>>> q) {_bidqueue=q;}
      void askqueue(std::vector<std::pair<double,vector<OrderInfo>>> q) {_askqueue=q;}
      std::vector<std::pair<double,vector<OrderInfo>>> initbidqueue() { 
          OrderInfo oi;
          std::vector<OrderInfo> v = {oi};
          std::pair<double,std::vector<OrderInfo>> p=pair(10.0,v);
          vector<pair<double,vector<OrderInfo>>> r={p};
          return r;
      }
      std::vector<std::pair<double,vector<OrderInfo>>> initaskqueue() { 
          OrderInfo oi;
          std::vector<OrderInfo> v = {oi};
          std::pair<double,std::vector<OrderInfo>> p=pair(10.0,v);
          vector<pair<double,vector<OrderInfo>>> r={p};
          return r;
      }
      OrderBookSimpleMMTSLocalGeneric(string symbol) {_symbol=symbol;}
      ~OrderBookSimpleMMTSLocalGeneric() { cerr << "OrderBookSimpleMMTSLocalGeneric Destructor:" << this << endl; }
};

OrderInfo genOrderInfo(double qty, double price, char side) {
   return OrderInfo("senderid1","1111","TSLA.US",'D',qty,price,side,time(0));
}


DEFINE_TEST(SimpleMMTradingStrategyInit) {
   Clock c;
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("ID1","USD", 1000000, "TSLA.US", 0, 220, 225, 10000, 30000, 100, 0.01, 0.3, &c);
   TEST("ID1"==ts.id());
   TEST("USD"==ts.ccy());
   TEST("TSLA.US"==ts.symbol());
   TEST(0.0==ts.initinstrumentbalance());
   TEST(1000000.0==ts.initcashbalance());
   TEST(220==ts.securitysecmarketbestbid());
   TEST(10000==ts.securitysecmarketbestbidqty());
   TEST(225==ts.securitysecmarketbestask());
   TEST(30000==ts.securitysecmarketbestaskqty());
   TEST(1000000.0==ts.cashbalance());
   TEST(0.0==ts.instrumentbalance());
   TEST(222.5==ts.initinstrumentsecmarketprice());
   TEST(100==ts.orderqty());
   TEST(0.01==ts.pricemargin());
   TEST(0.3==ts.orderqtymargin());
   TEST(0==ts.ismminprogress());
   TEST(0==ts.ordercreated());
   TEST("ID1_ORD_1"==ts.nextorderid()); 
   TEST(100*1.3==ts.bufferredorderqty());
   TEST(1==ts.outstandingorder().isdummy());
   OrderBookSimpleMMTSLocal ob1 = OrderBookSimpleMMTSLocal("TSLA.US");
   OrderBookSimpleMMTSLocal ob2 = OrderBookSimpleMMTSLocal("MSFT.US");
   TEST(1==ts.isorderbookrelevant(ob1));
   TEST(0==ts.isorderbookrelevant(ob2));

}


DEFINE_TEST(SimpleMMTradingStrategyUpdateSecMarket) {
   Clock c;
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("ID2","USD", 1000000, "TSLA.US", 0, 220, 225, 10000, 30000, 100, 0.01, 0.3, &c);
   ts.setsecmarket(218, 220, 2000, 4000);
   TEST(218==ts.securitysecmarketbestbid());
   TEST(2000==ts.securitysecmarketbestbidqty());
   TEST(220==ts.securitysecmarketbestask());
   TEST(4000==ts.securitysecmarketbestaskqty());
}

DEFINE_TEST(SimpleMMTradingStrategyOnExecutionIrrelevantSymNoStrategyTriggered) {
   Clock c = Clock(1697373408);
   c.manipulate(1);
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("ID1","USD", 1000000, "TSLA.US", 0, 220, 225, 10000, 30000, 100, 0.01, 0.3, &c);
   OrderBookSimpleMMTSLocalGeneric ob = OrderBookSimpleMMTSLocalGeneric("MSFT.US");
   ob.bestbid(220);
   ob.bestask(225);
   OrderInfo oi;
   ExecutionInfo ei;
   ob.bidqueue({{220,{oi}},{219,{oi}},{218,{oi}}});
   ob.askqueue({{225,{oi}},{226,{oi}},{227,{oi}}});

   ts.onOrderExecution(ob,oi,ei);
   TEST(0==ts.ismminprogress());
}


DEFINE_TEST(SimpleMMTradingStrategyOnExecutionNoPriceDiffNoStrategyTriggered) {
   Clock c = Clock(1697373408);
   c.manipulate(1);
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("ID1","USD", 1000000, "TSLA.US", 0, 220, 225, 10000, 30000, 100, 0.01, 0.3, &c);
   OrderBookSimpleMMTSLocalGeneric ob = OrderBookSimpleMMTSLocalGeneric("TSLA.US");
   ob.bestbid(220);
   ob.bestask(225);
   OrderInfo oi;
   ExecutionInfo ei;
   
   ob.bidqueue({{220,{genOrderInfo(10000,220,'1')}},{219,{genOrderInfo(10000,219,'1')}},{218,{genOrderInfo(10000,218,'1')}}});
   ob.askqueue({{225,{genOrderInfo(10000,220,'2')}},{226,{genOrderInfo(10000,219,'2')}},{227,{genOrderInfo(10000,218,'2')}}});
   ts.onOrderExecution(ob,oi,ei);
   TEST(0==ts.ismminprogress());
}


DEFINE_TEST(SimpleMMTradingStrategyOnExecutionPrimaryBidSlightHigherThanSecondaryAskNoStrategyTriggered) {
   Clock c = Clock(1697373408);
   c.manipulate(1);
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("ID1","USD", 1000000, "TSLA.US", 0, 224.8, 225, 10000, 30000, 100, 0.01, 0.3, &c);
   OrderBookSimpleMMTSLocalGeneric ob = OrderBookSimpleMMTSLocalGeneric("TSLA.US");
   ob.bestbid(225.1);
   ob.bestask(225.3);
   OrderInfo oi;
   ExecutionInfo ei;
   
   ob.bidqueue({{225.1,{genOrderInfo(10000,225.1,'1')}},{225.2,{genOrderInfo(10000,225.2,'1')}},{225.3,{genOrderInfo(10000,225.3,'1')}}});
   ob.askqueue({{225.3,{genOrderInfo(10000,225.3,'2')}},{225.4,{genOrderInfo(10000,225.4,'2')}},{225.5,{genOrderInfo(10000,225.5,'1')}}});
   ts.onOrderExecution(ob,oi,ei);
   TEST(0==ts.ismminprogress());

}

int main()
{
	bool pass = true;

	pass &= TestFixture::ExecuteAllTests(0,0,TestFixture::Verbose);

	return pass ? 0 : 1;
}


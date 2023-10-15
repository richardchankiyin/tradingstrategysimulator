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


int main()
{
	bool pass = true;

	pass &= TestFixture::ExecuteAllTests(0,0,TestFixture::Verbose);

	return pass ? 0 : 1;
}


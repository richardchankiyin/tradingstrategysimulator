#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../simpletest/simpletest.cpp"

#include "../core/SimpleMMTradingStrategy.h"

using namespace std;

DEFINE_TEST(SimpleMMTradingStrategyInit) {
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("ID1","USD", 1000000, "TSLA.US", 0, 220, 225, 10000, 30000, 100, 0.01, 0.3);
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
}


DEFINE_TEST(SimpleMMTradingStrategyUpdateSecMarket) {
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("ID2","USD", 1000000, "TSLA.US", 0, 220, 225, 10000, 30000, 100, 0.01, 0.3);
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


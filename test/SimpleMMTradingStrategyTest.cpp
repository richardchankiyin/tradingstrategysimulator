#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../simpletest/simpletest.cpp"

#include "../core/SimpleMMTradingStrategy.h"

using namespace std;

DEFINE_TEST(SimpleMMTradingStrategyInit) {
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("USD", 1000000, "TSLA.US", 0, 220, 225, 10000, 30000, 100);
   TEST("USD"==ts.ccy());
   TEST("TSLA.US"==ts.securityid());
   TEST(0.0==ts.initinstrumentbalance());
   TEST(1000000.0==ts.initcashbalance());
   TEST(220==ts.securitysecmarketbestbid());
   TEST(10000==ts.securitysecmarketbestbidqty());
   TEST(225==ts.securitysecmarketbestask());
   TEST(30000==ts.securitysecmarketbestaskqty());
   TEST(1000000.0==ts.cashbalance());
   TEST(0.0==ts.instrumentbalance());
   cout << ts.initinstrumentsecmarketprice() << endl;
   TEST(222.5==ts.initinstrumentsecmarketprice());
   TEST(100==ts.orderqty());
}


DEFINE_TEST(SimpleMMTradingStrategyUpdateSecMarket) {
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("USD", 1000000, "TSLA.US", 0, 220, 225, 10000, 30000, 100);
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


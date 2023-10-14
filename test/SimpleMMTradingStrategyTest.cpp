#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../simpletest/simpletest.cpp"

#include "../core/SimpleMMTradingStrategy.h"

using namespace std;

DEFINE_TEST(SimpleMMTradingStrategyInit) {
   SimpleMMTradingStrategy ts = SimpleMMTradingStrategy("USD", 1000000, "TSLA.US", 0, 220, 225, 10000, 30000);
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
}



int main()
{
	bool pass = true;

	pass &= TestFixture::ExecuteAllTests(0,0,TestFixture::Verbose);

	return pass ? 0 : 1;
}


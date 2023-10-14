#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../simpletest/simpletest.cpp"
#include "../core/PositionKeeper.h"
using namespace std;

DEFINE_TEST(PositionKeepInit) {
   PositionKeeper pk = PositionKeeper("USD",1000000.0,"TSLA.US",0);
   TEST("USD"==pk.ccy());
   TEST("TSLA.US"==pk.securityid());
   TEST(0.0==pk.instrumentbalance());
   TEST(1000000.0==pk.cashbalance());
}

DEFINE_TEST(PositionKeepAddCash) {
   PositionKeeper pk = PositionKeeper("USD",1000000.0,"TSLA.US",0);
   pk.addcash(3000);
   TEST(1003000.0==pk.cashbalance());
}

DEFINE_TEST(PositionKeepAddInstrument) {
   PositionKeeper pk = PositionKeeper("USD",1000000.0,"TSLA.US",0);
   pk.addinstrument(300);
   TEST(300.0==pk.instrumentbalance());
}

DEFINE_TEST(PositionKeepDeductCash) {
   PositionKeeper pk = PositionKeeper("USD",1000000.0,"TSLA.US",0);
   pk.addcash(-3000);
   TEST(997000.0==pk.cashbalance());
}

DEFINE_TEST(PositionKeepDeductInstrument) {
   PositionKeeper pk = PositionKeeper("USD",1000000.0,"TSLA.US",0);
   pk.addinstrument(300);
   TEST(300.0==pk.instrumentbalance());
   pk.addinstrument(-200);
   TEST(100.0==pk.instrumentbalance());
   pk.addinstrument(-500);
   TEST(-400.0==pk.instrumentbalance());
}


int main()
{	
	bool pass = true;

	pass &= TestFixture::ExecuteAllTests(0,0,TestFixture::Verbose);

	return pass ? 0 : 1;
}

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../simpletest/simpletest.cpp"
#include "../core/ExecutionReport.h"

using namespace std;
DEFINE_TEST(ExecutionReportInit) {
   time_t t=time(0);
   ExecutionReport er = ExecutionReport("EXEC111","TSLA.US","ORD111",'8',220,2000,1000,1500,'1',t);
   TEST("EXEC111"==er.execid());
   TEST("TSLA.US"==er.symbol());
   TEST("ORD111"==er.orderid());
   TEST('8'==er.orderstatus());
   TEST(2000==er.qty());
   TEST(1000==er.lastqty());
   TEST(1500==er.cumqty());
   TEST(500==er.outstandingqty());
   TEST('1'==er.side());
   TEST(t==er.exectime());
}


int main()
{
	bool pass = true;

	pass &= TestFixture::ExecuteAllTests(0,0,TestFixture::Verbose);

	return pass ? 0 : 1;
}


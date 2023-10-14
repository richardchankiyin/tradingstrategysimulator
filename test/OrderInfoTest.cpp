#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../simpletest/simpletest.cpp"
#include "../core/OrderInfo.h"

using namespace std;
DEFINE_TEST(OrderInfoMarketOrderNewOrderSingleInit) {
   time_t t=time(0);
   OrderInfo oi = OrderInfo("senderid1","1111","TSLA.US",'D',1000,'1',t);
   TEST("senderid1"==oi.senderid());
   TEST("1111"==oi.orderid());
   TEST("TSLA.US"==oi.symbol());
   TEST('D'==oi.msgtype());
   TEST(1000==oi.orderqty());
   TEST('1'==oi.ordertype());
   TEST('1'==oi.side());
   TEST(t==oi.transacttime());
}

DEFINE_TEST(OrderInfoLimitOrderNewOrderSingleInit) {
   time_t t=time(0);
   OrderInfo oi = OrderInfo("senderid1","1111","TSLA.US",'D',1000,225.3,'1',t);
   TEST("senderid1"==oi.senderid());
   TEST("1111"==oi.orderid());
   TEST("TSLA.US"==oi.symbol());
   TEST('D'==oi.msgtype());
   TEST(1000==oi.orderqty());
   TEST('2'==oi.ordertype());
   TEST(225.3==oi.price());
   TEST('1'==oi.side());
   TEST(t==oi.transacttime());
}

DEFINE_TEST(OrderInfoCancelOrderInit) {
   time_t t=time(0);
   OrderInfo oi = OrderInfo("senderid1","1111",t);
   TEST("senderid1"==oi.senderid());
   TEST("1111"==oi.orderid());
   TEST('G'==oi.msgtype());
   TEST(t==oi.transacttime());
}

DEFINE_TEST(OrderInfoQtyCalc) {
   time_t t=time(0);
   OrderInfo oi = OrderInfo("senderid1","1111","TSLA.US",'D',1000,225.3,'1',t);
   oi.lastqty(20);
   TEST(20==oi.lastqty());
   TEST(20==oi.cumqty());
   TEST(980==oi.outstandingqty());
   oi.lastqty(60);
   TEST(60==oi.lastqty());
   TEST(80==oi.cumqty());
   TEST(920==oi.outstandingqty());
}


int main()
{
	bool pass = true;

	pass &= TestFixture::ExecuteAllTests(0,0,TestFixture::Verbose);

	return pass ? 0 : 1;
}


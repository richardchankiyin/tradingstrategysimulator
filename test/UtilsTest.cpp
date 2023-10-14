#include <iostream>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include "../simpletest/simpletest.cpp"

#include "../core/Utils.h"

using namespace std;

DEFINE_TEST(avgpriceemptyqueue) {
   std::vector<std::pair<double,vector<OrderInfo>>> q;
   TEST(-1==Utils::avgprice(q,1000));
}

DEFINE_TEST(avgpricequeuedepthlessthanqty) {
   time_t t=time(0);
   OrderInfo oi1 = OrderInfo("senderid1","1111","TSLA.US",'D',1000,220,'1',t);
   OrderInfo oi2 = OrderInfo("senderid1","1111","TSLA.US",'D',2000,220,'1',t);
   OrderInfo oi3 = OrderInfo("senderid1","1111","TSLA.US",'D',3000,220.5,'1',t);
   std::vector<std::pair<double,vector<OrderInfo>>> q = {{220,{oi1,oi2}},{220.5,{oi3}}};
   double actualresult = Utils::avgprice(q,10000);
   TEST(-1==actualresult);
}

DEFINE_TEST(avgpricequeuedepthmorethanqty) {
   time_t t=time(0);
   OrderInfo oi1 = OrderInfo("senderid1","1111","TSLA.US",'D',1000,220,'1',t);
   OrderInfo oi2 = OrderInfo("senderid1","1111","TSLA.US",'D',2000,220,'1',t);
   OrderInfo oi3 = OrderInfo("senderid1","1111","TSLA.US",'D',3000,220.5,'1',t);
   std::vector<std::pair<double,vector<OrderInfo>>> q = {{220,{oi1,oi2}},{220.5,{oi3}}};
   double actualresult = Utils::avgprice(q,1000);
   TEST(220==actualresult);
   actualresult = Utils::avgprice(q,1500);
   TEST(220==actualresult);
   actualresult = Utils::avgprice(q,3000);
   TEST(220==actualresult);
   actualresult = Utils::avgprice(q,4000);
   TEST((220*3000+220.5*1000)/4000==actualresult); 
}





int main()
{
	bool pass = true;

	pass &= TestFixture::ExecuteAllTests(0,0,TestFixture::Verbose);

	return pass ? 0 : 1;
}


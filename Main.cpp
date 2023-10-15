#include <iostream>
#include <map>
#include <deque>
#include <vector>

#ifndef ORDERINFO_INCL_H
#define ORDERINFO_INCL_H
#include "core/OrderInfo.h"
#endif

#ifndef EXECUTIONINFO_INCL_H
#define EXECUTIONINFO_INCL_H
#include "core/ExecutionInfo.h"
#endif

#ifndef EXECUTIONREPORT_INCL_H
#define EXECUTIONREPORT_INCL_H
#include "core/ExecutionReport.h"
#endif


#ifndef POSITIONKEEPER_INCL_H
#define POSITIONKEEPER_INCL_H
#include "core/PositionKeeper.h"
#endif


#ifndef ORDERBOOKIMPL_INCL_H
#define ORDERBOOKIMPL_INCL_H
#include "core/OrderBookImpl.h"
#endif

using namespace std;

struct kv {
    int key;
    string value;
};

int main() {
   cout << "testing" << endl;
   OrderInfo o = OrderInfo("sender1","111","TSLA.US",'D',100.0,'1');
   ExecutionReport erbuy = ExecutionReport("222","NVDA.US","333",'2',465.2,100.0,100.0);
   ExecutionReport ersell = ExecutionReport("444","NVDA.US","333",'2',465.2,100.0,100.0);
   ExecutionInfo ei = ExecutionInfo("333","MSFT.US",330.3,10,'A',erbuy,ersell);
   cout << o.symbol() << endl;
   cout << erbuy.symbol() << endl;
   cout << ersell.symbol() << endl;
   cout << ei.symbol() << endl;
   std::multimap<string,OrderInfo> ordermap = {{o.orderid(),o}};
   bool found = ordermap.contains("111");
   if (found) {
      cout << "found" << endl;
   }
   OrderInfo item = ordermap.find("111")->second;
   cout << ((&item == &o) ? "T" : "F") << endl;
   cout << "Order Id: " << o.orderid() << endl;
   cout << "Order time: " << o.transacttime() << endl;
   std::map<string, const OrderInfo&> ordermap2 = {{o.orderid(),o}};
   OrderInfo item2 = ordermap.find("111")->second;
   cout << ((&item2 == &o) ? "T" : "F") << endl;
   cout << "size of OrderInfo o:" << sizeof(o) << endl;
   kv kv1;
   kv1.key = 123;
   kv1.value = "str123";

   kv kv2;
   kv2.key = 345;
   kv2.value = "str345";

   std::map<string,kv> map = {{"222",kv1},{"222",kv2}};
   cout << "map count:" << map.size() << endl;
   kv kv3 = map.find("222")->second;
   cout <<  ((&kv2 == &kv3) ? "T" : "F") << endl;

   std::deque<string> deque;
   deque.push_back("123"); deque.push_back("345");deque.push_back("456");
   cout << "deque:" << deque.at(0)  << "|" << deque.at(1) << "|" << deque.at(2) << endl;
   deque.erase(deque.begin()+1);
   cout << "deque:" << deque.at(0)  << "|" << deque.at(1) << endl;
   std::deque<OrderInfo> dequeorder;

   OrderInfo o2 = OrderInfo("sender2","222","TSLA.US",'D',110.0,'1');
   OrderInfo o3 = OrderInfo("sender3","333","TSLA.US",'D',115.0,'1');

   dequeorder.push_back(o);dequeorder.push_back(o2);dequeorder.push_back(o3);

   cout << "dequeorder:" << dequeorder.at(0).orderid()  << "|" << dequeorder.at(1).orderid() << "|" << dequeorder.at(2).orderid() << endl;
   dequeorder.erase(dequeorder.begin()+1);
   cout << "dequeorder:" << dequeorder.at(0).orderid()  << "|" << dequeorder.at(1).orderid()  << endl;

   OrderBookImpl obook;
   cout << "best bid:" << obook.bestbid() << " best ask:" << obook.bestask() << endl;
   cout << "bid queue:" << obook.bidqueue()[0].first << " presented" << endl; 
   std::vector<std::pair<int,string>> vector = {{1,"222"},{2,"333"}};
   cout << "vector item 0: " << vector[0].first << "," << vector[0].second << endl;  

   OrderInfo o4 = OrderInfo("sender4","444","TSLA.US",'D',115.0,'1');
   cout << "Order 4 before: " << o4.orderid() << " cumqty: "<< o4.cumqty() << endl;
   o4.lastqty(110);
   cout << "Order 4 after: " << o4.orderid() << " cumqty: "<< o4.cumqty() << endl;

   PositionKeeper pk = PositionKeeper("USD",1000000.0,"TSLA.US",0);
   cout << "PK Init" <<  pk.ccy() << "," << pk.cashbalance() << "," << pk.symbol() << "," << pk.instrumentbalance() << endl;
   pk.addcash(20000);
   cout << "PK Added Cash" <<  pk.ccy() << "," << pk.cashbalance() << "," << pk.symbol() << "," << pk.instrumentbalance() << endl;
   pk.addinstrument(100);
   cout << "PK Added Instrument" <<  pk.ccy() << "," << pk.cashbalance() << "," << pk.symbol() << "," << pk.instrumentbalance() << endl;

   return 0;
}

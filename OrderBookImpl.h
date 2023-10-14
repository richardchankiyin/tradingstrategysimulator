#include <iostream>
#include <ctime>
#include <vector>

#ifndef ORDERBOOK_INCL_H
#define ORDERBOOK_INCL_H 
#include "OrderBook.h"
#endif

class OrderBookImpl:public OrderBook {
public:  
      double bestbid() { return 0.0; }
      double bestask() { return 0.0; }
      std::vector<std::pair<double,vector<OrderInfo>>> bidqueue() { 
          OrderInfo oi;
          std::vector<OrderInfo> v = {oi};
          std::pair<double,std::vector<OrderInfo>> p=pair(10.0,v);
          vector<pair<double,vector<OrderInfo>>> r={p};
          return r;
      }
      std::vector<std::pair<double,vector<OrderInfo>>> sellqueue() { 
          OrderInfo oi;
          std::vector<OrderInfo> v = {oi};
          std::pair<double,std::vector<OrderInfo>> p=pair(10.0,v);
          vector<pair<double,vector<OrderInfo>>> r={p};
          return r;
      }
      OrderBookImpl() { }
      ~OrderBookImpl() { cout << "OrderBookImpl Destructor:" << this << endl; }
};


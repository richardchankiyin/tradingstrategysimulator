#ifndef ORDERBOOK_H
#define ORDERBOOK_H 

#include <iostream>
#include <vector>
#include <ctime>

#ifndef ORDERINFO_INCL_H
#define ORDERINFO_INCL_H
#include "OrderInfo.h"
#endif

using namespace std;

class OrderBook {
public:
    virtual double bestbid() = 0;
    virtual double bestask() = 0;
    virtual std::vector<std::pair<double,vector<OrderInfo>>> bidqueue() = 0;
    virtual std::vector<std::pair<double,vector<OrderInfo>>> sellqueue() = 0;
    virtual void acceptorder(const OrderInfo& orderInfo) {}
    virtual ~OrderBook() {}   
};
#endif

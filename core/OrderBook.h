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
    virtual string symbol() = 0;
    virtual double bestbid() = 0;
    virtual double bestask() = 0;
    virtual std::vector<std::pair<double,vector<OrderInfo>>> bidqueue() = 0;
    virtual std::vector<std::pair<double,vector<OrderInfo>>> askqueue() = 0;
    virtual void receiveorder(const OrderInfo& orderInfo) {}
    virtual void addorder(const OrderInfo& orderInfo) {}
    virtual void cancelorder(const OrderInfo& orderInfo) {}
    virtual ~OrderBook() {}   
};
#endif

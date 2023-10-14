#include <iostream>
using namespace std;
#ifndef ORDERBOOK_INCL_H
#define ORDERBOOK_INCL_H
#include "OrderBook.h"
#endif

#ifndef ORDERINFO_INCL_H
#define ORDERINFO_INCL_H
#include "OrderInfo.h"
#endif

#ifndef EXECUTIONINFO_INCL_H
#define EXECUTIONINFO_INCL_H
#include "ExecutionInfo.h"
#endif

class TradingStrategy {
public:
     virtual void onOrderAdd(OrderBook& orderBook, const OrderInfo& orderInfo) = 0;
     virtual void onOrderCancel(OrderBook& orderBook, const OrderInfo& orderInfo) = 0;
     virtual void onOrderExecution(OrderBook& orderBook, const OrderInfo& orderInfo, const ExecutionInfo& executionInfo) = 0;
     virtual void onSendOrder(OrderBook& orderBook, const OrderInfo& orderInfo) = 0;
};
